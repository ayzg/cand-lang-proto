#pragma once
#include <list>
#include <concepts>
#include <type_traits>
#include <string>
#include <limits>
#include <map>
#include <optional>
#include <functional>
#include "global_dependencies.hpp"
#include "char_traits.hpp"
#include "token.hpp"
namespace caoco {
	using tk_vector = sl_vector<tk>;
	using tk_vector_it = tk_vector::iterator;
	using tk_vector_cit = tk_vector::const_iterator;

	//using char_t = char_traits::char_t;

	class Node {
	public:
		enum class e_type : int {
			none_ = -1,					// A none statement is a statement that is not a statement.
			invalid_,				// An invalid statement is a statement that is not a valid statement.

			// Token Analogs (scopes , semicolon, comma are evaluated in this parse step)
			string_literal_,
			number_literal_,
			real_literal_,
			none_literal_,
			alnumus_,

			unsigned_literal_,
			octet_literal_,
			bit_literal_,

			// . operator
			period_,

			// Assignemnt operators
			simple_assignment_, // =
			addition_assignment_, // +=
			subtraction_assignment_, // -=
			multiplication_assignment_, // *=
			division_assignment_, // /=
			remainder_assignment_, // %=
			bitwise_and_assignment_, // &=
			bitwise_or_assignment_, // |=
			bitwise_xor_assignment_, // ^=
			left_shift_assignment_, // <<=
			right_shift_assignment_, // >>=	

			// Increment and decrement operators
			increment_, // ++
			decrement_, // --

			// Arithmetic operators
			addition_, // +
			subtraction_, // -
			multiplication_, // *
			division_, // /
			remainder_, // %
			bitwise_NOT_, // ~
			bitwise_AND_, // &
			bitwise_OR_, // |
			bitwise_XOR_, // ^
			bitwise_left_shift_, // <<
			bitwise_right_shift_, // >>

			// Logical
			negation_, // !
			logical_AND_, // &&
			logical_OR_, // ||

			// Comparison
			equal_, // ==
			not_equal_, // !=
			less_than_, // <
			greater_than_, // >
			less_than_or_equal_, // <=
			greater_than_or_equal_, // >=
			three_way_comparison_, // <=>

			// Special Tokens
			atype_, // &type
			aidentity_, // &identity
			avalue_, // &value
			aint_, // &int[RANGE[-inf-inf]]
			auint_, // &uint[RANGE[0...inf]]
			areal_, // &real[RANGE[-inf...inf]]
			//aureal_, // &ureal[RANGE[0...inf]]
			aoctet_, // &octet[RANGE[0...255]]
			abit_, // &bit[RANGE[0...1]]
			aarray_, // &array[T,Size] // T is a type
			apointer_, // &pointer[T] // T is a type
			amemory_, // &memory[T,Size] // T is a type
			afunction_, // &function 

			// Directive Tokens
			enter_, // #enter
			start_, // #start
			type_, // #type
			var_, // #var
			class_, // #class
			func_, // #func
			// Modifier Tokens
			const_, // #const
			static_, // #static
			ref_, // #ref

			// Control Flow Tokens
			if_, // #if
			else_, // #else
			elif_, // #elif
			while_, // #while
			for_, // #for
			switch_, // #switch
			case_, // #case
			default_, // #default
			break_, // #break
			continue_, // #continue
			return_, // #return
			into_, // #into

			// Compound Nodes
			// Basic.
			statement_,				// A statement is a single line of code.	
			expression_,			// An expression is a statement that returns a value.
			declaration_,			// A declaration is a statement that does not return a value.
			program_,				// A program is a collection of statements.
			compiled_program_,		// A compiled program is all the statements within the #enter block.
			interpreted_program_,   // An interpreted program is all statements within the #start block.
			pragmatic_block_,       // {} statements which may delare classes and functions. eg like in a C++ namespace or C++ class definition.
			functional_block_,      // {} statements which may only declare variables and expressions. eg like in a C++ function definition.
			arguments_,				// (a,b,c) where abc are expressions.
			type_constraints_,		// [a,b,c] where abc are types.
			// Declarations
			identifier_statement_, // ambiguous declaration starting with an identifier. myvarname = b; 

