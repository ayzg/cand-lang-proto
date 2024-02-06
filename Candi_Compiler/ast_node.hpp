#pragma once
#include <list>
#include <concepts>
#include <type_traits>
#include <string>
#include <limits>
#include <map>
#include <optional>
#include <functional>
#include "char_traits.hpp"
#include "token.hpp"
namespace caoco {
	using size_t = std::size_t;
	using char_t = char_traits::char_t;
	using string_t = std::basic_string<char_t>;

	class Node {
	public:
		enum class eType : int {
			none_ = -1,					// A none statement is a statement that is not a statement.
			invalid_,				// An invalid statement is a statement that is not a valid statement.

			// Token Analogs (scopes , semicolor, comma are evaluated in this parse step)
			string_literal_,
			number_literal_,
			real_literal_,
			none_literal_,
			alnumus_,

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
			aureal_, // &ureal[RANGE[0...inf]]
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
		enum eTraitIndex : size_t {
			importance_ = 0,
			associativity_ = 1,
			operation_ = 2
		};
	public:
		using traits_map_t = std::map<eType, std::tuple<int, eAssoc, eOperation>>;
		static const traits_map_t statement_traits;

	private:
		// Node Properties
		eType type_;
		std::vector<Tk>::const_iterator beg_;			// Beginning of the statement.
		std::vector<Tk>::const_iterator end_;			// One token past the end of the statement.
		// Node Relations

		Node* parent_{ nullptr };									// Parent statement.
		std::list<Node> body_;
	public:
		Node() : type_(eType::eof_){}
		Node(eType type) : type_(type){}
		Node(eType type, std::vector<Tk>::const_iterator beg, std::vector<Tk>::const_iterator end) : type_(type), beg_(beg), end_(end) {}
		Node(eType type, std::vector<Tk>::const_iterator beg, std::vector<Tk>::const_iterator end, Node& parent) : type_(type), beg_(beg), end_(end), parent_(&parent) {}
		
		eType type() const { return type_; }
		const std::list<Node>& body() const { return body_; }
		Node& front() { return body_.front(); }
		Node& back() { return body_.back(); }
		bool empty() const { return body_.empty(); }
		Node& push_back(Node stmt) { body_.push_back(stmt); return body_.back(); }
		Node& push_front(Node stmt) { body_.push_front(stmt); return body_.front(); }

		//  <@method:insert> Adds a statement to the body of the statement to the left of the specified iterator.
		//					 Returns an iterator to the newly inserted statement.
		// https://stackoverflow.com/questions/23038119/c-insertion-into-vector-after-or-before-iterator
		// https://en.cppreference.com/w/cpp/container/vector/insert
		Node& insert(Node statement, std::list<Node>::iterator it) {
			return *body_.insert(it, statement);
		}

		// Changes type and tokens of node in place, maintaining body and parent.
		constexpr Node& morph(eType type, std::vector<Tk>::const_iterator beg, std::vector<Tk>::const_iterator end) {
			type_ = type;
			beg_ = beg; end_ = end;
			return *this;
		}
		constexpr Node& morph(eType type) {
			type_ = type;
			return *this;
		}

		void change_parent(Node& new_parent) {
			parent_ = &new_parent;
		}
		void change_body(std::list<Node> new_body) {
			body_ = new_body;
		}

		// Tokens
		constexpr auto has_tokens() { return not (beg_ == end_); }
		constexpr auto token_begin() { return beg_; }
		constexpr auto token_end() { return end_; }

		// Get the tokens that make up the statement in the source code.
		constexpr std::vector<Tk> get_tokens() const {
			return std::vector<Tk>(beg_, end_);
		}

		// Get the a vector of literals of the tokens that make up this statement.
		constexpr std::vector<string_t> get_literals() {
			std::vector<string_t> literals;
			for (auto& token : get_tokens()) {
				literals.push_back(token.literal());
			}
			return literals;
		}

		// Get a string representation of the statement. NOTE: returns a char8_t string.
		constexpr string_t to_string() const  {
			string_t statement_string = u8"";
			for (auto& token : get_tokens()) {
				statement_string += token.literal();
			}
			return statement_string;
		}

