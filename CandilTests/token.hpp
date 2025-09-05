#pragma once
#include "global_typedef.hpp"
#include "char_traits.hpp"
#include "import.hpp"


namespace cil {
	enum class TokenAssociativity {
		none,
		left,
		right
	};

	enum class TokenOperationType {
		none,
		unary,
		binary,
		variadic
	};

	enum class TokenKind {
		// Abstract 
		none,
		invalid,

		// Base 
		line_comment,
		block_comment,
		string_literal,
		number_literal,
		real_literal,
		newline,
		whitespace,
		alnumus,
		directive, // directive #enter, #start etc.
		candi, // & candi special operator
		eof,

		// Assignemnt operators
		simple_assignment, // =
		addition_assignment, // +=
		subtraction_assignment, // -=
		multiplication_assignment, // *=
		division_assignment, // /=
		remainder_assignment, // %=
		bitwise_and_assignment, // &=
		bitwise_or_assignment, // |=
		bitwise_xor_assignment, // ^=
		left_shift_assignment, // <<=
		right_shift_assignment, // >>=	

		// Increment and decrement operators
		increment, // ++
		decrement, // --

		// Arithmetic operators
		addition, // +
		subtraction, // -
		multiplication, // *
		division, // /
		remainder, // %
		bitwise_NOT, // ~
		bitwise_AND, // &
		bitwise_OR, // |
		bitwise_XOR, // ^
		bitwise_left_shift, // <<
		bitwise_right_shift, // >>

		// Logical
		negation, // !
		logical_AND, // &&
		logical_OR, // ||

		// Comparison
		equal, // ==
		not_equal, // !=
		less_than, // <
		greater_than, // >
		less_than_or_equal, // <=
		greater_than_or_equal, // >=
		three_way_comparison, // <=>

		// Scopes
		open_scope, // (
		close_scope, // )
		open_list, // {
		close_list, // }
		open_frame, // [
		close_frame, // ]

		// Special
		eos, // ;
		comma, // ,

	}; // enum class TokenKind
	
	struct Token {
		TokenKind kind;
		rvector<char>::const_iterator begin;
		rvector<char>::const_iterator end;

		int char_index;
		int line_index;

		/* <@brief> Returns the string literal of the token. */
		rstring lit() const  {
			return rstring{ begin, end };
		};

		/* <@brief> Returns the number of characters contained in the string literal. */
		auto size() const {
			return std::distance(begin, end);
		};

		/* <@brief> Returns the type(kind) of the token. */
		TokenKind type() const {
			return kind;
		};

		auto line() const {
			return line_index;
		};

		auto column() const {
			return char_index;
		};

