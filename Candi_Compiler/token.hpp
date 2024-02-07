#pragma once
#include "char_traits.hpp"
namespace caoco {
	using size_t = std::size_t;
	using char_t = char_traits::char_t;
	using char_vector = std::vector<char_t>;
	using string_t = std::basic_string<char_t>;

	class Tk {
	public:
		enum class eType : int {
			// Abstract 
			none = -1,
			invalid,
			eof,

			// Base 
			line_comment,
			block_comment,
			string_literal,
			number_literal,
			real_literal,
			octet_literal,
			bit_literal,
			unsigned_literal,
			newline,
			whitespace,
			alnumus,

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
			period, // .
			ellipsis, // ...

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
			print_, // #print
			none_literal_, // #none

			// Modifier Tokens
			public_, // #public
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
			return_, // #ret
			into_ // #into
		};
	private:
		eType type_;
		char_vector::const_iterator beg_; 
		char_vector::const_iterator end_;

		size_t line_;
		size_t col_;
	public:
		constexpr Tk() noexcept : type_(eType::none), beg_(), end_(), line_(0), col_(0) {}
		constexpr Tk(eType type) noexcept : type_(type), beg_(), end_(), line_(0), col_(0) {}
		constexpr Tk(eType type, char_vector::const_iterator beg, char_vector::const_iterator end) noexcept 
			: type_(type), beg_(beg), end_(end), line_(0), col_(0) {}
		constexpr Tk(eType type, char_vector::const_iterator beg, char_vector::const_iterator end, size_t line, size_t col) noexcept 
			: type_(type), beg_(beg), end_(end), line_(line), col_(col) {}
	public:
		constexpr eType type() const noexcept { return type_; }
		constexpr char_vector::const_iterator beg() const noexcept { return beg_; }
		constexpr char_vector::const_iterator end() const noexcept { return end_; }
		constexpr size_t size() const { return std::distance(beg_, end_); }
		constexpr size_t line() const noexcept { return line_; }
		constexpr size_t col() const noexcept { return col_; }

		constexpr string_t literal() const {
			// WARNING: Attempting to check or dereference the end or begin of a token with no literal will result in an exception.
			return string_t(beg_, end_);
		}


		// <@brief> Token Type Check: True if the type of this token is equal to the passed token type.
		constexpr bool type_is(eType type) const noexcept {
			return type_ == type;
		}

		/* <@brief> Equality Operator: Equal if the type and literal string are equal. */
		constexpr bool operator==(const Tk& rhs) const {
			return type_ == rhs.type() && literal() == rhs.literal();
		};
		constexpr bool operator!=(const Tk& rhs) const {
			return !(*this == rhs);
		};
	};
}