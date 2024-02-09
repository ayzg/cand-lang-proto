#pragma once
#include <vector>
#include <stdexcept>
#include <typeinfo>
#include <typeindex>
#include <map>
#include "char_traits.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "ast_node.hpp"
#include <stack>
#include "syntax_traits.hpp"
#include "parser_utils.hpp"

// <@macro:caoco_PARSING_PROCESS_DEF> Defines a parsing process functor.
#define caoco_PARSING_PROCESS_DEF(p) struct p : public parsing_process \
{ p() : parsing_process(typeid(p)) {} parsing_result perform(tk_vector_cit begin, tk_vector_cit end);}

// <@macro:caoco_PARSING_PROCESS_IMPL> Defines a parsing process functor's perform method.
#define caoco_PARSING_PROCESS_IMPL(p) 	parsing_result p::perform(tk_vector_cit begin, tk_vector_cit end)

namespace caoco {

	// <@class:parsing_result>
	// <@brief> A struct containing the result of a parsing method.
	// <@member:cursor> The end of the parsed statement.
	// <@member:valid>	If the statement is valid, error handling will be done by caller.
	// <@member:error_message>	The error message if the statement is invalid. 
	//							NOTE: Error messages are passed up a call stack through a parsing_process.
	struct parsing_result {
		tk_vector_cit it_;
		astnode node_;
		bool valid_{ true };
		sl_string error_message_{ "" };

		parsing_result() = default;
		parsing_result(astnode node,tk_vector_cit it = {}, bool valid = true, sl_string error_message = "")
			: node_(node),it_(it), valid_(valid), error_message_(error_message) {}

		constexpr tk_vector_cit it() const noexcept { return it_; }
		constexpr astnode& node() noexcept { return node_; }
		constexpr bool valid() const noexcept { return valid_; }
		constexpr sl_string error_message() const noexcept { return error_message_; }
	};

	// <@class:parsing_process> virtual base class for a parsing process functor.
	class parsing_process {
		std::stringstream error_stream_;
		parsing_result make_result(astnode node, tk_vector_cit cursor, bool valid = true, sl_string error_message = "") {
			error_stream_ << error_message << "\n";
			return parsing_result{ node,cursor, valid, error_stream_.str() };
		}
	protected:
		parsing_result make_error(tk_vector_cit cursor, tk offending_token, sl_string error_message) {
			error_stream_ << "\n" << error_message << " Offending token: " << sl::to_str(offending_token.literal())
				<< "| Line: " << offending_token.line() << "| Col: " << offending_token.col() << "\n";
			return parsing_result{ astnode(astnode_enum::invalid_),cursor, false, error_stream_.str() };
		}
		parsing_result make_success(astnode node,tk_vector_cit end) {
			return make_result(node,end, true);
		}
		parsing_result make_success(astnode node) {
			return make_result(node, node.token_end(), true);
		}
		parsing_result make_pass(tk_vector_cit it) {
			return make_result(astnode(astnode_enum::none_),it, false);
		}

		parsing_process(const sl_type_info& parsing_process_type) { error_stream_ << "[" << parsing_process_type.name() << "]"; };
		parsing_process(const parsing_process& other) {
			error_stream_ << other.error_stream_.str();
		};
		virtual parsing_result perform(tk_vector_cit begin, tk_vector_cit end) = 0;
	public:
		parsing_result operator()(tk_vector_cit begin, tk_vector_cit end) {
			return perform(begin, end);
		}
		virtual ~parsing_process() = default;
	};

