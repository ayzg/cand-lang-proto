#pragma once
#include "token.hpp"
#include "ast_node.hpp"

// !Warning: do not use this macro outside of syntax_traits.hpp
#define CAOCO_TK_TRAIT(TOKEN_TYPE, IMPORTANCE, ASSOCIATIVITY, OPERATION, PRODUCED_STATEMENT) \
 {tk_enum::TOKEN_TYPE, {IMPORTANCE, e_assoc::ASSOCIATIVITY, e_operation::OPERATION, astnode_enum::PRODUCED_STATEMENT}}

// !Warning: do not use this macro outside of syntax_traits.hpp
#define CAOCO_AST_TRAIT(NODE_TYPE, IMPORTANCE, ASSOCIATIVITY, OPERATION) \
{astnode_enum::NODE_TYPE, {IMPORTANCE, e_assoc::ASSOCIATIVITY, e_operation::OPERATION}}

namespace caoco {
	namespace syntax {
		enum class e_assoc : bool {
			left_ = true,
			right_ = false
		};
		enum class e_operation : int {
			binary_ = 0,
			unary_ = 1,
			none_ = 2
		};
		enum class e_tk_trait : sl_size {
			priority_ = 0,
			associativity_ = 1,
			operation_ = 2,
			produced_statement_ = 3
		};
		enum class e_astnode_trait : sl_size {
			priority_ = 0,
			associativity_ = 1,
			operation_ = 2
		};

		using tk_traits_map = sl_map<tk_enum, sl_tuple<int, e_assoc, e_operation, astnode_enum>>;
		using astnode_traits_map = sl_map<astnode_enum, sl_tuple<int, e_assoc, e_operation>>;

		namespace priority {
			constexpr auto max = INT_MAX;
			constexpr auto functional = 150000;
			constexpr auto unary = 140000;
			constexpr auto factor = 130000;
			constexpr auto term = 120000;
			constexpr auto bitshift = 110000;
			constexpr auto three_way_equality = 100000;
			constexpr auto comparison = 90000;
			constexpr auto equality = 80000;
			constexpr auto bitwise = 70000;
			constexpr auto logical = 60000;
			constexpr auto assignment = 20000;
		}

