#pragma once
#include "libstd_types.hpp"


namespace caoco {

	namespace sl {
		// Converts a string_t utf 8 string to a std::string char string
		constexpr inline sl_string to_str(const sl_u8string& str) {
			return sl_string(str.begin(), str.end());
		}

		// Converts a string_t utf 8 string to a vector of chars
		constexpr inline sl_char8_vector to_char8_vector(const sl_u8string& str) {
			return sl_char8_vector(str.begin(), str.end());
		}

		// Converts a regular string literal to a utf 8 char vector
		constexpr inline sl_char8_vector to_char8_vector(const char* str) {
			return sl_char8_vector(str, str + sl_char_traits<char>::length(str));
		}

		// Loads a file into a vector of chars
		sl_char8_vector load_file_to_char8_vector(sl_string name) {
			sl_vector<char> bytes;
			std::ifstream file(name, std::ios::binary);
			file.seekg(0, std::ios::end);
			auto size = file.tellg();
			file.seekg(0, std::ios::beg);
			bytes.resize(size);
			file.read(bytes.data(), size);
			file.close();

			// if the last char is not a null terminator, add one
			if (bytes.back() != '\0')
				bytes.push_back('\0');

			// Convert the vector of chars to a vector of char_t
			sl_char8_vector chars;
			chars.reserve(bytes.size());
			for (auto& c : bytes) {
				chars.push_back(static_cast<char8_t>(c));
			}
			return chars;
		}

	}
}