			function_call_,			// a(b,c,d);
			variable_assignment_,							// a = b;
			type_definition_,								// #type a = b;
			anon_variable_definition_,						// #var a;
			anon_variable_definition_assingment_,			// #var a = b;
			constrained_variable_definition_,				// #var a : int;
			method_definition_,								// #func a() {};
			constrained_method_definition_,					// #func [T] a() {};
			shorthand_void_method_definition_,				// #func a {};
			shorthand_constrained_void_method_definition_,	// #func [T] a {};
			class_definition_,								// #class a{};
			// EOF
			open_scope_, // (
			close_scope_, // )
			open_list_, // {
			close_list_, // }
			open_frame_, // [
			close_frame_, // ]
			eof_,
		};

		enum class eAssoc : int {
			left_ = 0,
			right_ = 1,
			both_ = 2,
			none_ = 3
		};
		enum class eOperation : int {
			binary_ = 0,
			unary_ = 1,
			variadic_ = 2,
			none_ = 3
		};
		enum eTraitIndex : sl_size {
			importance_ = 0,
			associativity_ = 1,
			operation_ = 2
		};
	public:
		using traits_map = sl_map<e_type, sl_tuple<int, eAssoc, eOperation>>;
		static const traits_map statement_traits;

	private:
		// Node Properties
		e_type type_;
		tk_vector_cit beg_;			// Beginning of the statement.
		tk_vector_cit end_;			// One token past the end of the statement.
		// Node Relations
		Node* parent_{ nullptr };
		sl_list<Node> body_;
	public:
		Node() : type_(e_type::eof_){}
		Node(e_type type) : type_(type){}
		Node(e_type type, tk_vector_cit beg, tk_vector_cit end) : type_(type), beg_(beg), end_(end) {}
		Node(e_type type, tk_vector_cit beg, tk_vector_cit end, Node& parent) : type_(type), beg_(beg), end_(end), parent_(&parent) {}
		
		e_type type() const { return type_; }
		const sl_list<Node>& body() const { return body_; }
		Node& front() { return body_.front(); }
		Node& back() { return body_.back(); }
		bool empty() const { return body_.empty(); }
		Node& push_back(Node stmt) { body_.push_back(stmt); return body_.back(); }
		Node& push_front(Node stmt) { body_.push_front(stmt); return body_.front(); }

		Node& insert(Node statement, sl_list<Node>::iterator it) {
			return *body_.insert(it, statement);
		}

		// Changes type and tokens of node in place, maintaining body and parent.
		constexpr Node& morph(e_type type, tk_vector_cit beg, tk_vector_cit end) {
			type_ = type;
			beg_ = beg; end_ = end;
			return *this;
		}
		constexpr Node& morph(e_type type) {
			type_ = type;
			return *this;
		}

		void change_parent(Node& new_parent) {
			parent_ = &new_parent;
		}
		void change_body(sl_list<Node> new_body) {
			body_ = new_body;
		}

		// Tokens
		constexpr auto has_tokens() { return not (beg_ == end_); }
		constexpr auto token_begin() { return beg_; }
		constexpr auto token_end() { return end_; }

		// Get the tokens that make up the statement in the source code.
		constexpr tk_vector get_tokens() const {
			return tk_vector(beg_, end_);
		}

		// Get the a vector of literals of the tokens that make up this statement.
		constexpr std::vector<sl_u8string> get_literals() {
			sl_vector<sl_u8string> literals;
			for (auto& token : get_tokens()) {
				literals.push_back(token.literal());
			}
			return literals;
		}

		// Get a string representation of the statement. NOTE: returns a char8_t string.
		constexpr sl_u8string to_string() const  {
			sl_u8string statement_string = u8"";
			for (auto& token : get_tokens()) {
				statement_string += token.literal();
			}
			return statement_string;
		}