		// Statement Properties
		int importance() { return std::get<eTraitIndex::importance_>(statement_traits.at(type_)); }
		eAssoc assoc() { return std::get<eTraitIndex::associativity_>(statement_traits.at(type_)); }
		eOperation operation() { return std::get<eTraitIndex::operation_>(statement_traits.at(type_)); }
	
		// Relational Properties
		// Find the nearest ancestor of the specified type. searching up the tree. If no ancestor is found, returns nullopt.
		std::optional<std::reference_wrapper<Node>> find_ancestor(eType type) {
			if (parent_ == nullptr) return std::nullopt;
			if (parent_->type() == type) return *parent_;
			return parent_->find_ancestor(type);
		}

		constexpr auto debug_string() const {
			std::string debug_string = "[";
			switch (type_) {
				case eType::none_: debug_string += "none_"; break;
				case eType::invalid_: debug_string += "invalid_"; break;
				case eType::string_literal_: debug_string += "string_literal_"; break;
				case eType::number_literal_: debug_string += "number_literal_"; break;
				case eType::real_literal_: debug_string += "real_literal_"; break;
				case eType::alnumus_: debug_string += "alnumus_"; break;
				case eType::simple_assignment_: debug_string += "simple_assignment_"; break;
				case eType::addition_assignment_: debug_string += "addition_assignment_"; break;
				case eType::subtraction_assignment_: debug_string += "subtraction_assignment_"; break;
				case eType::multiplication_assignment_: debug_string += "multiplication_assignment_"; break;
				case eType::division_assignment_: debug_string += "division_assignment_"; break;
				case eType::remainder_assignment_: debug_string += "remainder_assignment_"; break;
				case eType::bitwise_and_assignment_: debug_string += "bitwise_and_assignment_"; break;
				case eType::bitwise_or_assignment_: debug_string += "bitwise_or_assignment_"; break;
				case eType::bitwise_xor_assignment_: debug_string += "bitwise_xor_assignment_"; break;
				case eType::left_shift_assignment_: debug_string += "left_shift_assignment_"; break;
				case eType::right_shift_assignment_: debug_string += "right_shift_assignment_"; break;
				case eType::increment_: debug_string += "increment_"; break;
				case eType::decrement_: debug_string += "decrement_"; break;
				case eType::addition_: debug_string += "addition_"; break;
				case eType::subtraction_: debug_string += "subtraction_"; break;
				case eType::multiplication_: debug_string += "multiplication_"; break;
				case eType::division_: debug_string += "division_"; break;
				case eType::remainder_: debug_string += "remainder_"; break;
				case eType::bitwise_NOT_: debug_string += "bitwise_NOT_"; break;
				case eType::bitwise_AND_: debug_string += "bitwise_AND_"; break;
				case eType::bitwise_OR_: debug_string += "bitwise_OR_"; break;
				case eType::bitwise_XOR_: debug_string += "bitwise_XOR_"; break;
				case eType::bitwise_left_shift_: debug_string += "bitwise_left_shift_"; break;
				case eType::bitwise_right_shift_: debug_string += "bitwise_right_shift_"; break;
				case eType::negation_: debug_string += "negation_"; break;
				case eType::logical_AND_: debug_string += "logical_AND_"; break;
				case eType::logical_OR_: debug_string += "logical_OR_"; break;
				case eType::equal_: debug_string += "equal_"; break;
				case eType::not_equal_: debug_string += "not_equal_"; break;
				case eType::less_than_: debug_string += "less_than_"; break;
				case eType::greater_than_: debug_string += "greater_than_"; break;
				case eType::less_than_or_equal_: debug_string += "less_than_or_equal_"; break;
				case eType::greater_than_or_equal_: debug_string += "greater_than_or_equal_"; break;
				case eType::three_way_comparison_: debug_string += "three_way_comparison_"; break;
				case eType::atype_: debug_string += "atype_"; break;
				case eType::aidentity_: debug_string += "aidentity_"; break;
				case eType::avalue_: debug_string += "avalue_"; break;
				case eType::aint_: debug_string += "aint_"; break;
				case eType::auint_: debug_string += "auint_"; break;
				case eType::areal_: debug_string += "areal_"; break;
				case eType::aureal_: debug_string += "aureal_"; break;
				case eType::aoctet_: debug_string += "aoctet_"; break;
				case eType::abit_: debug_string += "abit_"; break;
				case eType::aarray_: debug_string += "aarray_"; break;
				case eType::apointer_: debug_string += "apointer_"; break;
				case eType::amemory_: debug_string += "amemory_"; break;
				case eType::afunction_: debug_string += "afunction_"; break;
				case eType::enter_: debug_string += "enter_"; break;
				case eType::start_: debug_string += "start_"; break;
				case eType::type_: debug_string += "type_"; break;
				case eType::var_: debug_string += "var_"; break;
				case eType::class_: debug_string += "class_"; break;
				case eType::func_: debug_string += "func_"; break;
				case eType::const_: debug_string += "const_"; break;
				case eType::static_: debug_string += "static_"; break;
				case eType::ref_: debug_string += "ref_"; break;
				case eType::if_: debug_string += "if_"; break;
				case eType::else_: debug_string += "else_"; break;
				case eType::elif_: debug_string += "elif_"; break;
				case eType::while_: debug_string += "while_"; break;
				case eType::for_: debug_string += "for_"; break;
				case eType::switch_: debug_string += "switch_"; break;
				case eType::case_: debug_string += "case_"; break;
				case eType::default_: debug_string += "default_"; break;
				case eType::break_: debug_string += "break_"; break;
				case eType::continue_: debug_string += "continue_"; break;
				case eType::return_: debug_string += "return_"; break;
				case eType::into_: debug_string += "into_"; break;
				case eType::statement_: debug_string += "statement_"; break;
				case eType::expression_: debug_string += "expression_"; break;
				case eType::declaration_: debug_string += "declaration_"; break;
				case eType::program_: debug_string += "program_"; break;
				case eType::compiled_program_: debug_string += "compiled_program_"; break;
				case eType::interpreted_program_: debug_string += "interpreted_program_"; break;
				case eType::pragmatic_block_: debug_string += "pragmatic_block_"; break;
				case eType::functional_block_: debug_string += "functional_block_"; break;
				case eType::identifier_statement_: debug_string += "identifier_statement_"; break;
				case eType::variable_assignment_: debug_string += "variable_assignment_"; break;
				case eType::type_definition_: debug_string += "type_definition_"; break;
				case eType::anon_variable_definition_: debug_string += "anon_variable_definition_"; break;
				case eType::anon_variable_definition_assingment_: debug_string += "anon_variable_definition_assingment_"; break;
				case eType::constrained_variable_definition_: debug_string += "constrained_variable_definition_"; break;
				case eType::type_constraints_: debug_string += "type_constraints_"; break;
				case eType::class_definition_: debug_string += "class_definition_"; break;
				case eType::shorthand_void_method_definition_: debug_string += "shorhand_void_method_definition_"; break;
				case eType::shorthand_constrained_void_method_definition_: debug_string += "shorhand_void_method_definition_"; break;
				case eType::arguments_: debug_string += "arguments_"; break;
				case eType::function_call_: debug_string += "function_call_"; break;
				case eType::method_definition_: debug_string += "method_definition_"; break;
				case eType::eof_: debug_string += "eof_"; break;
				case eType::period_: debug_string += "period_"; break;

			}

			debug_string += "] ";
			debug_string += caoco::to_std_string(to_string());
			debug_string += "| ";
			return debug_string;
		}
	};