	sl_opt<astnode> build_statement(tk_vector_cit begin,
		tk_vector_cit end, sl_opt<astnode> last_pass = sl::nullopt) {
		tk_cursor it(begin,end);

		if (!last_pass.has_value()) {
			// Determine the following operator and first operand.
			if (it.operation() == e_operation::unary_) { // This expression starts with a unary operation.
				astnode unary_operation = it.to_statement();
				if (*it.next(2) == end) { // Unary operation is not followed by operand.
					unary_operation.push_back(it.next().to_statement()); // lhs of unary op is the operand of the unary op.
					return unary_operation; // Entire statement is this unary operation.
				}
				else { // Unary operation is followed by operand.
					if (it.priority() < it.next(2).priority()) { // Unary operation is less important than next operation.
						astnode next_operation = it.next(2).to_statement();	// next op is the next operation.
						next_operation.push_back(it.next().to_statement());	// lhs of next op is the operand of the unary op.
						unary_operation.push_back(build_statement(*it.next(2), end, std::make_optional(next_operation)).value()); // Rest of expr is operand of unary op.
						return std::make_optional(unary_operation);	// Entire statement is a unary operation with rest of expr as the operand.
					}
					else if (it.priority() >= it.next(2).priority()) { // Unary operation is more or equally important than next operation.
						astnode next_pass = it.next(2).to_statement(); // next pass is the next operation.
						unary_operation.push_back(it.next().to_statement()); // lhs of unary op is the operand of the unary op.
						next_pass.push_back(unary_operation); // lhs of next pass is the unary op.
						return build_statement(*it.next(2), end, std::make_optional(next_pass));  // Rest of expr is the next pass.
					}
				}
			}
			else if (it.type() == tk_enum::open_scope_) { // This expression starts with a scope.
				parser_scope_result scope = find_scope(*it, end);
				if (!scope.valid) { // If the scope is invalid, throw an error.
					throw std::runtime_error("Mismatched parenthesis.");
				}
				else if (scope.is_empty()) { // If the scope is empty, throw an error.
					throw std::runtime_error("Empty parenthesis.");
				}
				else {
					if (scope.scope_end() == end) {	// Scope is redundant, the entire statement is the scope.
						return build_statement(scope.contained_begin(), scope.contained_end());
					}
					else { // If the scope is followed by an operator, the scope is a lhs operand.
						astnode next_pass = tk_cursor(scope.scope_end(),end).to_statement(); // next pass is the following operator.
						next_pass.push_back(build_statement(scope.contained_begin(), scope.contained_end()).value()); // lhs of next pass is the scope.
						return build_statement(scope.scope_end(), end, std::make_optional(next_pass)); // Rest of expr is the next pass.
					}
				}
			}
			else { // first token is assumed to be a singular operand NOTE: (may change this later to throw an error on invalid token)
				if (*it.next() == end) { // If there is no following operator. This is the last pass.
					return it.to_statement();
				}
				else if (it.next().type_is(tk_enum::open_scope_)) { // special case for function call
					// Everything within the scope is the arguments of the function call.
					// The function call is the lhs of the following operator.
					parser_scope_result arg_scope = find_scope(*it.next(), end);
					if (!arg_scope.valid) {
						throw std::runtime_error("Mismatched parenthesis in arguments to function call operator.");
					}

					astnode function_call = astnode(astnode_enum::function_call_, *it, arg_scope.scope_end());
					function_call.push_back(it.to_statement()); // lhs of function call is the this operand.
					function_call.push_back({astnode_enum::arguments_,arg_scope.contained_begin(), arg_scope.contained_end()}); // rhs of function call is the arguments.

					if (arg_scope.scope_end() == end) { // If there is no following operator the entire statement is the function call.
						return function_call;
					}
					else { 
						astnode first_pass = tk_cursor(arg_scope.scope_end(), end).to_statement(); // first pass is the following operator.
						first_pass.push_back(function_call); // lhs of first pass is the function call.
						return build_statement(arg_scope.scope_end(), end, first_pass); // Rest of expr is the first pass.
					}
				}
				else { // If there is a following operator. This is the first pass.
					astnode first_pass = it.next().to_statement(); // first pass is the following operator.
					first_pass.push_back(it.to_statement()); // lhs of first pass is the this operand.
					return build_statement(*it.next(), end, first_pass); // Rest of expr is the first pass.
				}
			}
		}
		else {
			// This is a following pass, assume 'it' is a binary operator. 
			// Assume last_pass is an unfinished binary operation of type binary operator.
			// Do some input validation first to reduce code duplication.
			if (*it.next() == end || it.next().type_is(tk_enum::eof_)) { // If there is no following operand. Error.Binary Operator must be followed by operand.
				throw std::runtime_error("End of expression after binary operator. Operator must be followed by operand.");
			}

			// We will check for single operands, if it is not. Then we assume it must be a scope or a unary operator.Otherwise Error.
			if (it.next().type() != tk_enum::number_literal_
				&& it.next().type() != tk_enum::alnumus_) { // is not a single operand?
				if (it.next().operation() == e_operation::unary_) { // is a unary operator?
					if (*it.next(2) == end) {
						throw std::runtime_error("End of expression after unary operator. Operator must be followed by operand.");
					}
				}
				else if (it.next().type() == tk_enum::open_scope_) { // is a scope?
					parser_scope_result scope = find_scope(*it.next(), end);
					if (!scope.valid) {
						throw std::runtime_error("Mismatched parenthesis.");
					}
					else if (scope.is_empty()) {
						throw std::runtime_error("Empty parenthesis.");
					}
				}
				else  // is not a unary operator or a scope? Error!
					throw std::runtime_error("Invalid right hand side operand. Operator must be followed by operand.");
			}

			// Determine the following operator and operand.
			// If the following operand token is a unary operation, the next operator is the one after that operation.
			// If the following operand is a scope, everything within the scope is a statement- which is the operand. 
			//		. next operator is the one after the scope.
			// Else the next operator is the token after the following operand.
			tk_vector_cit next_operator_it;
			if (it.next().operation() == e_operation::unary_) {
				next_operator_it = *it.next(3);
			}
			else if (it.next().type() == tk_enum::open_scope_) {
				parser_scope_result scope = find_scope(*it.next(), end);
				next_operator_it = scope.scope_end();
			}
			else {
				next_operator_it = *it.next(2);
			}

			tk_cursor next_op_cursor = tk_cursor(next_operator_it, end);
			astnode optional_function_call = astnode(astnode_enum::none_);
			// Special case for function call
			if (next_op_cursor.type_is(tk_enum::open_scope_)) {
				// Operand followed by a scope is a function call.
				// Everything within the scope is the arguments of the function call.
				auto arg_scope = find_scope(*next_op_cursor, end);
				if (!arg_scope.valid) {
					throw std::runtime_error("Mismatched parenthesis in arguments to function call operator.");
				}

				auto function_call = astnode(astnode_enum::function_call_, *it, arg_scope.scope_end());
				function_call.push_back(it.next().to_statement());
				function_call.push_back({ astnode_enum::arguments_,arg_scope.contained_begin(), arg_scope.contained_end() });

				next_operator_it = arg_scope.scope_end();
				next_op_cursor = tk_cursor(next_operator_it, end);

				optional_function_call = function_call;
			}

			// If we are at the end of the expression, this is the last pass. Complete the binary operation based on associativity and rhs operand.
			if (next_operator_it == end) {
				if (it.associativity() == e_assoc::right_) { // right assoc push front next operand as lhs
					if (it.next().operation() == e_operation::unary_) { // next operand is a unary operation.
						last_pass.value().push_front(it.next().to_statement());
						last_pass.value().front().push_back(it.next(2).to_statement());
					}
					else if (it.next().type() == tk_enum::open_scope_) { // next operand is a scope.
						parser_scope_result scope = find_scope(*it, end);
						last_pass.value().push_front(build_statement(scope.contained_begin(), scope.contained_end()).value());
					}
					else { // next operand is a single operand.
						// if next operand is followed actually a function call use that as the rhs instead.
						if (optional_function_call.type() != astnode_enum::none_) {
							last_pass.value().push_front(optional_function_call);
						}
						else
							last_pass.value().push_front(it.next().to_statement());
					}
					return last_pass;
				}
				else { // left assoc push back next operand as rhs
					if (it.next().operation() == e_operation::unary_) { // next operand is a unary operation.
						last_pass.value().push_back(it.next().to_statement());
						last_pass.value().back().push_back(it.next(2).to_statement());
					}
					else if (it.next().type() == tk_enum::open_scope_) { // next operand is a scope.
						parser_scope_result scope = find_scope(*it.next(), end);
						last_pass.value().push_back(build_statement(scope.contained_begin(), scope.contained_end()).value());
					}
					else { // next operand is a single operand.
						// if next operand is followed actually a function call use that as the rhs instead.
						if (optional_function_call.type() != astnode_enum::none_) {
							last_pass.value().push_back(optional_function_call);
						}
						else
							last_pass.value().push_back(it.next().to_statement());
					}
					return last_pass;
				}
			}
			else {
				// Else we are inside a binary operation. Check if the following operator is more or less important.
				if (it.priority() < next_op_cursor.priority()) { // More important?
					// Split the expression in 2 parts.Solve the right side first.Set as right hand side of the left side.Finished.
					if (it.associativity() == e_assoc::right_) { // right assoc swap lhs and rhs
						last_pass.value().push_front(build_statement(*it.next(), end, std::nullopt).value()); // lhs is the rest of the expression.
						return last_pass; // End of expr.
					}
					else { // left assoc
						last_pass.value().push_back(build_statement(*it.next(), end, std::nullopt).value()); // rhs is the rest of the expression.
						return last_pass; // End of expr.
					}
				}
				else if (it.priority() >= next_op_cursor.priority()) { // Less important ?
					// .	This operator is a finished expression.Solve it.
					//		Set as left hand side of next operator. Call self with next operator as the cursor.
					//		If the operator is right associative. The right and left hand side are swapped.
					astnode lhs_expression = astnode(last_pass.value().type()); // lhs is the last pass.
					if (it.associativity() == e_assoc::right_) { // right Assoc
						if (it.next().type() == tk_enum::open_scope_) { // next operand is a scope.
							parser_scope_result scope = find_scope(*it.next(), end); // find the scope.
							lhs_expression.push_back(build_statement(scope.contained_begin(), scope.contained_end()).value()); // solve and set as lhs.
							lhs_expression.push_back(last_pass.value().front()); // last operation's lhs is the rhs.
						}
						else if (it.next().operation() == e_operation::unary_) { // next operand is a unary operation.
							lhs_expression.push_back(it.next().to_statement()); // this unary op is the lhs.
							lhs_expression.back().push_back(it.next(2).to_statement()); // add operand to unary op.
							lhs_expression.push_back(last_pass.value().front()); // last operation's lhs is the rhs.
						}
						else { // next operand is a single operand.
							lhs_expression.push_back(it.next().to_statement()); // this operand is the lhs.
							lhs_expression.push_back(last_pass.value().front()); // last operation's lhs is the rhs.
						}
					}
					else { // left Assoc
						if (it.next().type() == tk_enum::open_scope_) { // next operand is a scope.
							parser_scope_result scope = find_scope(*it.next(), end); // find the scope.
							lhs_expression.push_back(last_pass.value().front()); // last operation's lhs is the lhs.
							lhs_expression.push_back(build_statement(scope.contained_begin(), scope.contained_end()).value()); // solve and set as rhs.

						}
						else if (it.next().operation() == e_operation::unary_) { // next operand is a unary operation.
							lhs_expression.push_back(last_pass.value().front()); // last operation's lhs is the lhs.
							lhs_expression.push_back(it.next().to_statement()); // this unary op is the rhs.
							lhs_expression.back().push_back(it.next(2).to_statement()); // add operand to unary op.
						}
						else { // next operand is a single operand.
							// if next operand is followed actually a function call use that as the rhs instead.
							if (optional_function_call.type() != astnode_enum::none_) {
								lhs_expression.push_back(last_pass.value().front()); // last operation's lhs is the lhs.
								lhs_expression.push_back(optional_function_call); // this operand is the rhs.
							}
							else {
								lhs_expression.push_back(last_pass.value().front()); // last operation's lhs is the lhs.
								lhs_expression.push_back(it.next().to_statement()); // this operand is the rhs.
							}
						}
					}

					astnode next_pass = next_op_cursor.to_statement(); // next pass is the next operator.
					next_pass.push_back(lhs_expression); // lhs of next pass is the lhs expression.
					return build_statement(next_operator_it, end, next_pass); // Rest of expr is the next pass.
				}
			}
		}
	}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Parsing Functor Definitions
// Parsing functors must be forward declared as they are used recursively.
	caoco_PARSING_PROCESS_DEF(ParsePragmaticBlock);
	caoco_PARSING_PROCESS_DEF(ParseFunctionalBlock);
	caoco_PARSING_PROCESS_DEF(ParseValueExpression);
	// ambigous statements part of a pragmatic block.
	caoco_PARSING_PROCESS_DEF(ParseIdentifierStatement);
	caoco_PARSING_PROCESS_DEF(ParseDirectiveType);
	caoco_PARSING_PROCESS_DEF(ParseDirectiveVar);
	caoco_PARSING_PROCESS_DEF(ParseDirectiveFunc);
	caoco_PARSING_PROCESS_DEF(ParseDirectiveClass);
	caoco_PARSING_PROCESS_DEF(ParseDirectiveNone);
	// Functional block statements
	caoco_PARSING_PROCESS_DEF(ParseDirectiveReturn); 
	caoco_PARSING_PROCESS_DEF(ParseDirectiveIf);
	// literals
	caoco_PARSING_PROCESS_DEF(ParseLiteral);
	caoco_PARSING_PROCESS_DEF(ParseStringLiteral);
	caoco_PARSING_PROCESS_DEF(ParseNumberLiteral);
	caoco_PARSING_PROCESS_DEF(ParseRealLiteral);
	caoco_PARSING_PROCESS_DEF(ParseAlnumusLiteral);
	caoco_PARSING_PROCESS_DEF(ParseUnsignedLiteral);
	caoco_PARSING_PROCESS_DEF(ParseOctetLiteral);
	caoco_PARSING_PROCESS_DEF(ParseBitLiteral);
	// candi special objects
	caoco_PARSING_PROCESS_DEF(ParseCandiSpecialObject);
	caoco_PARSING_PROCESS_DEF(ParseCsoType);
	caoco_PARSING_PROCESS_DEF(ParseCsoValue);
	caoco_PARSING_PROCESS_DEF(ParseCsoIdentity);
	caoco_PARSING_PROCESS_DEF(ParseCsoInt);
	caoco_PARSING_PROCESS_DEF(ParseCsoUint);
	caoco_PARSING_PROCESS_DEF(ParseCsoReal);
	caoco_PARSING_PROCESS_DEF(ParseCsoOctet);
	caoco_PARSING_PROCESS_DEF(ParseCsoBit);
	caoco_PARSING_PROCESS_DEF(ParseCsoPointer);
	caoco_PARSING_PROCESS_DEF(ParseCsoArray);

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Parsing Functor Implementations
	caoco_PARSING_PROCESS_IMPL(ParseCsoType) {
		assert(begin->type() == tk_enum::atype_ && "[PROGRAMMER LOGIC ERROR][ParseCsoType] begin is not atype_ token.");
		return make_success({ astnode_enum::atype_, begin, std::next(begin) });
	}
	caoco_PARSING_PROCESS_IMPL(ParseCsoValue) {
		assert(begin->type() == tk_enum::avalue_ && "[LOGIC ERROR][ParseCsoValue] begin is not avalue_ token.");
		return make_success({ astnode_enum::avalue_, begin, std::next(begin) });
	}
	caoco_PARSING_PROCESS_IMPL(ParseCsoIdentity) {
		assert(begin->type() == tk_enum::aidentity_ && "[LOGIC ERROR][ParseCsoIdentity] begin is not aid_ token.");
		return make_success({ astnode_enum::aidentity_, begin, std::next(begin) });
	}