		// Statement Properties
		int importance() { return sl::get<eTraitIndex::importance_>(statement_traits.at(type_)); }
		eAssoc assoc() { return sl::get<eTraitIndex::associativity_>(statement_traits.at(type_)); }
		eOperation operation() { return sl::get<eTraitIndex::operation_>(statement_traits.at(type_)); }
	
		// Relational Properties
		// Find the nearest ancestor of the specified type. searching up the tree. If no ancestor is found, returns nullopt.
		sl_opt<sl_ref<Node>> find_ancestor(e_type type) {
			if (parent_ == nullptr) return std::nullopt;
			if (parent_->type() == type) return *parent_;
			return parent_->find_ancestor(type);
		}

		constexpr auto debug_string() const {
			std::string debug_string = "[";
			switch (type_) {
				case e_type::none_: debug_string += "none_"; break;
				case e_type::invalid_: debug_string += "invalid_"; break;
				case e_type::string_literal_: debug_string += "string_literal_"; break;
				case e_type::number_literal_: debug_string += "number_literal_"; break;
				case e_type::real_literal_: debug_string += "real_literal_"; break;
				case e_type::alnumus_: debug_string += "alnumus_"; break;
				case e_type::simple_assignment_: debug_string += "simple_assignment_"; break;
				case e_type::addition_assignment_: debug_string += "addition_assignment_"; break;
				case e_type::subtraction_assignment_: debug_string += "subtraction_assignment_"; break;
				case e_type::multiplication_assignment_: debug_string += "multiplication_assignment_"; break;
				case e_type::division_assignment_: debug_string += "division_assignment_"; break;
				case e_type::remainder_assignment_: debug_string += "remainder_assignment_"; break;
				case e_type::bitwise_and_assignment_: debug_string += "bitwise_and_assignment_"; break;
				case e_type::bitwise_or_assignment_: debug_string += "bitwise_or_assignment_"; break;
				case e_type::bitwise_xor_assignment_: debug_string += "bitwise_xor_assignment_"; break;
				case e_type::left_shift_assignment_: debug_string += "left_shift_assignment_"; break;
				case e_type::right_shift_assignment_: debug_string += "right_shift_assignment_"; break;
				case e_type::increment_: debug_string += "increment_"; break;
				case e_type::decrement_: debug_string += "decrement_"; break;
				case e_type::addition_: debug_string += "addition_"; break;
				case e_type::subtraction_: debug_string += "subtraction_"; break;
				case e_type::multiplication_: debug_string += "multiplication_"; break;
				case e_type::division_: debug_string += "division_"; break;
				case e_type::remainder_: debug_string += "remainder_"; break;
				case e_type::bitwise_NOT_: debug_string += "bitwise_NOT_"; break;
				case e_type::bitwise_AND_: debug_string += "bitwise_AND_"; break;
				case e_type::bitwise_OR_: debug_string += "bitwise_OR_"; break;
				case e_type::bitwise_XOR_: debug_string += "bitwise_XOR_"; break;
				case e_type::bitwise_left_shift_: debug_string += "bitwise_left_shift_"; break;
				case e_type::bitwise_right_shift_: debug_string += "bitwise_right_shift_"; break;
				case e_type::negation_: debug_string += "negation_"; break;
				case e_type::logical_AND_: debug_string += "logical_AND_"; break;
				case e_type::logical_OR_: debug_string += "logical_OR_"; break;
				case e_type::equal_: debug_string += "equal_"; break;
				case e_type::not_equal_: debug_string += "not_equal_"; break;
				case e_type::less_than_: debug_string += "less_than_"; break;
				case e_type::greater_than_: debug_string += "greater_than_"; break;
				case e_type::less_than_or_equal_: debug_string += "less_than_or_equal_"; break;
				case e_type::greater_than_or_equal_: debug_string += "greater_than_or_equal_"; break;
				case e_type::three_way_comparison_: debug_string += "three_way_comparison_"; break;
				case e_type::atype_: debug_string += "atype_"; break;
				case e_type::aidentity_: debug_string += "aidentity_"; break;
				case e_type::avalue_: debug_string += "avalue_"; break;
				case e_type::aint_: debug_string += "aint_"; break;
				case e_type::auint_: debug_string += "auint_"; break;
				case e_type::areal_: debug_string += "areal_"; break;
				//case e_type::aureal_: debug_string += "aureal_"; break;
				case e_type::aoctet_: debug_string += "aoctet_"; break;
				case e_type::abit_: debug_string += "abit_"; break;
				case e_type::aarray_: debug_string += "aarray_"; break;
				case e_type::apointer_: debug_string += "apointer_"; break;
				case e_type::amemory_: debug_string += "amemory_"; break;
				case e_type::afunction_: debug_string += "afunction_"; break;
				case e_type::enter_: debug_string += "enter_"; break;
				case e_type::start_: debug_string += "start_"; break;
				case e_type::type_: debug_string += "type_"; break;
				case e_type::var_: debug_string += "var_"; break;
				case e_type::class_: debug_string += "class_"; break;
				case e_type::func_: debug_string += "func_"; break;
				case e_type::const_: debug_string += "const_"; break;
				case e_type::static_: debug_string += "static_"; break;
				case e_type::ref_: debug_string += "ref_"; break;
				case e_type::if_: debug_string += "if_"; break;
				case e_type::else_: debug_string += "else_"; break;
				case e_type::elif_: debug_string += "elif_"; break;
				case e_type::while_: debug_string += "while_"; break;
				case e_type::for_: debug_string += "for_"; break;
				case e_type::switch_: debug_string += "switch_"; break;
				case e_type::case_: debug_string += "case_"; break;
				case e_type::default_: debug_string += "default_"; break;
				case e_type::break_: debug_string += "break_"; break;
				case e_type::continue_: debug_string += "continue_"; break;
				case e_type::return_: debug_string += "return_"; break;
				case e_type::into_: debug_string += "into_"; break;
				case e_type::statement_: debug_string += "statement_"; break;
				case e_type::expression_: debug_string += "expression_"; break;
				case e_type::declaration_: debug_string += "declaration_"; break;
				case e_type::program_: debug_string += "program_"; break;
				case e_type::compiled_program_: debug_string += "compiled_program_"; break;
				case e_type::interpreted_program_: debug_string += "interpreted_program_"; break;
				case e_type::pragmatic_block_: debug_string += "pragmatic_block_"; break;
				case e_type::functional_block_: debug_string += "functional_block_"; break;
				case e_type::identifier_statement_: debug_string += "identifier_statement_"; break;
				case e_type::variable_assignment_: debug_string += "variable_assignment_"; break;
				case e_type::type_definition_: debug_string += "type_definition_"; break;
				case e_type::anon_variable_definition_: debug_string += "anon_variable_definition_"; break;
				case e_type::anon_variable_definition_assingment_: debug_string += "anon_variable_definition_assingment_"; break;
				case e_type::constrained_variable_definition_: debug_string += "constrained_variable_definition_"; break;
				case e_type::type_constraints_: debug_string += "type_constraints_"; break;
				case e_type::class_definition_: debug_string += "class_definition_"; break;
				case e_type::shorthand_void_method_definition_: debug_string += "shorhand_void_method_definition_"; break;
				case e_type::shorthand_constrained_void_method_definition_: debug_string += "shorhand_void_method_definition_"; break;
				case e_type::arguments_: debug_string += "arguments_"; break;
				case e_type::function_call_: debug_string += "function_call_"; break;
				case e_type::method_definition_: debug_string += "method_definition_"; break;
				case e_type::eof_: debug_string += "eof_"; break;
				case e_type::period_: debug_string += "period_"; break;

			}

			debug_string += "] ";
			debug_string += caoco::sl::to_str(to_string());
			debug_string += "| ";
			return debug_string;
		}
	};
	using astnode_enum = Node::e_type;

