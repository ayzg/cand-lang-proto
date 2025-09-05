#pragma once
#include <set>
#include "global_typedef.hpp"

namespace cil {
	struct SyntaxTraits {
		struct DirectiveLiteral {
			static constexpr rstring_view enter = "#enter";
			static constexpr rstring_view start = "#start";
			
			// definitions
			static constexpr rstring_view type = "#type";
			static constexpr rstring_view vari = "#vari";
			static constexpr rstring_view func = "#func";
			static constexpr rstring_view class_ = "#class";

			// control flow
			static constexpr rstring_view if_ = "#if";
			static constexpr rstring_view else_ = "#else";
			static constexpr rstring_view elif_ = "#elif";
			static constexpr rstring_view while_ = "#while";
			static constexpr rstring_view for_ = "#for";
			static constexpr rstring_view forif_ = "#forif";
			static constexpr rstring_view ret = "#return";
			static constexpr rstring_view break_ = "#break";
			static constexpr rstring_view continue_ = "#continue";
			
			// direction control
			static constexpr rstring_view into_ = "#into";
			static constexpr rstring_view goto_ = "#goto";
			static constexpr rstring_view out_ = "#out";
		};

		struct CandiSpecialLiteral {
			static constexpr rstring_view a_int = "$int";
			static constexpr rstring_view a_unsigned = "$unsigned";
			static constexpr rstring_view a_real = "$real";
			static constexpr rstring_view a_char = "$char";
			static constexpr rstring_view a_bool = "$bool";
			static constexpr rstring_view a_void = "$void";
		};

		enum class CandilTypeKind {
			undefined_,
			bool_,
			char_,
			int_,
			unsigned_,
			real_,
		};

		static const std::set<rstring_view> candi_special_literals;
		static const std::set<rstring_view> directive_literals;
	};

	// Special undefined type returned by parser upon attemping 
	// to evaluate an undefined candi special type. typeid(UndefinedType)
	struct UndefinedType {
	};

	const std::set<rstring_view> SyntaxTraits::candi_special_literals = {
		CandiSpecialLiteral::a_int,
			CandiSpecialLiteral::a_unsigned,
			CandiSpecialLiteral::a_real,
			CandiSpecialLiteral::a_char,
			CandiSpecialLiteral::a_bool
	};

	const std::set<rstring_view> SyntaxTraits::directive_literals = {
		DirectiveLiteral::enter,
			DirectiveLiteral::start,
			DirectiveLiteral::type,
			DirectiveLiteral::vari,
			DirectiveLiteral::func,
			DirectiveLiteral::class_,
			DirectiveLiteral::if_,
			DirectiveLiteral::else_,
			DirectiveLiteral::elif_,
			DirectiveLiteral::while_,
			DirectiveLiteral::for_,
			DirectiveLiteral::forif_,
			DirectiveLiteral::ret,
			DirectiveLiteral::break_,
			DirectiveLiteral::continue_,
			DirectiveLiteral::into_,
			DirectiveLiteral::goto_,
			DirectiveLiteral::out_
	};
}