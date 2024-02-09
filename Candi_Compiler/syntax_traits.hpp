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
//#include <stack>

namespace caoco {

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

	using TokenTraitsMapType = sl_map<tk_enum, sl_tuple<int, Associativity, Operation, astnode_enum>>;
	using NodeTraitsMapType = sl_map<astnode_enum, sl_tuple<int, Associativity, Operation>>;

#define caoco_TK_TRAIT(TOKEN_TYPE, IMPORTANCE, ASSOCIATIVITY, OPERATION, PRODUCED_STATEMENT) {tk_enum::TOKEN_TYPE, {IMPORTANCE, Associativity::ASSOCIATIVITY, Operation::OPERATION, astnode_enum::PRODUCED_STATEMENT}}
	
	static TokenTraitsMapType token_traits = {
	{tk_enum::alnumus_,		{INT_MAX,	Associativity::left_,	Operation::none_,		astnode_enum::alnumus_}},
	{tk_enum::number_literal_, {INT_MAX,	Associativity::left_,	Operation::none_,		astnode_enum::number_literal_}},
	{tk_enum::eof_,			{INT_MAX,	Associativity::left_,	Operation::none_,		astnode_enum::eof_}},
	caoco_TK_TRAIT(real_literal_,INT_MAX, left_, none_, real_literal_), // real literal
	caoco_TK_TRAIT(string_literal_,INT_MAX, left_, none_, string_literal_), // string literal
	caoco_TK_TRAIT(octet_literal_,INT_MAX, left_, none_, octet_literal_), // octet literal
	caoco_TK_TRAIT(bit_literal_,INT_MAX, left_, none_, bit_literal_), // bit literal
	caoco_TK_TRAIT(unsigned_literal_,INT_MAX, left_, none_, unsigned_literal_), // unsigned literal
	caoco_TK_TRAIT(open_frame_,INT_MAX, left_, unary_, open_frame_), // ( function call
	caoco_TK_TRAIT(period_,160000, left_, binary_, period_), // . operator
	caoco_TK_TRAIT(decrement_,140000, left_, unary_, decrement_), // --
	caoco_TK_TRAIT(increment_,140000, left_, unary_, increment_), // ++
	caoco_TK_TRAIT(bitwise_NOT_,140000, left_, unary_, bitwise_NOT_), // ~
	caoco_TK_TRAIT(negation_,140000, left_, unary_, negation_), // !
	caoco_TK_TRAIT(division_,130000, left_, binary_, division_), // /
	caoco_TK_TRAIT(multiplication_,130000, left_, binary_, multiplication_), // *
	caoco_TK_TRAIT(remainder_,130000, left_, binary_, remainder_), // %
	caoco_TK_TRAIT(subtraction_,120000, left_, binary_, subtraction_), // -
	caoco_TK_TRAIT(addition_,120000, left_, binary_, addition_), // +
	caoco_TK_TRAIT(bitwise_left_shift_,110000, left_, binary_, bitwise_left_shift_), // <<
	caoco_TK_TRAIT(bitwise_right_shift_,110000, left_, binary_, bitwise_right_shift_), // >>
	caoco_TK_TRAIT(three_way_comparison_,100000, left_, binary_, three_way_comparison_), // <=>
	caoco_TK_TRAIT(greater_than_,90000, left_, binary_, greater_than_), // >
	caoco_TK_TRAIT(less_than_,90000, left_, binary_, less_than_), // <
	caoco_TK_TRAIT(greater_than_or_equal_,90000, left_, binary_, greater_than_or_equal_), // >=
	caoco_TK_TRAIT(less_than_or_equal_,90000, left_, binary_, less_than_or_equal_), // <=
	caoco_TK_TRAIT(equal_,80000, left_, binary_, equal_), // ==
	caoco_TK_TRAIT(not_equal_,80000, left_, binary_, not_equal_), // !=
	caoco_TK_TRAIT(bitwise_AND_,70000, left_, binary_, bitwise_AND_), // &
	caoco_TK_TRAIT(bitwise_XOR_,60000, left_, binary_, bitwise_XOR_), // ^
	caoco_TK_TRAIT(bitwise_OR_,50000, left_, binary_, bitwise_OR_), // |
	caoco_TK_TRAIT(logical_AND_,40000, left_, binary_, logical_AND_), // &&
	caoco_TK_TRAIT(logical_OR_,30000, left_, binary_, logical_OR_), // ||
	// Assingment operators
	caoco_TK_TRAIT(simple_assignment_,20000, left_, binary_, simple_assignment_), // =
	caoco_TK_TRAIT(addition_assignment_,20000, left_, binary_, addition_assignment_), // +=
	caoco_TK_TRAIT(subtraction_assignment_,20000, left_, binary_, subtraction_assignment_), // -=
	caoco_TK_TRAIT(multiplication_assignment_,20000, left_, binary_, multiplication_assignment_), // *=
	caoco_TK_TRAIT(division_assignment_,20000, left_, binary_, division_assignment_), // /=
	caoco_TK_TRAIT(remainder_assignment_,20000, left_, binary_, remainder_assignment_), // %=
	caoco_TK_TRAIT(bitwise_and_assignment_,20000, left_, binary_, bitwise_and_assignment_), // &=
	caoco_TK_TRAIT(bitwise_or_assignment_,20000, left_, binary_, bitwise_or_assignment_), // |=	
	caoco_TK_TRAIT(bitwise_xor_assignment_,20000, left_, binary_, bitwise_xor_assignment_), // ^=
	caoco_TK_TRAIT(left_shift_assignment_,20000, left_, binary_, left_shift_assignment_), // <<=
	caoco_TK_TRAIT(right_shift_assignment_,20000, left_, binary_, right_shift_assignment_), // >>=
	};
	static NodeTraitsMapType node_traits = {
	{astnode_enum::alnumus_, {INT_MAX, Associativity::left_, Operation::none_}},
	{astnode_enum::number_literal_, {INT_MAX, Associativity::left_, Operation::none_}},
	{astnode_enum::addition_, {1000, Associativity::left_, Operation::binary_}},
	{astnode_enum::subtraction_, {1000, Associativity::left_, Operation::binary_}},
	{astnode_enum::multiplication_, {2000, Associativity::left_, Operation::binary_}},
	{astnode_enum::division_, {2000, Associativity::left_, Operation::binary_}},

	{astnode_enum::open_scope_, {INT_MAX, Associativity::left_, Operation::none_}},
	{astnode_enum::close_scope_, {INT_MAX, Associativity::left_, Operation::none_}},
	{astnode_enum::eof_, {INT_MAX, Associativity::left_, Operation::none_}}
	};

	Operation get_node_operation(astnode_enum node_type) {
		return std::get<static_cast<std::size_t>(TokenTraitIndex::operation_)>(node_traits[node_type]);
	}

	Associativity get_node_associativity(astnode_enum node_type) {
		return std::get<static_cast<std::size_t>(TokenTraitIndex::associativity_)>(node_traits[node_type]);
	}

	int get_node_importance(astnode_enum node_type) {
		return std::get<static_cast<std::size_t>(TokenTraitIndex::importance_)>(node_traits[node_type]);
	}

	Operation get_token_operation(tk_enum token_type) {
		return std::get<static_cast<std::size_t>(TokenTraitIndex::operation_)>(token_traits[token_type]);
	}

	Associativity get_token_associativity(tk_enum token_type) {
		return std::get<static_cast<std::size_t>(TokenTraitIndex::associativity_)>(token_traits[token_type]);
	}

	int get_token_importance(tk_enum token_type) {
		return std::get<static_cast<std::size_t>(TokenTraitIndex::importance_)>(token_traits[token_type]);
	}

};// namespace caoco