	caoco_PARSING_PROCESS_IMPL(ParseCsoInt) {
		assert(begin->type() == tk_enum::aint_ && "[LOGIC ERROR][ParseCsoInt] begin is not aint_ token.");
		tk_cursor it(begin, end);
		using constrained_int_type_mask = std::tuple<
			tk_mask<tk_enum::aint_>, tk_mask<tk_enum::open_frame_>, tk_mask<tk_enum::subtraction_, mask_policy::optional>,
			tk_mask<tk_enum::number_literal_>, tk_mask<tk_enum::ellipsis_>, tk_mask<tk_enum::subtraction_, mask_policy::optional>,
			tk_mask<tk_enum::number_literal_>, tk_mask<tk_enum::close_frame_>>;
		// If '&int' is followed by an open frame '[' - it is a constrained integer type.
		// Format:
		// <constrained_int_type> 
		//		::= <aint> <open_frame> <unary_minus?> <number_literal>> <elipsis> <unary_minus?> <number_literal> <close_frame>
		//		::= <aint>
		// Example:		
		//	&int[-42...42]
		if (scan_tokens_pack<constrained_int_type_mask>(begin, end)) {
			astnode atype_node(astnode_enum::aint_);
			if (it.next(2).type_is(tk_enum::subtraction_)) {// If the first number is negative.
				auto& unary_minus = atype_node.push_back(it.next(2).to_statement()); // Add the unary minus to the node.
				unary_minus.push_back(it.next(3).to_statement()); // Add the number to the unary minus
				// Skip the ellipsis.
				if (it.next(5).type_is(tk_enum::subtraction_)) { // If the second number is negative.
									auto& unary_minus = atype_node.push_back(it.next(5).to_statement()); // Add the unary minus to the node.
									unary_minus.push_back(it.next(6).to_statement()); // Add the number to the unary minus
									// Skip the closed frame
									return make_success(atype_node, *it.next(8));
				}
				else {
					atype_node.push_back(it.next(5).to_statement()); // Add the number to the node.
					// Skip the closed frame.
					return make_success(atype_node, *it.next(7));
				}
			}
			else {
				atype_node.push_back(it.next(2).to_statement()); // Add the number to the node.
				// Skip the ellipsis.
				if (it.next(4).type_is(tk_enum::subtraction_)) { // If the second number is negative.
					auto& unary_minus = atype_node.push_back(it.next(4).to_statement()); // Add the unary minus to the node.
					unary_minus.push_back(it.next(5).to_statement()); // Add the number to the unary minus
					// Skip the closed frame
					return make_success(atype_node, *it.next(7));
				}
				else {
					atype_node.push_back(it.next(4).to_statement()); // Add the number to the node.
					// Skip the closed frame.
					return make_success(atype_node, *it.next(6));
				}
			}
		}
		else {
			return make_success({ astnode_enum::aint_, begin, std::next(begin) });
		}
	}
	caoco_PARSING_PROCESS_IMPL(ParseCsoUint) {
		assert(begin->type() == tk_enum::auint_ && "[LOGIC ERROR][ParseCsoUint] begin is not auint_ token.");
		tk_cursor it(begin, end);
		using constrained_uint_type_mask = std::tuple<
			tk_mask<tk_enum::auint_>, tk_mask<tk_enum::open_frame_>,	tk_mask<tk_enum::number_literal_>, 
			tk_mask<tk_enum::ellipsis_>,tk_mask<tk_enum::number_literal_>, tk_mask<tk_enum::close_frame_>>;
		if (scan_tokens_pack<constrained_uint_type_mask>(begin, end)) {
			astnode atype_node(astnode_enum::auint_);
			atype_node.push_back(it.next(2).to_statement()); // Add the number to the node.
			// Skip the ellipsis.
			atype_node.push_back(it.next(4).to_statement()); // Add the number to the node.
			// Skip the closed frame.
			return make_success(atype_node, *it.next(6));
		}
		else {
			return make_success(astnode_enum::auint_, *it.next());
		}
	}
	caoco_PARSING_PROCESS_IMPL(ParseCsoReal) {
		assert(begin->type() == tk_enum::areal_ && "[LOGIC ERROR][ParseCsoReal] begin is not areal_ token.");
		return make_success({ astnode_enum::areal_, begin, std::next(begin) });
	}
	caoco_PARSING_PROCESS_IMPL(ParseCsoOctet) {
		assert(begin->type() == tk_enum::aoctet_ && "[LOGIC ERROR][ParseCsoOctet] begin is not aoctet_ token.");
		return make_success({ astnode_enum::aoctet_, begin, std::next(begin) });
	}
	caoco_PARSING_PROCESS_IMPL(ParseCsoBit) {
		assert(begin->type() == tk_enum::abit_ && "[LOGIC ERROR][ParseCsoBit] begin is not abit_ token.");
		return make_success({ astnode_enum::abit_, begin, std::next(begin) });
	}
	caoco_PARSING_PROCESS_IMPL(ParseCsoPointer) {
		assert(begin->type() == tk_enum::apointer_ && "[LOGIC ERROR][ParseCsoInt] begin is not apointer_ token.");
		tk_cursor it(begin, end);
		if (find_forward(*it, { tk_enum::apointer_, tk_enum::open_frame_ ,tk_enum::alnumus_,tk_enum::close_frame_})) {
			astnode node(astnode_enum::apointer_);
			node.push_back(it.next(2).to_statement());
			return make_success(node, *it.next(4));
		}
		else if (find_forward(*it, { tk_enum::apointer_, tk_enum::open_frame_ })) { // if contraint is not an alnumus it must be a cso type.
			astnode node(astnode_enum::apointer_);
			auto parse_result = ParseCandiSpecialObject()(*it.next(2), it.end());

			if (parse_result.valid()) {
				node.push_back(parse_result.node());
				// Skip the close frame.
				return make_success(node, parse_result.it()+1);
			}
			else {
				throw std::runtime_error("ParseCsoPointer: Invalid CSO Type in &pointer type constraint.");
			}
		}
		else {
			throw std::runtime_error("ParseCsoPointer: Pointer must be contrained to a type.");
		}
	}
	caoco_PARSING_PROCESS_IMPL(ParseCsoArray) {
		assert(begin->type() == tk_enum::aarray_ && "[LOGIC ERROR][ParseCsoArray] begin is not aarray_ token.");
		tk_cursor it(begin, end);
		if (find_forward(*it, { tk_enum::aarray_,tk_enum::open_frame_,tk_enum::alnumus_ })) {
			astnode node(astnode_enum::aarray_);
			node.push_back(it.next(2).to_statement());

			// Should be followed by a comma and a number literal.
			if (find_forward(*it.next(3), { tk_enum::comma_,tk_enum::number_literal_ })) {
				node.push_back(it.next(4).to_statement());
				// Skip the close frame.
				return make_success(node, *it.next(6));
			}
			else {
				throw std::runtime_error("ParseCsoArray: aarray_ must have a value constraint. Invalid array size.");
			}
		}
		else if (find_forward(*it, { tk_enum::aarray_,tk_enum::open_frame_ })) {
			// if constraint is not an alnumus it must be a cso type.
			astnode node(astnode_enum::aarray_);
			auto parse_result = ParseCandiSpecialObject()(*it.next(2), it.end());

			if (parse_result.valid()) {
				node.push_back(parse_result.node());
			
				// Should be followed by a comma and a number literal.
				if (find_forward(parse_result.it(), {tk_enum::comma_,tk_enum::number_literal_})) {
					//skip comma
					node.push_back(tk_cursor(parse_result.it()+1,end).to_statement());
					// Skip the close frame.
					return make_success(node, parse_result.it() + 3);
				}
				else {
					throw std::runtime_error("ParseCsoArray: aarray_ must have a value constraint. Invalid array size.");
				}
			}
			else {
				throw std::runtime_error("ParseCsoArray: Invalid CSO Type in &array type constraint.");
			}
		}
		else {
			throw std::runtime_error("ParseCsoArray: aarray_ must be contrained to a type.");
		}
	}
	caoco_PARSING_PROCESS_IMPL(ParseCandiSpecialObject) {
		switch (begin->type())
		{
		case tk_enum::atype_:
			return ParseCsoType()(begin, end);
			break;
		case tk_enum::avalue_:
			return ParseCsoValue()(begin, end);
			break;
		case tk_enum::aidentity_:
			return ParseCsoIdentity()(begin, end);
			break;
		case tk_enum::aint_:
			return ParseCsoInt()(begin, end);
			break;
		case tk_enum::auint_:
			return ParseCsoUint()(begin, end);
			break;
		case tk_enum::areal_:
			return ParseCsoReal()(begin, end);
			break;
		case tk_enum::aoctet_:
			return ParseCsoOctet()(begin, end);
			break;
		case tk_enum::abit_:
			return ParseCsoBit()(begin, end);
			break;
		case tk_enum::apointer_:
			return ParseCsoPointer()(begin, end);
			break;
		default:
			return make_error(begin, *begin, "ParseCandiSpecialObject: Invalid CSO.");
			break;
		}
	}