		static tk_traits_map token_traits = {
		CAOCO_TK_TRAIT(alnumus_,priority::max, left_, none_, alnumus_), // alnumus
		CAOCO_TK_TRAIT(number_literal_,priority::max, left_, none_, number_literal_), // number literal
		CAOCO_TK_TRAIT(eof_,priority::max, left_, none_, eof_), // eof
		CAOCO_TK_TRAIT(real_literal_,priority::max, left_, none_, real_literal_), // real literal
		CAOCO_TK_TRAIT(string_literal_,priority::max, left_, none_, string_literal_), // string literal
		CAOCO_TK_TRAIT(octet_literal_,priority::max, left_, none_, octet_literal_), // octet literal
		CAOCO_TK_TRAIT(bit_literal_,priority::max, left_, none_, bit_literal_), // bit literal
		CAOCO_TK_TRAIT(unsigned_literal_,priority::max, left_, none_, unsigned_literal_), // unsigned literal
		CAOCO_TK_TRAIT(open_frame_,priority::max, left_, unary_, open_frame_), // ( function call
		CAOCO_TK_TRAIT(period_,priority::functional, left_, binary_, period_), // . operator
		CAOCO_TK_TRAIT(decrement_,priority::unary, left_, unary_, decrement_), // --
		CAOCO_TK_TRAIT(increment_,priority::unary, left_, unary_, increment_), // ++
		CAOCO_TK_TRAIT(bitwise_NOT_,priority::unary, left_, unary_, bitwise_NOT_), // ~
		CAOCO_TK_TRAIT(negation_,priority::unary, left_, unary_, negation_), // !
		CAOCO_TK_TRAIT(division_,priority::factor, left_, binary_, division_), // /
		CAOCO_TK_TRAIT(multiplication_,priority::factor, left_, binary_, multiplication_), // *
		CAOCO_TK_TRAIT(remainder_,priority::factor, left_, binary_, remainder_), // %
		CAOCO_TK_TRAIT(subtraction_,priority::term, left_, binary_, subtraction_), // -
		CAOCO_TK_TRAIT(addition_,priority::term, left_, binary_, addition_), // +
		CAOCO_TK_TRAIT(bitwise_left_shift_,priority::bitshift, left_, binary_, bitwise_left_shift_), // <<
		CAOCO_TK_TRAIT(bitwise_right_shift_,priority::bitshift, left_, binary_, bitwise_right_shift_), // >>
		CAOCO_TK_TRAIT(three_way_comparison_,priority::three_way_equality, left_, binary_, three_way_comparison_), // <=>
		CAOCO_TK_TRAIT(greater_than_,priority::comparison, left_, binary_, greater_than_), // >
		CAOCO_TK_TRAIT(less_than_,priority::comparison, left_, binary_, less_than_), // <
		CAOCO_TK_TRAIT(greater_than_or_equal_,priority::comparison, left_, binary_, greater_than_or_equal_), // >=
		CAOCO_TK_TRAIT(less_than_or_equal_,priority::comparison, left_, binary_, less_than_or_equal_), // <=
		CAOCO_TK_TRAIT(equal_,priority::equality, left_, binary_, equal_), // ==
		CAOCO_TK_TRAIT(not_equal_,priority::equality, left_, binary_, not_equal_), // !=
		CAOCO_TK_TRAIT(bitwise_AND_,priority::bitwise, left_, binary_, bitwise_AND_), // &
		CAOCO_TK_TRAIT(bitwise_XOR_,priority::bitwise-1, left_, binary_, bitwise_XOR_), // ^
		CAOCO_TK_TRAIT(bitwise_OR_,priority::bitwise-2, left_, binary_, bitwise_OR_), // |
		CAOCO_TK_TRAIT(logical_AND_,priority::logical, left_, binary_, logical_AND_), // &&
		CAOCO_TK_TRAIT(logical_OR_,priority::logical-1, left_, binary_, logical_OR_), // ||
		// Assingment operators
		CAOCO_TK_TRAIT(simple_assignment_,priority::assignment, left_, binary_, simple_assignment_), // =
		CAOCO_TK_TRAIT(addition_assignment_,priority::assignment, left_, binary_, addition_assignment_), // +=
		CAOCO_TK_TRAIT(subtraction_assignment_,priority::assignment, left_, binary_, subtraction_assignment_), // -=
		CAOCO_TK_TRAIT(multiplication_assignment_,priority::assignment, left_, binary_, multiplication_assignment_), // *=
		CAOCO_TK_TRAIT(division_assignment_,priority::assignment, left_, binary_, division_assignment_), // /=
		CAOCO_TK_TRAIT(remainder_assignment_,priority::assignment, left_, binary_, remainder_assignment_), // %=
		CAOCO_TK_TRAIT(bitwise_and_assignment_,priority::assignment, left_, binary_, bitwise_and_assignment_), // &=
		CAOCO_TK_TRAIT(bitwise_or_assignment_,priority::assignment, left_, binary_, bitwise_or_assignment_), // |=	
		CAOCO_TK_TRAIT(bitwise_xor_assignment_,priority::assignment, left_, binary_, bitwise_xor_assignment_), // ^=
		CAOCO_TK_TRAIT(left_shift_assignment_,priority::assignment, left_, binary_, left_shift_assignment_), // <<=
		CAOCO_TK_TRAIT(right_shift_assignment_,priority::assignment, left_, binary_, right_shift_assignment_), // >>=
		};
		static astnode_traits_map node_traits = {
		CAOCO_AST_TRAIT(alnumus_,priority::max, left_, none_), // alnumus
		CAOCO_AST_TRAIT(number_literal_,priority::max, left_, none_), // number literal
		CAOCO_AST_TRAIT(eof_,priority::max, left_, none_), // eof
		CAOCO_AST_TRAIT(real_literal_,priority::max, left_, none_), // real literal
		CAOCO_AST_TRAIT(string_literal_,priority::max, left_, none_), // string literal
		CAOCO_AST_TRAIT(octet_literal_,priority::max, left_, none_), // octet literal
		CAOCO_AST_TRAIT(bit_literal_,priority::max, left_, none_), // bit literal
		CAOCO_AST_TRAIT(unsigned_literal_,priority::max, left_, none_), // unsigned literal
		CAOCO_AST_TRAIT(open_frame_,priority::max, left_, unary_), // ( function call
		CAOCO_AST_TRAIT(period_,priority::functional, left_, binary_), // . operator
		CAOCO_AST_TRAIT(decrement_,priority::unary, left_, unary_), // --
		CAOCO_AST_TRAIT(increment_,priority::unary, left_, unary_), // ++
		CAOCO_AST_TRAIT(bitwise_NOT_,priority::unary, left_, unary_), // ~
		CAOCO_AST_TRAIT(negation_,priority::unary, left_, unary_), // !
		CAOCO_AST_TRAIT(division_,priority::factor, left_, binary_), // /
		CAOCO_AST_TRAIT(multiplication_,priority::factor, left_, binary_), // *
		CAOCO_AST_TRAIT(remainder_,priority::factor, left_, binary_), // %
		CAOCO_AST_TRAIT(subtraction_,priority::term, left_, binary_), // -
		CAOCO_AST_TRAIT(addition_,priority::term, left_, binary_), // +
		CAOCO_AST_TRAIT(bitwise_left_shift_,priority::bitshift, left_, binary_), // <<
		CAOCO_AST_TRAIT(bitwise_right_shift_,priority::bitshift, left_, binary_), // >>
		CAOCO_AST_TRAIT(three_way_comparison_,priority::three_way_equality, left_, binary_), // <=>
		CAOCO_AST_TRAIT(greater_than_,priority::comparison, left_, binary_), // >
		CAOCO_AST_TRAIT(less_than_,priority::comparison, left_, binary_), // <
		CAOCO_AST_TRAIT(greater_than_or_equal_,priority::comparison, left_, binary_), // >=
		CAOCO_AST_TRAIT(less_than_or_equal_,priority::comparison, left_, binary_), // <=
		CAOCO_AST_TRAIT(equal_,priority::equality, left_, binary_), // ==
		CAOCO_AST_TRAIT(not_equal_,priority::equality, left_, binary_), // !=
		CAOCO_AST_TRAIT(bitwise_AND_,priority::bitwise, left_, binary_), // &
		CAOCO_AST_TRAIT(bitwise_XOR_,priority::bitwise - 1, left_, binary_), // ^
		CAOCO_AST_TRAIT(bitwise_OR_,priority::bitwise - 2, left_, binary_), // |
		CAOCO_AST_TRAIT(logical_AND_,priority::logical, left_, binary_), // &&
		CAOCO_AST_TRAIT(logical_OR_,priority::logical - 1, left_, binary_), // ||
		// Assingment operators
		CAOCO_AST_TRAIT(simple_assignment_,priority::assignment, left_, binary_), // =
		CAOCO_AST_TRAIT(addition_assignment_,priority::assignment, left_, binary_), // +=
		CAOCO_AST_TRAIT(subtraction_assignment_,priority::assignment, left_, binary_), // -=
		CAOCO_AST_TRAIT(multiplication_assignment_,priority::assignment, left_, binary_), // *=
		CAOCO_AST_TRAIT(division_assignment_,priority::assignment, left_, binary_), // /=
		CAOCO_AST_TRAIT(remainder_assignment_,priority::assignment, left_, binary_), // %=
		CAOCO_AST_TRAIT(bitwise_and_assignment_,priority::assignment, left_, binary_), // &=
		CAOCO_AST_TRAIT(bitwise_or_assignment_,priority::assignment, left_, binary_), // |=	
		CAOCO_AST_TRAIT(bitwise_xor_assignment_,priority::assignment, left_, binary_), // ^=
		CAOCO_AST_TRAIT(left_shift_assignment_,priority::assignment, left_, binary_), // <<=
		CAOCO_AST_TRAIT(right_shift_assignment_,priority::assignment, left_, binary_), // >>=
		};