		/* <@brief> Returns the precidence level of this token. Operand and Literal Tokens will be of highest precidence.*/
		int precidence() const {
			switch (kind) {
			case TokenKind::none:
			case TokenKind::invalid:
			case TokenKind::line_comment:
			case TokenKind::block_comment:
			case TokenKind::string_literal:
			case TokenKind::number_literal:
			case TokenKind::real_literal:
			case TokenKind::newline:
			case TokenKind::whitespace:
			case TokenKind::alnumus:
			case TokenKind::directive:
			case TokenKind::candi:
			case TokenKind::eos:
			case TokenKind::comma:
			case TokenKind::open_scope:
			case TokenKind::close_scope:
			case TokenKind::open_list:
			case TokenKind::close_list:
			case TokenKind::open_frame:
			case TokenKind::close_frame:
			case TokenKind::simple_assignment:
			case TokenKind::addition_assignment:
			case TokenKind::subtraction_assignment:
			case TokenKind::multiplication_assignment:
			case TokenKind::division_assignment:
			case TokenKind::remainder_assignment:
			case TokenKind::bitwise_and_assignment:
			case TokenKind::bitwise_or_assignment:
			case TokenKind::bitwise_xor_assignment:
			case TokenKind::left_shift_assignment:
			case TokenKind::right_shift_assignment:
			case TokenKind::increment:
			case TokenKind::decrement:
				return 0;
			case TokenKind::addition:
			case TokenKind::subtraction:
				return std::numeric_limits<int>::max();
			case TokenKind::multiplication:
			case TokenKind::division:
			case TokenKind::remainder:
				return std::numeric_limits<int>::max() - 2;
			case TokenKind::bitwise_NOT:
			case TokenKind::bitwise_AND:
			case TokenKind::bitwise_OR:
			case TokenKind::bitwise_XOR:
			case TokenKind::bitwise_left_shift:
			case TokenKind::bitwise_right_shift:
			case TokenKind::negation:
			case TokenKind::logical_AND:
			case TokenKind::logical_OR:
			case TokenKind::equal:
			case TokenKind::not_equal:
			case TokenKind::less_than:
			case TokenKind::greater_than:
			case TokenKind::less_than_or_equal:
			case TokenKind::greater_than_or_equal:
			case TokenKind::three_way_comparison:
				return std::numeric_limits<int>::max() - 3;
			default:
				return 0;
			}
		};
		TokenAssociativity associativity() const {
			switch (kind) {
			case TokenKind::none:
			case TokenKind::invalid:
			case TokenKind::line_comment:
			case TokenKind::block_comment:
			case TokenKind::string_literal:
			case TokenKind::number_literal:
			case TokenKind::real_literal:
			case TokenKind::newline:
			case TokenKind::whitespace:
			case TokenKind::alnumus:
			case TokenKind::directive:
			case TokenKind::candi:
			case TokenKind::eos:
			case TokenKind::comma:
			case TokenKind::open_scope:
			case TokenKind::close_scope:
			case TokenKind::open_list:
			case TokenKind::close_list:
			case TokenKind::open_frame:
			case TokenKind::close_frame:
			case TokenKind::simple_assignment:
			case TokenKind::addition_assignment:
			case TokenKind::subtraction_assignment:
			case TokenKind::multiplication_assignment:
			case TokenKind::division_assignment:
			case TokenKind::remainder_assignment:
			case TokenKind::bitwise_and_assignment:
			case TokenKind::bitwise_or_assignment:
			case TokenKind::bitwise_xor_assignment:
			case TokenKind::left_shift_assignment:
			case TokenKind::right_shift_assignment:
			case TokenKind::increment:
			case TokenKind::decrement:
				return TokenAssociativity::none;
			case TokenKind::addition:
			case TokenKind::subtraction:
			case TokenKind::multiplication:
			case TokenKind::division:
			case TokenKind::remainder:
			case TokenKind::bitwise_NOT:
			case TokenKind::bitwise_AND:
			case TokenKind::bitwise_OR:
			case TokenKind::bitwise_XOR:
			case TokenKind::bitwise_left_shift:
			case TokenKind::bitwise_right_shift:
			case TokenKind::negation:
			case TokenKind::logical_AND:
			case TokenKind::logical_OR:
			case TokenKind::equal:
			case TokenKind::not_equal:
			case TokenKind::less_than:
			case TokenKind::greater_than:
			case TokenKind::less_than_or_equal:
			case TokenKind::greater_than_or_equal:
			case TokenKind::three_way_comparison:
				return TokenAssociativity::left;
			default:
				return TokenAssociativity::none;
			}
		};

