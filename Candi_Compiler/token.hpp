#pragma once
#include "global_dependencies.hpp"
#include "char_traits.hpp"
namespace caoco {
	class tk {
	public:
		enum class e_type : int {
			// Abstract 
			none = -1, invalid, eof,
			// Base 
			line_comment, block_comment,
			string_literal, number_literal, real_literal, octet_literal,
			bit_literal, unsigned_literal, newline, whitespace, alnumus,
			// Assignemnt operators
			simple_assignment, addition_assignment, subtraction_assignment, // -=
			multiplication_assignment, division_assignment, remainder_assignment, // %=
			bitwise_and_assignment, bitwise_or_assignment, bitwise_xor_assignment, // ^=
			left_shift_assignment, right_shift_assignment,
			// Increment and decrement operators
			increment, decrement,
			// Arithmetic operators
			addition, subtraction, multiplication, division, remainder,
			bitwise_NOT, bitwise_AND, bitwise_OR, bitwise_XOR,
			bitwise_left_shift, bitwise_right_shift,
			// Logical
			negation, logical_AND, logical_OR,
			// Comparison
			equal, not_equal, less_than, greater_than,
			less_than_or_equal, greater_than_or_equal, three_way_comparison,
			// Scopes
			open_scope, close_scope, open_list, close_list, open_frame, close_frame,
			// Special
			eos, comma, period, ellipsis,
			// Special Tokens
			atype_, aidentity_, avalue_, aint_, auint_, areal_, aoctet_,
			abit_, aarray_, apointer_, amemory_, afunction_,
			// Directive Tokens
			enter_, start_, type_, var_, class_, func_, print_, none_literal_,
			// Modifier Tokens
			public_, const_, static_, ref_,
			// Control Flow Tokens
			if_, else_, elif_, while_, for_, switch_, case_, default_, break_,
			continue_, return_, into_
		};

		using eType = e_type; // Temporary alias
	private:
		e_type type_;
		sl_char8_vector_cit beg_;
		sl_char8_vector_cit end_;

		sl_size line_;
		sl_size col_;
	public:
		SL_CX tk() noexcept : type_(e_type::none), beg_(), end_(), line_(0), col_(0) {}
		SL_CX tk(e_type type) noexcept : type_(type), beg_(), end_(), line_(0), col_(0) {}
		SL_CX tk(e_type type, sl_char8_vector_cit beg, sl_char8_vector_cit end) noexcept
			: type_(type), beg_(beg), end_(end), line_(0), col_(0) {}
		SL_CX tk(e_type type, sl_char8_vector_cit beg, sl_char8_vector_cit end, sl_size line, sl_size col) noexcept
			: type_(type), beg_(beg), end_(end), line_(line), col_(col) {}
	public:
		SL_CX e_type type() const noexcept { return type_; }
		SL_CX sl_char8_vector_cit beg() const noexcept { return beg_; }
		SL_CX sl_char8_vector_cit end() const noexcept { return end_; }
		SL_CX sl_size size() const { return std::distance(beg_, end_); }
		SL_CX sl_size line() const noexcept { return line_; }
		SL_CX sl_size col() const noexcept { return col_; }
		SL_CX sl_u8string literal() const {
			// WARNING: Attempting to check or dereference the end or begin of a token with no literal will result in an exception.
			return sl_u8string(beg_, end_);
		}
		SL_CX bool type_is(e_type type) const noexcept {
			return type_ == type;
		}
		SL_CX bool operator==(const tk& rhs) const {
			return type_ == rhs.type() && literal() == rhs.literal();
		};
		SL_CX bool operator!=(const tk& rhs) const {
			return !(*this == rhs);
		};
	};

	using tk_enum = tk::e_type;
	using Tk = tk; // Temporary alias
};