	const Node::traits_map Node::statement_traits{
	{e_type::alnumus_, {INT_MAX, eAssoc::none_, eOperation::none_}},
	{e_type::number_literal_, {INT_MAX, eAssoc::none_, eOperation::none_}},
	{e_type::addition_, {1000, eAssoc::left_, eOperation::binary_}},
	{e_type::subtraction_, {1000, eAssoc::left_, eOperation::binary_}},
	{e_type::multiplication_, {2000, eAssoc::left_, eOperation::binary_}},
	{e_type::division_, {2000, eAssoc::left_, eOperation::binary_}},
	{e_type::remainder_, {2000, eAssoc::left_, eOperation::binary_}},
	{e_type::bitwise_NOT_, {3000, eAssoc::right_, eOperation::unary_}},
	{e_type::bitwise_AND_, {4000, eAssoc::left_, eOperation::binary_}},
	{e_type::bitwise_OR_, {5000, eAssoc::left_, eOperation::binary_}},
	{e_type::bitwise_XOR_, {6000, eAssoc::left_, eOperation::binary_}},
	{e_type::bitwise_left_shift_, {7000, eAssoc::left_, eOperation::binary_}},
	{e_type::bitwise_right_shift_, {7000, eAssoc::left_, eOperation::binary_}},
	{e_type::negation_, {8000, eAssoc::right_, eOperation::unary_}},
	{e_type::logical_AND_, {9000, eAssoc::left_, eOperation::binary_}},
	{e_type::logical_OR_, {10000, eAssoc::left_, eOperation::binary_}},
	{e_type::equal_, {11000, eAssoc::left_, eOperation::binary_}},
	{e_type::not_equal_, {11000, eAssoc::left_, eOperation::binary_}},
	{e_type::less_than_, {11000, eAssoc::left_, eOperation::binary_}},
	{e_type::greater_than_, {11000, eAssoc::left_, eOperation::binary_}},
	{e_type::less_than_or_equal_, {11000, eAssoc::left_, eOperation::binary_}},
	{e_type::greater_than_or_equal_, {11000, eAssoc::left_, eOperation::binary_}},
	{e_type::three_way_comparison_, {11000, eAssoc::left_, eOperation::binary_}},
	{e_type::simple_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{e_type::addition_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{e_type::subtraction_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{e_type::multiplication_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{e_type::division_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{e_type::remainder_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{e_type::bitwise_and_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{e_type::bitwise_or_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{e_type::bitwise_xor_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{e_type::left_shift_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{e_type::right_shift_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{e_type::increment_, {13000, eAssoc::right_, eOperation::unary_}},
	{e_type::decrement_, {13000, eAssoc::right_, eOperation::unary_}},
	{e_type::atype_, {14000, eAssoc::right_, eOperation::unary_}},
	{e_type::aidentity_, {14000, eAssoc::right_, eOperation::unary_}},
	{e_type::avalue_, {14000, eAssoc::right_, eOperation::unary_}},
	{e_type::aint_, {14000, eAssoc::right_, eOperation::unary_}},
	{e_type::auint_, {14000, eAssoc::right_, eOperation::unary_}},
	{e_type::areal_, {14000, eAssoc::right_, eOperation::unary_}},
	//{e_type::aureal_, {14000, eAssoc::right_, eOperation::unary_}},
	{e_type::aoctet_, {14000, eAssoc::right_, eOperation::unary_}},
	{e_type::abit_, {14000, eAssoc::right_, eOperation::unary_}},
	{e_type::aarray_, {14000, eAssoc::right_, eOperation::unary_}},
	{e_type::apointer_, {14000, eAssoc::right_, eOperation::unary_}},
	{e_type::amemory_, {14000, eAssoc::right_, eOperation::unary_}},
	{e_type::afunction_, {14000, eAssoc::right_, eOperation::unary_}},
	{e_type::enter_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::start_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::type_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::var_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::class_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::const_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::static_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::ref_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::if_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::else_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::elif_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::while_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::for_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::switch_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::case_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::default_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::break_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::continue_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::return_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::into_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::statement_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::expression_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::declaration_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::program_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::compiled_program_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::interpreted_program_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::pragmatic_block_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::functional_block_, {15000, eAssoc::none_, eOperation::none_}},
	{e_type::eof_, {INT_MIN, eAssoc::none_, eOperation::none_}}
	};

} // namespace caoco