		TokenOperationType operation_type() const {
			switch (kind) {
            case TokenKind::none:
			case TokenKind::invalid:
			case TokenKind::line_comment:
			case TokenKind::block_comment:
			case TokenKind::string_literal:
			case TokenKind::number_literal:
			case TokenKind::real_literal:
			case TokenKind::newline:
			case TokenKind::whitespace:
			case TokenKind::alnumus:
			case TokenKind::directive:
			case TokenKind::candi:
			case TokenKind::eos:
			case TokenKind::comma:
			case TokenKind::open_scope:
			case TokenKind::close_scope:
			case TokenKind::open_list:
			case TokenKind::close_list:
			case TokenKind::open_frame:
			case TokenKind::close_frame:
				return TokenOperationType::none;
			case TokenKind::simple_assignment:
			case TokenKind::addition_assignment:
			case TokenKind::subtraction_assignment:
			case TokenKind::multiplication_assignment:
			case TokenKind::division_assignment:
			case TokenKind::remainder_assignment:
			case TokenKind::bitwise_and_assignment:
			case TokenKind::bitwise_or_assignment:
			case TokenKind::bitwise_xor_assignment:
			case TokenKind::left_shift_assignment:
			case TokenKind::right_shift_assignment:
				return TokenOperationType::binary;
			case TokenKind::increment:
			case TokenKind::decrement:
				return TokenOperationType::unary;
			case TokenKind::addition:
			case TokenKind::subtraction:
			case TokenKind::multiplication:
			case TokenKind::division:
			case TokenKind::remainder:
			case TokenKind::bitwise_NOT:
			case TokenKind::bitwise_AND:
			case TokenKind::bitwise_OR:
			case TokenKind::bitwise_XOR:
			case TokenKind::bitwise_left_shift:
			case TokenKind::bitwise_right_shift:
			case TokenKind::negation:
			case TokenKind::logical_AND:
			case TokenKind::logical_OR:
			case TokenKind::equal:
			case TokenKind::not_equal:
			case TokenKind::less_than:
			case TokenKind::greater_than:
			case TokenKind::less_than_or_equal:
			case TokenKind::greater_than_or_equal:
			case TokenKind::three_way_comparison:
				return TokenOperationType::binary;
			default:
				return TokenOperationType::none;
			}
		};

		/* <@brief> Returns true if the token is a literal value. One of: number_literal, real_literal, string_literal */
		bool is_literal_value() const {
			switch (kind) {
			case TokenKind::number_literal:
			case TokenKind::real_literal:
			case TokenKind::string_literal:
				return true;
			default:
				return false;
			}
		};

		bool is_identity_expr_operator() const {
			switch (kind)
			{
			case cil::TokenKind::addition:
			case cil::TokenKind::subtraction:
			case cil::TokenKind::multiplication:
				return true;
			default:
				return false;
			}
		}

		auto identity_expr_importance() const{
			switch (kind)
			{
			case cil::TokenKind::addition:
				return 1;
			case cil::TokenKind::multiplication:
				return 2;
			case cil::TokenKind::subtraction:
				return 3;
			default:
				return 0;
			}
		}

		/* <@brief> Returns true if the token is a literal operator */
		bool is_literal_operator() const {
			switch (kind)
			{
			case cil::TokenKind::increment:
			case cil::TokenKind::decrement:
			case cil::TokenKind::addition:
			case cil::TokenKind::subtraction:
			case cil::TokenKind::multiplication:
			case cil::TokenKind::division:
			case cil::TokenKind::remainder:
			case cil::TokenKind::bitwise_NOT:
			case cil::TokenKind::bitwise_AND:
			case cil::TokenKind::bitwise_OR:
			case cil::TokenKind::bitwise_XOR:
			case cil::TokenKind::bitwise_left_shift:
			case cil::TokenKind::bitwise_right_shift:
			case cil::TokenKind::negation:
			case cil::TokenKind::logical_AND:
			case cil::TokenKind::logical_OR:
			case cil::TokenKind::equal:
			case cil::TokenKind::not_equal:
			case cil::TokenKind::less_than:
			case cil::TokenKind::greater_than:
			case cil::TokenKind::less_than_or_equal:
			case cil::TokenKind::greater_than_or_equal:
			case cil::TokenKind::three_way_comparison:
				return true;
			default:
				return false;
			}
		}

		/* <@brief> Equality Operator: Equal if the type and literal are equal. */
		bool operator==(const Token& rhs) const {
			return kind == rhs.kind && lit() == rhs.lit();
		};

		/* <@brief> Inequality Operator: Not equal if the type or literal are not equal. */
		bool operator!=(const Token& rhs) const {
			return !(*this == rhs);
		};

	};// struct Token

} // namespace cil
