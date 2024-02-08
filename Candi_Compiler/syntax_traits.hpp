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

namespace caoco {
	using tk_vector_t = std::vector<Tk>;
	using tk_iterator_t = tk_vector_t::const_iterator;
	using tk_enum = Tk::eType;
	using node_enum = Node::eType;
	enum class Associativity : bool {
		left_ = true,
		right_ = false
	};
	enum class Operation : int {
		binary_ = 0,
		unary_ = 1,
		none_ = 2
	};
	enum class TokenTraitIndex : std::size_t {
		importance_ = 0,
		associativity_ = 1,
		operation_ = 2,
		produced_statement_ = 3
	};
	enum class StatementTraitIndex : std::size_t {
		importance_ = 0,
		associativity_ = 1,
		operation_ = 2
	};

	using TokenTraitsMapType = std::map<Tk::eType, std::tuple<int, Associativity, Operation, Node::eType>>;
	using NodeTraitsMapType = std::map<Node::eType, std::tuple<int, Associativity, Operation>>;

#define caoco_TK_TRAIT(TOKEN_TYPE, IMPORTANCE, ASSOCIATIVITY, OPERATION, PRODUCED_STATEMENT) {Tk::eType::TOKEN_TYPE, {IMPORTANCE, Associativity::ASSOCIATIVITY, Operation::OPERATION, Node::eType::PRODUCED_STATEMENT}}
	static TokenTraitsMapType token_traits = {
	{Tk::eType::alnumus,		{INT_MAX,	Associativity::left_,	Operation::none_,		Node::eType::alnumus_}},
	{Tk::eType::number_literal, {INT_MAX,	Associativity::left_,	Operation::none_,		Node::eType::number_literal_}},
	{Tk::eType::eof,			{INT_MAX,	Associativity::left_,	Operation::none_,		Node::eType::eof_}},
	caoco_TK_TRAIT(real_literal,INT_MAX, left_, none_, real_literal_), // real literal
	caoco_TK_TRAIT(string_literal,INT_MAX, left_, none_, string_literal_), // string literal
	caoco_TK_TRAIT(octet_literal,INT_MAX, left_, none_, octet_literal_), // octet literal
	caoco_TK_TRAIT(bit_literal,INT_MAX, left_, none_, bit_literal_), // bit literal
	caoco_TK_TRAIT(unsigned_literal,INT_MAX, left_, none_, unsigned_literal_), // unsigned literal
	caoco_TK_TRAIT(open_frame,INT_MAX, left_, unary_, open_frame_), // ( function call
	caoco_TK_TRAIT(period,160000, left_, binary_, period_), // . operator
	caoco_TK_TRAIT(decrement,140000, left_, unary_, decrement_), // --
	caoco_TK_TRAIT(increment,140000, left_, unary_, increment_), // ++
	caoco_TK_TRAIT(bitwise_NOT,140000, left_, unary_, bitwise_NOT_), // ~
	caoco_TK_TRAIT(negation,140000, left_, unary_, negation_), // !
	caoco_TK_TRAIT(division,130000, left_, binary_, division_), // /
	caoco_TK_TRAIT(multiplication,130000, left_, binary_, multiplication_), // *
	caoco_TK_TRAIT(remainder,130000, left_, binary_, remainder_), // %
	caoco_TK_TRAIT(subtraction,120000, left_, binary_, subtraction_), // -
	caoco_TK_TRAIT(addition,120000, left_, binary_, addition_), // +
	caoco_TK_TRAIT(bitwise_left_shift,110000, left_, binary_, bitwise_left_shift_), // <<
	caoco_TK_TRAIT(bitwise_right_shift,110000, left_, binary_, bitwise_right_shift_), // >>
	caoco_TK_TRAIT(three_way_comparison,100000, left_, binary_, three_way_comparison_), // <=>
	caoco_TK_TRAIT(greater_than,90000, left_, binary_, greater_than_), // >
	caoco_TK_TRAIT(less_than,90000, left_, binary_, less_than_), // <
	caoco_TK_TRAIT(greater_than_or_equal,90000, left_, binary_, greater_than_or_equal_), // >=
	caoco_TK_TRAIT(less_than_or_equal,90000, left_, binary_, less_than_or_equal_), // <=
	caoco_TK_TRAIT(equal,80000, left_, binary_, equal_), // ==
	caoco_TK_TRAIT(not_equal,80000, left_, binary_, not_equal_), // !=
	caoco_TK_TRAIT(bitwise_AND,70000, left_, binary_, bitwise_AND_), // &
	caoco_TK_TRAIT(bitwise_XOR,60000, left_, binary_, bitwise_XOR_), // ^
	caoco_TK_TRAIT(bitwise_OR,50000, left_, binary_, bitwise_OR_), // |
	caoco_TK_TRAIT(logical_AND,40000, left_, binary_, logical_AND_), // &&
	caoco_TK_TRAIT(logical_OR,30000, left_, binary_, logical_OR_), // ||
	// Assingment operators
	caoco_TK_TRAIT(simple_assignment,20000, left_, binary_, simple_assignment_), // =
	caoco_TK_TRAIT(addition_assignment,20000, left_, binary_, addition_assignment_), // +=
	caoco_TK_TRAIT(subtraction_assignment,20000, left_, binary_, subtraction_assignment_), // -=
	caoco_TK_TRAIT(multiplication_assignment,20000, left_, binary_, multiplication_assignment_), // *=
	caoco_TK_TRAIT(division_assignment,20000, left_, binary_, division_assignment_), // /=
	caoco_TK_TRAIT(remainder_assignment,20000, left_, binary_, remainder_assignment_), // %=
	caoco_TK_TRAIT(bitwise_and_assignment,20000, left_, binary_, bitwise_and_assignment_), // &=
	caoco_TK_TRAIT(bitwise_or_assignment,20000, left_, binary_, bitwise_or_assignment_), // |=	
	caoco_TK_TRAIT(bitwise_xor_assignment,20000, left_, binary_, bitwise_xor_assignment_), // ^=
	caoco_TK_TRAIT(left_shift_assignment,20000, left_, binary_, left_shift_assignment_), // <<=
	caoco_TK_TRAIT(right_shift_assignment,20000, left_, binary_, right_shift_assignment_), // >>=
	};
	static NodeTraitsMapType node_traits = {
	{Node::eType::alnumus_, {INT_MAX, Associativity::left_, Operation::none_}},
	{Node::eType::number_literal_, {INT_MAX, Associativity::left_, Operation::none_}},
	{Node::eType::addition_, {1000, Associativity::left_, Operation::binary_}},
	{Node::eType::subtraction_, {1000, Associativity::left_, Operation::binary_}},
	{Node::eType::multiplication_, {2000, Associativity::left_, Operation::binary_}},
	{Node::eType::division_, {2000, Associativity::left_, Operation::binary_}},

	{Node::eType::open_scope_, {INT_MAX, Associativity::left_, Operation::none_}},
	{Node::eType::close_scope_, {INT_MAX, Associativity::left_, Operation::none_}},
	{Node::eType::eof_, {INT_MAX, Associativity::left_, Operation::none_}}
	};

};// namespace caoco