	caoco_PARSING_PROCESS_IMPL(ParseStringLiteral) {
		assert(begin->type() == tk_enum::string_literal_ && "[LOGIC ERROR][ParseStringLiteral] begin is not string_literal token.");
		return make_success({ astnode_enum::string_literal_, begin, std::next(begin) });
	}
	caoco_PARSING_PROCESS_IMPL(ParseNumberLiteral) {
		assert(begin->type() == tk_enum::number_literal_ && "[LOGIC ERROR][ParseNumberLiteral] begin is not number_literal token.");
		return make_success({ astnode_enum::number_literal_, begin, std::next(begin) });
	}
	caoco_PARSING_PROCESS_IMPL(ParseRealLiteral) {
		assert(begin->type() == tk_enum::real_literal_ && "[LOGIC ERROR][ParseRealLiteral] begin is not real_literal token.");
		return make_success({ astnode_enum::real_literal_, begin, std::next(begin) });
	}
	caoco_PARSING_PROCESS_IMPL(ParseAlnumusLiteral) {
		assert(begin->type() == tk_enum::alnumus_ && "[LOGIC ERROR][ParseAlnumusLiteral] begin is not alnumus token.");
		return make_success({ astnode_enum::alnumus_, begin, std::next(begin) });
	}

	caoco_PARSING_PROCESS_IMPL(ParseUnsignedLiteral){
		assert(begin->type() == tk_enum::unsigned_literal_ && "[LOGIC ERROR][ParseUnsignedLiteral] begin is not unsigned_literal token.");
		return make_success({ astnode_enum::unsigned_literal_, begin, std::next(begin) });
	}
	caoco_PARSING_PROCESS_IMPL(ParseOctetLiteral) {
		assert(begin->type() == tk_enum::octet_literal_ && "[LOGIC ERROR][ParseOctetLiteral] begin is not octet_literal token.");
		return make_success({ astnode_enum::octet_literal_, begin, std::next(begin) });
	}
	caoco_PARSING_PROCESS_IMPL(ParseBitLiteral) {
		assert(begin->type() == tk_enum::bit_literal_ && "[LOGIC ERROR][ParseBitLiteral] begin is not bit_literal token.");
		return make_success({ astnode_enum::bit_literal_, begin, std::next(begin) });
	}

