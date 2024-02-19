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
			string_literal_, number_literal_, real_literal_, byte_literal_,
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
			atype_, aidentity_, avalue_, aint_, auint_, areal_, abyte_,
			abit_, astr_,aarray_, apointer_, amemory_, afunction_,
			// Directive Tokens
			enter_, start_, include_,type_,use_, var_, class_, func_, print_, none_literal_,
			obj_,private_,macro_,
			// Modifier Tokens
			public_, const_, static_, ref_,
			// Control Flow Tokens
			if_, else_, elif_, while_, for_, switch_, case_, default_, break_,
			continue_, return_, into_,on_
		};
	private:
		e_type type_;
		sl_char8_vector_cit beg_;
		sl_char8_vector_cit end_;
		sl_u8string literal_;

		sl_size line_;
		sl_size col_;
	public:
		SL_CX tk() noexcept : type_(e_type::none_), beg_(), end_(), line_(0), col_(0), literal_(){}
		SL_CX tk(e_type type) noexcept : type_(type), beg_(), end_(), line_(0), col_(0),literal_() {}
		SL_CX tk(e_type type, sl_char8_vector_cit beg, sl_char8_vector_cit end) noexcept
			: type_(type), beg_(beg), end_(end), line_(0), col_(0) {
			literal_ = sl_u8string(beg_, end_);
		}
		SL_CX tk(e_type type, sl_char8_vector_cit beg, sl_char8_vector_cit end, sl_size line, sl_size col) noexcept
			: type_(type), beg_(beg), end_(end), line_(line), col_(col) {
			literal_ = sl_u8string(beg_, end_);
		}
	public:
		SL_CX e_type type() const noexcept { return type_; }
		SL_CX sl_char8_vector_cit beg() const noexcept { return beg_; }
		SL_CX sl_char8_vector_cit end() const noexcept { return end_; }
		SL_CX sl_size size() const { return std::distance(beg_, end_); }
		SL_CX sl_size line() const noexcept { return line_; }
		SL_CX sl_size col() const noexcept { return col_; }
		SL_CX const sl_u8string & literal() const {
			// WARNING: Attempting to check or dereference the end or begin of a token with no literal will result in an exception.
			return literal_;
			//return sl_u8string(beg_, end_);
		}
		SL_CX sl_string literal_str() const {
			// WARNING: Attempting to check or dereference the end or begin of a token with no literal will result in an exception.
			//return sl::to_str(sl_u8string(beg_, end_));
			return sl::to_str(literal_);
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


		// Fast type queries.
		SL_CX bool is_directive() const noexcept {
			//atype_, aidentity_, avalue_, aint_, auint_, areal_, abyte_,
			//	abit_, astr_, aarray_, apointer_, amemory_, afunction_,
			//	enter_, start_, include_, type_, var_, class_, func_, print_, none_literal_,
			//	obj_, private_, macro_,
			//	public_, const_, static_, ref_,
			//	if_, else_, elif_, while_, for_, switch_, case_, default_, break_,
			//	continue_, return_, into_, on_
			return type_ == e_type::atype_ ||
				type_ == e_type::aidentity_ ||
				type_ == e_type::avalue_ ||
				type_ == e_type::aint_ ||
				type_ == e_type::auint_ ||
				type_ == e_type::areal_ ||
				type_ == e_type::abyte_ ||
				type_ == e_type::abit_ ||
				type_ == e_type::astr_ ||
				type_ == e_type::aarray_ ||
				type_ == e_type::apointer_ ||
				type_ == e_type::amemory_ ||
				type_ == e_type::afunction_ ||
				type_ == e_type::enter_ ||
				type_ == e_type::start_ ||
				type_ == e_type::include_ ||
				type_ == e_type::type_ ||
				type_ == e_type::var_ ||
				type_ == e_type::class_ ||
				type_ == e_type::func_ ||
				type_ == e_type::print_ ||
				type_ == e_type::none_literal_ ||
				type_ == e_type::obj_ ||
				type_ == e_type::private_ ||
				type_ == e_type::macro_ ||
				type_ == e_type::public_ ||
				type_ == e_type::const_ ||
				type_ == e_type::static_ ||
				type_ == e_type::ref_ ||
				type_ == e_type::if_ ||
				type_ == e_type::else_ ||
				type_ == e_type::elif_ ||
				type_ == e_type::while_ ||
				type_ == e_type::for_ ||
				type_ == e_type::switch_ ||
				type_ == e_type::case_ ||
				type_ == e_type::default_ ||
				type_ == e_type::break_ ||
				type_ == e_type::continue_ ||
				type_ == e_type::return_ ||
				type_ == e_type::into_ ||
				type_ == e_type::on_;
		}


		// debug only
		static constexpr caoco::sl_string type_to_string(e_type type) {
			switch (type) {
			case(e_type::none_): return "none";
			case(e_type::invalid_): return "invalid";
			case(e_type::eof_): return "eof";
			case(e_type::line_comment_): return "line_comment";
			case(e_type::block_comment_): return "block_comment";
			case(e_type::string_literal_): return "string_literal";
			case(e_type::number_literal_): return "number_literal";
			case(e_type::real_literal_): return "real_literal";
			case(e_type::newline_): return "newline";
			case(e_type::whitespace_): return "whitespace";
			case(e_type::alnumus_): return "alnumus";
			case(e_type::simple_assignment_): return "simple_assignment";
			case(e_type::addition_assignment_): return "addition_assignment";
			case(e_type::subtraction_assignment_): return "subtraction_assignment";
			case(e_type::multiplication_assignment_): return "multiplication_assignment";
			case(e_type::division_assignment_): return "division_assignment";
			case(e_type::remainder_assignment_): return "remainder_assignment";
			case(e_type::bitwise_and_assignment_): return "bitwise_and_assignment";
			case(e_type::bitwise_or_assignment_): return "bitwise_or_assignment";
			case(e_type::bitwise_xor_assignment_): return "bitwise_xor_assignment";
			case(e_type::left_shift_assignment_): return "left_shift_assignment";
			case(e_type::right_shift_assignment_): return "right_shift_assignment";
			case(e_type::increment_): return "increment";
			case(e_type::decrement_): return "decrement";
			case(e_type::addition_): return "addition";
			case(e_type::subtraction_): return "subtraction";
			case(e_type::multiplication_): return "multiplication";
			case(e_type::division_): return "division";
			case(e_type::remainder_): return "remainder";
			case(e_type::bitwise_NOT_): return "bitwise_NOT";
			case(e_type::bitwise_AND_): return "bitwise_AND";
			case(e_type::bitwise_OR_): return "bitwise_OR";
			case(e_type::bitwise_XOR_): return "bitwise_XOR";
			case(e_type::bitwise_left_shift_): return "bitwise_left_shift";
			case(e_type::bitwise_right_shift_): return "bitwise_right_shift";
			case(e_type::negation_): return "negation";
			case(e_type::logical_AND_): return "logical_AND";
			case(e_type::logical_OR_): return "logical_OR";
			case(e_type::equal_): return "equal";
			case(e_type::not_equal_): return "not_equal";
			case(e_type::less_than_): return "less_than";
			case(e_type::greater_than_): return "greater_than";
			case(e_type::less_than_or_equal_): return "less_than_or_equal";
			case(e_type::greater_than_or_equal_): return "greater_than_or_equal";
			case(e_type::three_way_comparison_): return "three_way_comparison";
			case(e_type::open_scope_): return "open_scope";
			case(e_type::close_scope_): return "close_scope";
			case(e_type::open_list_): return "open_list";
			case(e_type::close_list_): return "close_list";
			case(e_type::open_frame_): return "open_frame";
			case(e_type::close_frame_): return "close_frame";
			case(e_type::eos_): return "eos";
			case(e_type::comma_): return "comma";
			case(e_type::period_): return "period";
			case(e_type::ellipsis_): return "ellipsis";
			case(e_type::atype_): return "atype_";
			case(e_type::aidentity_): return "aidentity_";
			case(e_type::avalue_): return "avalue_";
			case(e_type::aint_): return "aint_";
			case(e_type::auint_): return "auint_";
			case(e_type::areal_): return "areal_";
				//case(e_type::aureal_): return "aureal_";
			case(e_type::abyte_): return "abyte_";
			case(e_type::abit_): return "abit_";
			case(e_type::aarray_): return "aarray_";
			case(e_type::apointer_): return "apointer_";
			case(e_type::amemory_): return "amemory_";
			case(e_type::afunction_): return "afunction_";
			case(e_type::enter_): return "enter_";
			case(e_type::start_): return "start_";
			case(e_type::type_): return "type_";
			case(e_type::var_): return "var_";
			case(e_type::class_): return "class_";
			case(e_type::func_): return "func_";
			case(e_type::print_): return "print_";
			case(e_type::public_): return "public_";
			case(e_type::const_): return "const_";
			case(e_type::static_): return "static_";
			case(e_type::ref_): return "ref_";
			case(e_type::if_): return "if_";
			case(e_type::else_): return "else_";
			case(e_type::elif_): return "elif_";
			case(e_type::while_): return "while_";
			case(e_type::for_): return "for_";
			case(e_type::switch_): return "switch_";
			case(e_type::case_): return "case_";
			case(e_type::default_): return "default_";
			case(e_type::break_): return "break_";
			case(e_type::continue_): return "continue_";
			case(e_type::return_): return "return_";
			case(e_type::into_): return "into_";
			case(e_type::none_literal_): return "none_literal_";
			case(e_type::bit_literal_): return "bit_literal";
			case(e_type::byte_literal_): return "octet_literal";
			case(e_type::unsigned_literal_): return "unsigned_literal";
			case(e_type::obj_): return "obj_";
			case(e_type::private_): return "private_";
			case(e_type::macro_): return "macro_";
			case(e_type::on_): return "on_";
			case(e_type::include_): return "include_";
			case(e_type::astr_): return "astr_";
			default: return "This token type is not string-convertible. Please implement a string conversion for this token type in the token_type_to_string function in test.cpp.";
			}
		}
		SL_CX caoco::sl_string type_to_string() const {
			return tk::type_to_string(type_);
		}
	};

	using tk_enum = tk::e_type;
	using tk_vector = sl_vector<tk>;
	using tk_vector_it = tk_vector::iterator;
	using tk_vector_cit = tk_vector::const_iterator;
};