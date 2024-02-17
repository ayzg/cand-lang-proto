#pragma once
#include "global_dependencies.hpp"
namespace caoco {
	namespace char_traits {
		using const_char_ilist = sl_ilist<const char8_t>;
		using char_limits = sl_limits<char8_t>;

		SL_CXS auto const ALL_CHAR_ARRAY = []() SL_CE {
			sl_array<char8_t, char_limits::max()> arr{};
			auto i = char_limits::min();
			for (auto& c : arr) {
				c = i;
				i++;
			}
			return arr;
		}();

		SL_CXS const auto ALL_CHAR_POINTER = [](const sl_array<char8_t, char_limits::max()>& char_arr) SL_CE {
			sl_array<const char8_t*, char_limits::max()> arr{};
			const char8_t* i = char_arr.data();
			for (auto& c : arr) {
				c = i;
				i++;
			}
			return arr;
		}(ALL_CHAR_ARRAY);

		SL_CXS const_char_ilist ALL_CHARACTERS = const_char_ilist(ALL_CHAR_ARRAY.data(), ALL_CHAR_ARRAY.data() + ALL_CHAR_ARRAY.size());

		SL_CXS const_char_ilist ALPHABETIC_CHARACTERS = {
			'a','b','c','d','e','f','g','h','i','j','k','l','m',
			'n','o','p','q','r','s','t','u','v','w','x','y','z',
			'A','B','C','D','E','F','G','H','I','J','K','L','M',
			'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
		};

		SL_CXS const_char_ilist ALNUMUS_CHARACTERS = {
			'a','b','c','d','e','f','g','h','i','j','k','l','m',
			'n','o','p','q','r','s','t','u','v','w','x','y','z',
			'A','B','C','D','E','F','G','H','I','J','K','L','M',
			'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
			'_','0','1','2','3','4','5','6','7','8','9'
		};

		SL_CXS const_char_ilist NUMERIC_CHARACTERS = {
		'0','1','2','3','4','5','6','7','8','9'
		};

		SL_CXS const_char_ilist CORE_SYMBOL_CHARACTERS = {
			'!','@','#','$','%','^','&','*','-','+','=','{','}','[',']','|','\\',';',':','\'','\"','<','>','?','/','~','`','.',',','(',')','_'
		};

		SL_CXS const_char_ilist EXTENDED_SYMBOL_CHARACTERS = {
				128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
				144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
				160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
				176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
				192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
				208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
				224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
				240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
		};

		SL_CXS const_char_ilist SYMBOL_CHARACTERS = {
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

		SL_CXS const_char_ilist WHITESPACE_CHARACTERS = {
			' ','\t','\n','\r','\v','\f'
		};

		SL_CXS const_char_ilist NEWLINE_CHARACTERS = {
			'\n','\r','\v','\f'
		};

		SL_CXS const_char_ilist CORE_CONTROL_CHARACTERS = {
			'\0','\a','\b','\t','\n','\v','\f','\r','\x1b'
		};

		SL_CXS bool is_alpha(char8_t c) {
			return sl::any_of(ALPHABETIC_CHARACTERS.begin(),
				ALPHABETIC_CHARACTERS.end(),
				[c](const char8_t match) { return match == c; });
		}

		SL_CXS bool is_numeric(char8_t c) {
			return sl::any_of(NUMERIC_CHARACTERS.begin(),
				NUMERIC_CHARACTERS.end(),
				[c](const char8_t match) { return match == c; });
		}

		SL_CXS bool is_underscore(char8_t c) {
			return c == '_';
		}

		SL_CXS bool is_alnum(char8_t c) {
			return is_alpha(c) || is_numeric(c);
		}

		SL_CXS bool is_alus(char8_t c) {
			return is_alpha(c) || is_underscore(c);
		}

		SL_CXS bool is_alnumus(char8_t c) {
			return is_alpha(c) || is_numeric(c) || is_underscore(c);
		}

		SL_CXS bool is_symbol(char8_t c) {
			return sl::any_of(SYMBOL_CHARACTERS.begin(),
				SYMBOL_CHARACTERS.end(),
				[c](const char8_t match) { return match == c; });
		}

		SL_CXS bool is_core_symbol(char8_t c) {
			return sl::any_of(CORE_SYMBOL_CHARACTERS.begin(),
				CORE_SYMBOL_CHARACTERS.end(),
				[c](const char8_t match) { return match == c; });
		}

		SL_CXS bool is_extended_symbol(char8_t c) {
			return sl::any_of(EXTENDED_SYMBOL_CHARACTERS.begin(),
				EXTENDED_SYMBOL_CHARACTERS.end(),
				[c](const char8_t match) { return match == c; });
		}

		SL_CXS bool is_printable_space(char8_t c) {
			return c == ' ' || c == '\t';
		}

		SL_CXS bool is_printable(char8_t c) {
			return is_alpha(c) || is_numeric(c) || is_core_symbol(c) || is_printable_space(c);
		}

		SL_CXS bool is_whitespace(char8_t c) {
			return sl::any_of(WHITESPACE_CHARACTERS.begin(),
				WHITESPACE_CHARACTERS.end(),
				[c](const char8_t match) { return match == c; });
		}

		SL_CXS bool is_newline(char8_t c) {
			return sl::any_of(NEWLINE_CHARACTERS.begin(),
				NEWLINE_CHARACTERS.end(),
				[c](const char8_t match) { return match == c; });
		}

		SL_CXS bool is_core_control(char8_t c) {
			return sl::any_of(CORE_CONTROL_CHARACTERS.begin(),
				CORE_CONTROL_CHARACTERS.end(),
				[c](const char8_t match) { return match == c; });
		}
	};
};