	caoco_PARSING_PROCESS_IMPL(ParseLiteral) {
		switch (begin->type())
		{
		case tk_enum::string_literal_:
			return ParseStringLiteral()(begin, end);
			break;
		case tk_enum::number_literal_:
			return ParseNumberLiteral()(begin, end);
			break;
		case tk_enum::real_literal_:
			return ParseRealLiteral()(begin, end);
			break;
		case tk_enum::alnumus_:
			return ParseAlnumusLiteral()(begin, end);
			break;
		case tk_enum::none_literal_:
			return ParseDirectiveNone()(begin,end);
			break;
		case tk_enum::unsigned_literal_:
			return ParseUnsignedLiteral()(begin, end);
			break;
		case tk_enum::octet_literal_:
			return ParseOctetLiteral()(begin, end);
			break;
		case tk_enum::bit_literal_:
			return ParseBitLiteral()(begin, end);
			break;
		default:
			return make_error(begin, *begin, "ParseLiteral: Invalid literal.");
			break;
		}
	}

	caoco_PARSING_PROCESS_IMPL(ParseDirectiveType) {
		tk_cursor cursor(begin, end);
		// #type <alnumus> = <alnumus> <eos>
		if (begin->type() != tk_enum::type_) {
			return make_error(begin, *begin, "ParseDirectiveType: Expected a type directive.");
		}

		cursor.advance();

		if (!cursor.type_is(tk_enum::alnumus_)) {
			return make_error(*cursor, **cursor, "ParseIdentifierStatement: Expected an alnumus.");
		}

		// For now the only format of identifier statement is a 
		// 1. variable assingment: <alnumus> <assingment> <literal|alnumus> <eos>

		// next should be an assignment token
		if (cursor.next().type() != tk_enum::simple_assignment_) {
			return make_error(*cursor.next(), **cursor.next(), "ParseIdentifierStatement: Expected a simple assignment token.");
		}


		// next is a type expression which is an alnumus or a cso type.
		if (cursor.next(2).type_is(tk_enum::alnumus_)) {
			// type expr is an alnumus
			auto alnumus_literal = ParseAlnumusLiteral()(*cursor.next(2), end);
			if (!alnumus_literal.valid()) {
				return make_error(*cursor.next(2), **cursor.next(2), "ParseIdentifierStatement: Expected an alnumus.");
			}

			// next should be an eos
			if (cursor.next(3).type() != tk_enum::eos_) {
				return make_error(*cursor.next(3), **cursor.next(3), "ParseIdentifierStatement: Expected an eos.");
			}

			astnode node{ astnode_enum::type_definition_, begin, *cursor.next(3) };
			node.push_back(astnode{ astnode_enum::alnumus_, *cursor, *cursor.next() });
			node.push_back(astnode{ astnode_enum::simple_assignment_, *cursor.next(),*cursor.next(2) });
			node.push_back(astnode{ astnode_enum::aint_,*cursor.next(2),alnumus_literal.it() });
			// skip eos
			return make_success(node, alnumus_literal.it() + 2);
		}
		else { // Try to parse a cso
			auto cso = ParseCandiSpecialObject()(*cursor.next(2), end);

			if (!cso.valid()) {
				// Could not parse a cso, not a valid type expr
				return make_error(*cursor.next(2), **cursor.next(2), "ParseIdentifierStatement: Expected a type expression.");
			}

			// next should be an eos
			if (cso.it()->type() != tk_enum::eos_) {
				return make_error(*cursor.next(3), **cursor.next(3), "ParseIdentifierStatement: Expected an eos.");
			}

			astnode node{ astnode_enum::type_definition_, begin, cso.it() + 1 };
			node.push_back(astnode{ astnode_enum::alnumus_, *cursor, *cursor.next() });
			node.push_back(astnode{ astnode_enum::simple_assignment_, *cursor.next(),*cursor.next(2) });
			node.push_back(cso.node());
			// skip eos
			return make_success(node, cso.it() + 2);
		}


		// Code should never reach here.
		assert(false && "[LOGIC ERROR][ParseDirectiveType] Code should never reach here.");
		// Create the node, omit the eos token.
		//return make_success(node, std::next(it, 4));

	}
	caoco_PARSING_PROCESS_IMPL(ParseDirectiveVar) {
		tk_cursor cursor(begin, end);

		if (find_forward(*cursor, { tk_enum::var_,tk_enum::alnumus_,tk_enum::eos_ })) {	// Anon Var Decl
			astnode node{ astnode_enum::anon_variable_definition_, begin, *cursor.next(3) };
			node.push_back({ astnode_enum::alnumus_,*cursor.next(), *cursor.next(2) });
			return make_success(node, *cursor.next(3));// 1 past eos token
		}
		else if (find_forward(*cursor, { tk_enum::var_,tk_enum::alnumus_,tk_enum::simple_assignment_ })) // Anon Var Decl Assign
		{
			// Build the assingment expression
			auto expr = ParseValueExpression()(*cursor.next(), end);
			if (!expr.valid()) {
				return make_error(*cursor.next(), **cursor.next(3), "ParseDirectiveVar: Invalid var statement format. Assingment expression is invalid:" + expr.error_message());
			}
			// Create the node, omit the eos token.
			astnode node{ astnode_enum::anon_variable_definition_assingment_, *cursor, expr.it()-1 };
			node.push_back({ astnode_enum::alnumus_, *cursor.next(), *cursor.next(2) });
			//node.push_back({ astnode_enum::simple_assignment_, *cursor.next(2),*cursor.next(3) });


			node.push_back(expr.node());
			return make_success(node, expr.it()); // 1 past eos token
		}
		else if (find_forward(begin, { tk_enum::var_,tk_enum::open_frame_ })) { // Constrained Variable Def
			// Find the scope of the frame.
			parser_scope_result frame_scope = find_statement(tk_enum::open_frame_, tk_enum::close_frame_, *cursor.next(), cursor.end());

			// After the frame scope must be an alnumus.
			if (find_forward(frame_scope.scope_end(), { tk_enum::alnumus_ })) {

				if ((frame_scope.scope_end() + 1)->type_is(tk_enum::eos_)) {
					// Create the node, omit the eos token.
					astnode node{ astnode_enum::constrained_variable_definition_, *cursor, frame_scope.scope_end() + 1 };
					node.push_back({ astnode_enum::type_constraints_, frame_scope.contained_begin(), frame_scope.contained_end() });
					node.push_back({ astnode_enum::alnumus_, frame_scope.scope_end(), frame_scope.scope_end() + 1 });
					return make_success(node, frame_scope.scope_end() + 2); // 1 past eos token
				}
				else if ((frame_scope.scope_end() + 1)->type_is(tk_enum::simple_assignment_)) {
					// Build the assingment expression
					auto expr = ParseValueExpression()(frame_scope.scope_end() + 2, end);
					if (!expr.valid()) {
						return make_error(*cursor.next(3), **cursor.next(3), "ParseDirectiveVar: Invalid var statement format.");
					}

					// Create the node, omit the eos token.
					astnode node{ astnode_enum::constrained_variable_definition_, *cursor, expr.it()-1};
					node.push_back({ astnode_enum::type_constraints_, frame_scope.contained_begin(), frame_scope.contained_end() });
					node.push_back({ astnode_enum::alnumus_, frame_scope.scope_end(), frame_scope.scope_end() + 1 });
					node.push_back({ astnode_enum::simple_assignment_, (frame_scope.scope_end() + 1),(frame_scope.scope_end() + 2) });
					node.push_back(expr.node());
					return make_success(node, expr.it()); // 1 past eos token
				}
				else {
					return make_error(frame_scope.scope_end(), *frame_scope.scope_end(), "ParseDirectiveVar: Invalid var statement format.");
				}
			}
			else {
				return make_error(frame_scope.scope_end(), *frame_scope.scope_end(), "ParseDirectiveVar: Invalid var statement format.");
			}
		}
		else {
			return make_error(begin, *(begin+1), "ParseDirectiveVar: Invalid var statement format. #var directive was not followed by an identity or type constraint.");
		}
	}
	caoco_PARSING_PROCESS_IMPL(ParseDirectiveFunc) {
		tk_cursor cursor(begin, end);

		// Shorthand Void Arg Method Definition <#func> <alnumus> <functional_block>
		if (find_forward(*cursor, { tk_enum::func_,tk_enum::alnumus_,tk_enum::open_list_ })) {	// Shorthand Void Arg Method (#func name {} )same as #func [@] name () {}
			parser_scope_result method_def_body_scope = find_statement(tk_enum::open_list_, tk_enum::close_list_, *cursor.next(2), cursor.end());

			// After list scope must be an eos.
			if (method_def_body_scope.scope_end()->type_is(tk_enum::eos_)) {
				// Create the node, omit the eos token.
				astnode node{ astnode_enum::shorthand_void_method_definition_, *cursor, method_def_body_scope.scope_end() + 2 };
				node.push_back({ astnode_enum::alnumus_, *cursor.next(), *cursor.next(2) });
				node.push_back(ParseFunctionalBlock()(method_def_body_scope.contained_begin(), method_def_body_scope.contained_end()).node());
				return make_success(node, method_def_body_scope.scope_end() + 1); // 1 past eos token
			}
			else {
				return make_error(method_def_body_scope.scope_end(), *method_def_body_scope.scope_end(),
					"[ParseDirectiveFunc]: Invalid func statement format. Expected an eos token ';' after functional block in function definition. Function name:"
					+ sl::to_str(cursor.next().lit()));
			}
		}
		// Unconstrained Method Definition <#func> <alnumus> <arguments> <functional_block>
		else if (find_forward(*cursor, { tk_enum::func_,tk_enum::alnumus_,tk_enum::open_scope_ })) {
			parser_scope_result method_arguments_scope = find_scope(*cursor.next(2), cursor.end());

			if (method_arguments_scope.scope_end()->type_is(tk_enum::open_list_)) {
				parser_scope_result method_def_body_scope = find_statement(tk_enum::open_list_, tk_enum::close_list_, method_arguments_scope.scope_end(), cursor.end());
				// After list scope must be an eos.
				if (method_def_body_scope.scope_end()->type_is(tk_enum::eos_)) {
					// Create the node, omit the eos token.
					astnode node{ astnode_enum::method_definition_, *cursor, method_def_body_scope.scope_end() + 2 };
					node.push_back({ astnode_enum::alnumus_, *cursor.next(), *cursor.next(2) });
					node.push_back({ astnode_enum::arguments_, method_arguments_scope.contained_begin(), method_arguments_scope.contained_end() });
					node.push_back(ParseFunctionalBlock()(method_def_body_scope.contained_begin(), method_def_body_scope.contained_end()).node());
					return make_success(node, method_def_body_scope.scope_end() + 1); // 1 past eos token
				}
				else {
					return make_error(method_def_body_scope.scope_end(), *method_def_body_scope.scope_end(), "ParseDirectiveFunc: Invalid func statement format.");
				}

			}
			else {
				return make_error(method_arguments_scope.scope_end(), *method_arguments_scope.scope_end(), "ParseDirectiveFunc: Invalid func statement format.");
			}

		}
		// Shorthand Void Constrained Method Def or Constrained Method Def
		else if (find_forward(begin, { tk_enum::func_,tk_enum::open_frame_ })) {
			// Find the scope of the frame.
			parser_scope_result frame_scope = find_statement(tk_enum::open_frame_, tk_enum::close_frame_, *cursor.next(), cursor.end());

			// After the frame scope must be an alnumus and an open list -> Shorthand Void Constrained Function Def
			if (find_forward(frame_scope.scope_end(), { tk_enum::alnumus_,tk_enum::open_list_ })) {
				parser_scope_result method_def_body_scope = find_statement(tk_enum::open_list_, tk_enum::close_list_, frame_scope.scope_end() + 1, cursor.end());
				// After list scope must be an eos.
				if (method_def_body_scope.scope_end()->type_is(tk_enum::eos_)) {
					// Create the node, omit the eos token.
					astnode node{ astnode_enum::shorthand_constrained_void_method_definition_, *cursor, method_def_body_scope.scope_end() + 2 };
					node.push_back({ astnode_enum::type_constraints_, frame_scope.contained_begin(), frame_scope.contained_end() });
					node.push_back({ astnode_enum::alnumus_, frame_scope.scope_end(), frame_scope.scope_end() + 1 });
					node.push_back({ astnode_enum::functional_block_, method_def_body_scope.contained_begin(), method_def_body_scope.contained_end() });
					return make_success(node, method_def_body_scope.scope_end() + 1); // 1 past eos token
				}
				else {
					return make_error(method_def_body_scope.scope_end(), *method_def_body_scope.scope_end(), "ParseDirectiveFunc: Invalid func statement format.");
				}
			}
			// Or an alnumus and an open scope -> Constrained Method Def
			else if (find_forward(frame_scope.scope_end(), { tk_enum::alnumus_,tk_enum::open_scope_ })) {
				parser_scope_result method_arguments_scope = find_scope(frame_scope.scope_end() + 1, cursor.end());

				if (method_arguments_scope.scope_end()->type_is(tk_enum::open_list_)) {
					parser_scope_result method_def_body_scope = find_statement(tk_enum::open_list_, tk_enum::close_list_, method_arguments_scope.scope_end(), cursor.end());
					// After list scope must be an eos.
					if (method_def_body_scope.scope_end()->type_is(tk_enum::eos_)) {
						// Create the node, omit the eos token.
						astnode node{ astnode_enum::method_definition_, *cursor, method_def_body_scope.scope_end() + 2 };
						node.push_back({ astnode_enum::type_constraints_, frame_scope.contained_begin(), frame_scope.contained_end() });
						node.push_back({ astnode_enum::alnumus_, frame_scope.scope_end(), frame_scope.scope_end() + 1 });
						node.push_back({ astnode_enum::arguments_, method_arguments_scope.contained_begin(), method_arguments_scope.contained_end() });
						node.push_back({ astnode_enum::functional_block_, method_def_body_scope.contained_begin(), method_def_body_scope.contained_end() });
						return make_success(node, method_def_body_scope.scope_end() + 1); // 1 past eos token
					}
					else {
						return make_error(method_def_body_scope.scope_end(), *method_def_body_scope.scope_end(), "ParseDirectiveFunc: Invalid func statement format.");
					}

				}
				else {
					return make_error(method_arguments_scope.scope_end(), *method_arguments_scope.scope_end(), "ParseDirectiveFunc: Invalid func statement format.");
				}
			}
			else {
				return make_error(frame_scope.scope_end(), *frame_scope.scope_end(), "ParseDirectiveVar: Invalid var statement format.");
			}
		}
		else {
			return make_error(begin, *begin, "ParseDirectiveFunc: Invalid func statement format.");
		}
	};
	caoco_PARSING_PROCESS_IMPL(ParseDirectiveClass) {
		if (begin->type() != tk_enum::class_) {
			return make_error(begin, *begin, "ParseDirectiveClass: Expected a class directive.");
		}

		auto it = begin + 1;

		// next should be an alnumus, the class name.
		if (it->type() != tk_enum::alnumus_) {
			return make_error(it, *it, "ParseDirectiveClass: Expected an alnumus.");
		}

		// next should be an open list scope.
		if (std::next(it)->type() != tk_enum::open_list_) {
			return make_error(std::next(it), *std::next(it), "ParseDirectiveClass: Expected an open list scope.");
		}

		// Everything within the list is a pragmatic block.
		parser_scope_result class_scope = find_list(std::next(it), end);
		auto class_definition = ParsePragmaticBlock()(class_scope.contained_begin(), class_scope.contained_end());
		if (!class_definition.valid()) {
			return make_error(class_definition.it(), *class_definition.it(), "ParseDirectiveClass: Invalid class definition.");
		}

		// next should be an eos
		if (class_scope.scope_end()->type() != tk_enum::eos_) {
			return make_error(class_definition.it(), *class_definition.it(), "ParseDirectiveClass: Expected an eos.");
		}

		astnode node{ astnode_enum::class_definition_, begin, class_scope.scope_end() };
		node.push_back(astnode{ astnode_enum::alnumus_, it, std::next(it) });
		node.push_back(class_definition.node());
		return make_success(node, std::next(class_scope.scope_end(), 1));
	}
	caoco_PARSING_PROCESS_IMPL(ParseDirectiveNone) {
		assert(begin->type() == tk_enum::none_literal_ && "[LOGIC ERROR][ParseCsoValue] begin is not avalue_ token.");
		return make_success({ astnode_enum::none_literal_, begin, std::next(begin) });
	}