	const Node::traits_map_t Node::statement_traits{
	{eType::alnumus_, {INT_MAX, eAssoc::none_, eOperation::none_}},
	{eType::number_literal_, {INT_MAX, eAssoc::none_, eOperation::none_}},
	{eType::addition_, {1000, eAssoc::left_, eOperation::binary_}},
	{eType::subtraction_, {1000, eAssoc::left_, eOperation::binary_}},
	{eType::multiplication_, {2000, eAssoc::left_, eOperation::binary_}},
	{eType::division_, {2000, eAssoc::left_, eOperation::binary_}},
	{eType::remainder_, {2000, eAssoc::left_, eOperation::binary_}},
	{eType::bitwise_NOT_, {3000, eAssoc::right_, eOperation::unary_}},
	{eType::bitwise_AND_, {4000, eAssoc::left_, eOperation::binary_}},
	{eType::bitwise_OR_, {5000, eAssoc::left_, eOperation::binary_}},
	{eType::bitwise_XOR_, {6000, eAssoc::left_, eOperation::binary_}},
	{eType::bitwise_left_shift_, {7000, eAssoc::left_, eOperation::binary_}},
	{eType::bitwise_right_shift_, {7000, eAssoc::left_, eOperation::binary_}},
	{eType::negation_, {8000, eAssoc::right_, eOperation::unary_}},
	{eType::logical_AND_, {9000, eAssoc::left_, eOperation::binary_}},
	{eType::logical_OR_, {10000, eAssoc::left_, eOperation::binary_}},
	{eType::equal_, {11000, eAssoc::left_, eOperation::binary_}},
	{eType::not_equal_, {11000, eAssoc::left_, eOperation::binary_}},
	{eType::less_than_, {11000, eAssoc::left_, eOperation::binary_}},
	{eType::greater_than_, {11000, eAssoc::left_, eOperation::binary_}},
	{eType::less_than_or_equal_, {11000, eAssoc::left_, eOperation::binary_}},
	{eType::greater_than_or_equal_, {11000, eAssoc::left_, eOperation::binary_}},
	{eType::three_way_comparison_, {11000, eAssoc::left_, eOperation::binary_}},
	{eType::simple_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{eType::addition_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{eType::subtraction_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{eType::multiplication_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{eType::division_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{eType::remainder_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{eType::bitwise_and_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{eType::bitwise_or_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{eType::bitwise_xor_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{eType::left_shift_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{eType::right_shift_assignment_, {12000, eAssoc::right_, eOperation::binary_}},
	{eType::increment_, {13000, eAssoc::right_, eOperation::unary_}},
	{eType::decrement_, {13000, eAssoc::right_, eOperation::unary_}},
	{eType::atype_, {14000, eAssoc::right_, eOperation::unary_}},
	{eType::aidentity_, {14000, eAssoc::right_, eOperation::unary_}},
	{eType::avalue_, {14000, eAssoc::right_, eOperation::unary_}},
	{eType::aint_, {14000, eAssoc::right_, eOperation::unary_}},
	{eType::auint_, {14000, eAssoc::right_, eOperation::unary_}},
	{eType::areal_, {14000, eAssoc::right_, eOperation::unary_}},
	{eType::aureal_, {14000, eAssoc::right_, eOperation::unary_}},
	{eType::aoctet_, {14000, eAssoc::right_, eOperation::unary_}},
	{eType::abit_, {14000, eAssoc::right_, eOperation::unary_}},
	{eType::aarray_, {14000, eAssoc::right_, eOperation::unary_}},
	{eType::apointer_, {14000, eAssoc::right_, eOperation::unary_}},
	{eType::amemory_, {14000, eAssoc::right_, eOperation::unary_}},
	{eType::afunction_, {14000, eAssoc::right_, eOperation::unary_}},
	{eType::enter_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::start_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::type_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::var_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::class_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::const_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::static_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::ref_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::if_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::else_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::elif_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::while_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::for_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::switch_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::case_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::default_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::break_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::continue_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::return_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::into_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::statement_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::expression_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::declaration_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::program_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::compiled_program_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::interpreted_program_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::pragmatic_block_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::functional_block_, {15000, eAssoc::none_, eOperation::none_}},
	{eType::eof_, {INT_MIN, eAssoc::none_, eOperation::none_}}
	};

} // namespace caoco