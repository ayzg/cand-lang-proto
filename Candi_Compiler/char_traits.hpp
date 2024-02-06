#pragma once
#include <initializer_list> // std::initializer_list implicit use
#include <array> // std::array 
#include <limits> // std::numeric_limits
#include <algorithm> // std::any_of
#include <string> // std::string
#include <string_view> // std::string_view
#include <vector>
#include <iterator>

namespace char_traits {
	template<typename T>
	using ilist = std::initializer_list<T>;
	template<typename T, size_t N>
	using array = std::array<T, N>;

	using char_t = char8_t;
	using char_limits_t = std::numeric_limits<char_t>;

	static constexpr const auto ALL_CHAR_ARRAY = []() consteval {
		array<char_t, std::numeric_limits<char_t>::max()> arr{};
		auto i = std::numeric_limits<char_t>::min();
		for (auto& c : arr) {
			c = i;
			i++;
		}
		return arr;
	}();

	static constexpr const auto ALL_CHAR_POINTER = [](const array<char_t, char_limits_t::max()>& char_arr) consteval {
		array<const char_t*, std::numeric_limits<char_t>::max()> arr{};
		const char_t* i = char_arr.data();
		for (auto& c : arr) {
			c = i;
			i++;
		}
		return arr;
	}(ALL_CHAR_ARRAY);

	static constexpr ilist<const char_t> ALL_CHARACTERS = ilist<const char_t>(ALL_CHAR_ARRAY.data(), ALL_CHAR_ARRAY.data() + ALL_CHAR_ARRAY.size());
	
	static constexpr ilist<const char_t> ALPHABETIC_CHARACTERS = {
		'a','b','c','d','e','f','g','h','i','j','k','l','m',
		'n','o','p','q','r','s','t','u','v','w','x','y','z',
		'A','B','C','D','E','F','G','H','I','J','K','L','M',
		'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
	};

	static constexpr ilist<const char_t> ALNUMUS_CHARACTERS = {
		'a','b','c','d','e','f','g','h','i','j','k','l','m',
		'n','o','p','q','r','s','t','u','v','w','x','y','z',
		'A','B','C','D','E','F','G','H','I','J','K','L','M',
		'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'_','0','1','2','3','4','5','6','7','8','9'
	};

	static constexpr ilist<const char_t> NUMERIC_CHARACTERS = {
	'0','1','2','3','4','5','6','7','8','9'
	};

	static constexpr ilist<const char_t> CORE_SYMBOL_CHARACTERS = {
		'!','@','#','$','%','^','&','*','-','+','=','{','}','[',']','|','\\',';',':','\'','\"','<','>','?','/','~','`','.',',','(',')','_'
	};

	static constexpr ilist<const char_t> EXTENDED_SYMBOL_CHARACTERS = {
			128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
			144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
			160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
			176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
			192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
			208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
			224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
			240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
	};

	static constexpr ilist<const char_t> SYMBOL_CHARACTERS = {
			'!','@','#','$','%','^','&','*','-','+','=','{','}','[',']','|',
			'\\',';',':','\'','\"','<','>','?','/','~','`','.',',','(',')','_',
			128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
			144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
			160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
			176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
			192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
			208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
			224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
			240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
	};

	static constexpr ilist<const char_t> WHITESPACE_CHARACTERS = {
		' ','\t','\n','\r','\v','\f'
	};

	static constexpr ilist<const char_t> NEWLINE_CHARACTERS = {
		'\n','\r','\v','\f'
	};

	static constexpr ilist<const char_t> CORE_CONTROL_CHARACTERS = {
		'\0','\a','\b','\t','\n','\v','\f','\r','\x1b'
	};

	static constexpr bool is_alpha(char_t c) {
		return std::any_of(ALPHABETIC_CHARACTERS.begin(),
			ALPHABETIC_CHARACTERS.end(),
			[c](const char_t match) { return match == c; });
	}

	static constexpr bool is_numeric(char_t c) {
		return std::any_of(NUMERIC_CHARACTERS.begin(),
			NUMERIC_CHARACTERS.end(),
			[c](const char_t match) { return match == c; });
	}

	static constexpr bool is_underscore(char_t c) {
		return c == '_';
	}

	static constexpr bool is_alnumus(char_t c) {
		return is_alpha(c) || is_numeric(c) || is_underscore(c);
	}

	static constexpr bool is_symbol(char_t c) {
		return std::any_of(SYMBOL_CHARACTERS.begin(),
			SYMBOL_CHARACTERS.end(),
			[c](const char_t match) { return match == c; });
	}

	static constexpr bool is_core_symbol(char_t c) {
		return std::any_of(CORE_SYMBOL_CHARACTERS.begin(),
			CORE_SYMBOL_CHARACTERS.end(),
			[c](const char_t match) { return match == c; });
	}

	static constexpr bool is_extended_symbol(char_t c) {
		return std::any_of(EXTENDED_SYMBOL_CHARACTERS.begin(),
			EXTENDED_SYMBOL_CHARACTERS.end(),
			[c](const char_t match) { return match == c; });
	}

	static constexpr bool is_printable_space(char_t c) {
		return c == ' ' || c == '\t';
	}

	static constexpr bool is_printable(char_t c) {
		return is_alpha(c) || is_numeric(c) || is_core_symbol(c) || is_printable_space(c);
	}

	static constexpr bool is_whitespace(char_t c) {
		return std::any_of(WHITESPACE_CHARACTERS.begin(),
			WHITESPACE_CHARACTERS.end(),
			[c](const char_t match) { return match == c; });
	}

	static constexpr bool is_newline(char_t c) {
		return std::any_of(NEWLINE_CHARACTERS.begin(),
			NEWLINE_CHARACTERS.end(),
			[c](const char_t match) { return match == c; });
	}

	static constexpr bool is_core_control(char_t c) {
		return std::any_of(CORE_CONTROL_CHARACTERS.begin(),
			CORE_CONTROL_CHARACTERS.end(),
			[c](const char_t match) { return match == c; });
	}
};
