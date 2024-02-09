#pragma once
#include "global_dependencies.hpp"
#include "char_traits.hpp"
namespace caoco {
	class tk {
	public:
		enum class e_type : int {
			// Abstract 
			none_ = -1, invalid_, eof_,
			// Base 
			line_comment_, block_comment_,
			string_literal_, number_literal_, real_literal_, octet_literal_,
			bit_literal_, unsigned_literal_, newline_, whitespace_, alnumus_,
			// Assignemnt operators
			simple_assignment_, addition_assignment_, subtraction_assignment_, // -=
			multiplication_assignment_, division_assignment_, remainder_assignment_, // %=
			bitwise_and_assignment_, bitwise_or_assignment_, bitwise_xor_assignment_, // ^=
			left_shift_assignment_, right_shift_assignment_,
			// Increment and decrement operators
			increment_, decrement_,
			// Arithmetic operators
			addition_, subtraction_, multiplication_, division_, remainder_,
			bitwise_NOT_, bitwise_AND_, bitwise_OR_, bitwise_XOR_,
			bitwise_left_shift_, bitwise_right_shift_,
			// Logical
			negation_, logical_AND_, logical_OR_,
			// Comparison
			equal_, not_equal_, less_than_, greater_than_,
			less_than_or_equal_, greater_than_or_equal_, three_way_comparison_,
			// Scopes
			open_scope_, close_scope_, open_list_, close_list_, open_frame_, close_frame_,
			// Special
			eos_, comma_, period_, ellipsis_,
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
	private:
		e_type type_;
		sl_char8_vector_cit beg_;
		sl_char8_vector_cit end_;

		sl_size line_;
		sl_size col_;
	public:
		SL_CX tk() noexcept : type_(e_type::none_), beg_(), end_(), line_(0), col_(0) {}
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
	using tk_vector = sl_vector<tk>;
	using tk_vector_it = tk_vector::iterator;
	using tk_vector_cit = tk_vector::const_iterator;
};