	caoco_PARSING_PROCESS_IMPL(ParseDirectiveReturn) {
		tk_cursor cursor(begin, end);
		parser_scope_result statement_scope = find_statement(tk_enum::return_, tk_enum::eos_, begin, end);

		if (!statement_scope.valid) {
			return make_error(statement_scope.scope_end(), *statement_scope.scope_end(), "ParseDirectiveReturn: Invalid return statement.");
		}
		astnode node{ astnode_enum::return_, begin, statement_scope.contained_end() };
		auto& expr = node.push_back({ astnode_enum::expression_, statement_scope.contained_begin(), statement_scope.contained_end() });
		expr.push_back(build_statement(statement_scope.contained_begin(), statement_scope.contained_end()).value());
		return make_success(node, statement_scope.scope_end());
	}
	caoco_PARSING_PROCESS_IMPL(ParseValueExpression) {
		auto expr_scope = caoco::find_open_statement(begin->type(), caoco::tk_enum::eos_, begin, end);

		sl_opt<astnode> expr_node;
		try {
			expr_node = build_statement(begin, expr_scope.contained_end());
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			return make_error(begin, *begin, "ParseValueExpression: Invalid statement.");
		}

		if(expr_node.has_value())
			return make_success(expr_node.value(), expr_scope.scope_end());
		else
			return make_error(begin, *begin, "ParseValueExpression: Invalid statement.");
	}