		e_operation get_node_operation(astnode_enum node_type);
		e_assoc get_node_associativity(astnode_enum node_type);
		int get_node_priority(astnode_enum node_type);
		e_operation get_token_operation(tk_enum token_type);
		e_assoc get_token_associativity(tk_enum token_type);
		int get_token_priority(tk_enum token_type);

		// Specific Methods based on the node type.
		namespace ast {

			// Requirement: node_type must be a binary operation.
			astnode& get_binop_lhs(astnode& node) {
				return node.front();
			}

			astnode& get_binop_rhs(astnode& node) {
				return node.back();
			}

			astnode& get_unop_operand(astnode& node) {
				return node.front();
			}

			// Requirement: node_type must be one of variable declarations:
			//		anon_variable_definition_, anon_variable_definition_assingment_, 
			//		constrained_variable_definition_,constrained_variable_definition_assingment_
			astnode& get_vardef_name(astnode& node) {
				return node.front();
			}

			astnode& get_vardef_constraint(astnode& node) {
				// todo: create a way to return some sort of sentinel node which indicates an error or an "unassigned value"
				if (node.type() == astnode_enum::constrained_variable_definition_ || node.type() == astnode_enum::constrained_variable_definition_assingment_) {
					return node[1];
				}
				else {
					throw "Invalid node type for get_vardef_constraint()";
				}
			}

			astnode& get_vardef_assignop(astnode& node) {
				if (node.type() == astnode_enum::anon_variable_definition_assingment_) {
					return node[1];
				}
				else if (node.type() == astnode_enum::constrained_variable_definition_assingment_) {
					return node[2];
				}
				else {
					throw "Invalid node type for get_vardef_assignop()";
				}
			}
		};// namespace ast

	};// namespace syntax


	syntax::e_operation syntax::get_node_operation(astnode_enum node_type) {
		return std::get<static_cast<sl_size>(e_tk_trait::operation_)>(node_traits[node_type]);
	}

	syntax::e_assoc syntax::get_node_associativity(astnode_enum node_type) {
		return std::get<static_cast<sl_size>(e_tk_trait::associativity_)>(node_traits[node_type]);
	}

	int syntax::get_node_priority(astnode_enum node_type) {
		return std::get<static_cast<sl_size>(e_tk_trait::priority_)>(node_traits[node_type]);
	}

	syntax::e_operation syntax::get_token_operation(tk_enum token_type) {
		return std::get<static_cast<sl_size>(e_tk_trait::operation_)>(token_traits[token_type]);
	}

	syntax::e_assoc syntax::get_token_associativity(tk_enum token_type) {
		return std::get<static_cast<sl_size>(e_tk_trait::associativity_)>(token_traits[token_type]);
	}

	int syntax::get_token_priority(tk_enum token_type) {
		return std::get<static_cast<sl_size>(e_tk_trait::priority_)>(token_traits[token_type]);
	}


};// namespace caoco