	caoco_PARSING_PROCESS_IMPL(ParsePragmaticBlock) {
		// Pragmatic blocks may contain statements starting with a directive or alnumus, ending in a semicolon.
		// <pragmatic_block> ::= (<directive>|<alnumus>) <statement> <eos> ?
		// <statement> ::= <type> | <var> | <func> | <class> | <identifier_statement>
		auto it = begin;
		astnode node(astnode_enum::pragmatic_block_, begin, end);

		// Find and parse all statements in the block.
		while (it < end && it->type() != tk_enum::eof_) {
			// Get the scope of the statement stating from the first token to the last matching semicolon.
			parser_scope_result statement_scope;

			auto parse_statement = [&it,&statement_scope,&end,&node](auto parsing_process, tk_enum open, tk_enum close)->void {
				statement_scope = find_statement(open, close, it, end);
				// If the statement is empty, skip it.
				if (statement_scope.is_empty()) {
					std::cout << "WARNING ParsePragmaticBlock: Empty statement." << std::endl;
				}
				if (!statement_scope.valid) {
					throw std::runtime_error("ParsePragmaticBlock: Invalid statement scope.");
				}
				parsing_result parse_result = parsing_process(statement_scope.scope_begin(), statement_scope.scope_end());
				if (!parse_result.valid()) {
					throw std::runtime_error("ParsePragmaticBlock: Invalid statement."+parse_result.error_message());
				}
				node.push_back(parse_result.node());
				it = parse_result.it();
			};
			auto parse_open_statement = [&it, &statement_scope, &end, &node](auto parsing_process, tk_enum open, tk_enum close)->void {
				statement_scope = find_open_statement(open, close, it, end);
				// If the statement is empty, skip it.
				if (statement_scope.is_empty()) {
					std::cout << "WARNING ParsePragmaticBlock: Empty statement." << std::endl;
				}
				if (!statement_scope.valid) {
					throw std::runtime_error("ParsePragmaticBlock: Invalid statement scope.");
				}
				parsing_result parse_result = parsing_process(statement_scope.scope_begin(), statement_scope.scope_end());
				if (!parse_result.valid()) {
					throw std::runtime_error("ParsePragmaticBlock: Invalid statement.");
				}
				node.push_back(parse_result.node());
				it = parse_result.it();
			};

			if (it->type_is(tk_enum::alnumus_)) {
				parse_open_statement(ParseIdentifierStatement(), tk_enum::alnumus_, tk_enum::eos_);
			}
			else {
				if (it->type() == tk_enum::type_) {
					parse_statement(ParseDirectiveType(), tk_enum::type_, tk_enum::eos_);
				}
				else if (it->type() == tk_enum::var_) {
					parse_statement(ParseDirectiveVar(), tk_enum::var_, tk_enum::eos_);
				}
				else if (it->type() == tk_enum::class_) {
					parse_statement(ParseDirectiveClass(), tk_enum::class_, tk_enum::eos_);
				}
				else if (it->type() == tk_enum::func_) {
					parse_statement(ParseDirectiveFunc(), tk_enum::func_, tk_enum::eos_);
				}
				else {
					return make_error(it, *it, "ParsePragmaticBlock: Invalid statement.");
				}
			}
		}

		return make_success(node, it);
	} // end ParsePragmaticBlock
	caoco_PARSING_PROCESS_IMPL(ParseFunctionalBlock) {
		auto it = begin;
		astnode node(astnode_enum::functional_block_, begin, end);

		// Find and parse all statements in the block.
		while (it < end && it->type() != tk_enum::eof_) {
			// Get the scope of the statement stating from the first token to the last matching semicolon.
			parser_scope_result statement_scope;

			auto parse_statement = [&it, &statement_scope, &end, &node](auto parsing_process, tk_enum open, tk_enum close)->void {
				statement_scope = find_statement(open, close, it, end);
				// If the statement is empty, skip it.
				if (statement_scope.is_empty()) {
					std::cout << "WARNING ParsePragmaticBlock: Empty statement." << std::endl;
				}
				if (!statement_scope.valid) {
					throw std::runtime_error("ParsePragmaticBlock: Invalid statement scope.");
				}
				parsing_result parse_result = parsing_process(statement_scope.scope_begin(), statement_scope.scope_end());
				if (!parse_result.valid()) {
					throw std::runtime_error("ParsePragmaticBlock: Invalid statement." + parse_result.error_message());
				}
				node.push_back(parse_result.node());
				it = parse_result.it();
			};
			auto parse_open_statement = [&it, &statement_scope, &end, &node](auto parsing_process, tk_enum open, tk_enum close)->void {
				statement_scope = find_open_statement(open, close, it, end);
				// If the statement is empty, skip it.
				if (statement_scope.is_empty()) {
					std::cout << "WARNING ParsePragmaticBlock: Empty statement." << std::endl;
				}
				if (!statement_scope.valid) {
					throw std::runtime_error("ParsePragmaticBlock: Invalid statement scope.");
				}
				parsing_result parse_result = parsing_process(statement_scope.scope_begin(), statement_scope.scope_end());
				if (!parse_result.valid()) {
					throw std::runtime_error("ParsePragmaticBlock: Invalid statement.");
				}
				node.push_back(parse_result.node());
				it = parse_result.it();
			};

			if (it->type_is(tk_enum::alnumus_)) {
				parse_open_statement(ParseIdentifierStatement(), tk_enum::alnumus_, tk_enum::eos_);
			}
			else {
				if (it->type() == tk_enum::return_) {
					parse_statement(ParseDirectiveReturn(), tk_enum::return_, tk_enum::eos_);
				}
				else if (it->type() == tk_enum::var_) {
					parse_statement(ParseDirectiveVar(), tk_enum::var_, tk_enum::eos_);
				}
				else {
					return make_error(it, *it, "ParsePragmaticBlock: Invalid statement.");
				}
			}
		}

		return make_success(node, it);
	}

	// Unused ?
	caoco_PARSING_PROCESS_IMPL(ParseIdentifierStatement) {
		auto it = begin;
		
		if (it->type() != tk_enum::alnumus_) {
			return make_error(it, *it, "ParseIdentifierStatement: Expected an alnumus.");
		}

		// For now the only format of identifier statement is a 
		// 1. variable assingment: <alnumus> <assingment> <literal|alnumus> <eos>

		// next should be an assignment token
		if (std::next(it)->type() != tk_enum::simple_assignment_) {
			return make_error(std::next(it), *std::next(it), "ParseIdentifierStatement: Expected a simple assignment token.");
		}

		// next should be a alnumus
		// NOTE: temporary-> this should be a value_expression
		if (std::next(it, 2)->type() != tk_enum::number_literal_) {
			return make_error(std::next(it, 2), *std::next(it, 2), "ParseIdentifierStatement: Expected an alnumus.");
		}

		// next should be an eos
		if (std::next(it, 3)->type() != tk_enum::eos_) {
			return make_error(std::next(it, 3), *std::next(it, 3), "ParseIdentifierStatement: Expected an eos.");
		}

		astnode node{ astnode_enum::variable_assignment_, begin, std::next(it, 3) };
		node.push_back(astnode{ astnode_enum::alnumus_, it, std::next(it) });
		node.push_back(astnode{ astnode_enum::simple_assignment_, std::next(it),std::next(it, 2) });
		node.push_back(astnode{ astnode_enum::number_literal_, std::next(it, 2),std::next(it, 3) });

		// Create the node, omit the eos token.
		return make_success(node, std::next(it, 4));
	}
} // end namespace caoco