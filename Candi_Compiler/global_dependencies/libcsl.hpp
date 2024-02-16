#pragma once
#include "libstd_types.hpp"


namespace caoco {

	// A cool trick which allows for strings to be passed as template parameters using C++20 lambdas
	#define LAMBDA_STRING(str) []() consteval { return #str; }

	// Expected
	template <typename ExpectedT, typename AlwaysT>
	class sl_partial_expected {
		AlwaysT always_;
		sl_opt<ExpectedT> expected_{ sl::nullopt };
		sl_string error_message_{ "" };

		template <typename ExpectedT, typename AlwaysT>
		sl_partial_expected(AlwaysT always, ExpectedT expected) : always_(always), expected_(expected) {}

		template <typename AlwaysT>
		sl_partial_expected(AlwaysT always) : always_(always) {}

	public:

		SL_CXS auto make_success(AlwaysT always, ExpectedT expected) {
			return sl_partial_expected(always, expected);
		}

		SL_CXS auto make_failure(AlwaysT always, const sl_string& error_message) {
			auto ret = sl_partial_expected(always);
			ret.error_message_ = error_message;
			return ret;
		}

		SL_CXS auto make_failure_chain(sl_partial_expected other, AlwaysT always, const sl_string& error_message) {
			auto ret = sl_partial_expected(other.always());
			ret.error_message_ = other.error_message_ + error_message;
			return ret;
		}


		constexpr bool valid() const {
			return expected_.has_value();
		}

		constexpr const auto& expected() const {
			return expected_.value();
		}

		constexpr const auto& always()  const {
			return always_;
		}

		constexpr const auto& error_message() const {
			return error_message_;
		}

		constexpr auto chain_failure(AlwaysT always, sl_string error_message) {
			return sl_partial_expected::make_failure_chain(*this, always, error_message);
		}
	};

	namespace sl {
		// Converts a string_t utf 8 string to a std::string char string
		SL_CXIN sl_string to_str(const sl_u8string& str) 
		{
			
			return sl_string(str.begin(), str.end());
		}

		// Converts a string_t utf 8 string to a vector of chars
		SL_CXIN sl_char8_vector to_char8_vector(const sl_u8string& str) {
			return sl_char8_vector(str.begin(), str.end());
		}

		// Converts a regular string literal to a utf 8 char vector
		SL_CXIN sl_char8_vector to_char8_vector(const char* str) {
			return sl_char8_vector(str, str + sl_char_traits<char>::length(str));
		}

		// Convert u8 string to a vector of char_8t
		sl_vector<char8_t> to_u8vec(const char8_t* str) {
			sl_vector<char8_t> vec;
			for (int i = 0; str[i] != '\0'; i++) {
				vec.push_back(str[i]);
			}
			return vec;
		}

		sl_vector<char8_t> to_u8vec(const char* str) {
			sl_vector<char8_t> vec;
			for (int i = 0; str[i] != '\0'; i++) {
				vec.push_back(str[i]);
			}
			return vec;
		}

		// Loads a file into a vector of chars
		sl_char8_vector load_file_to_char8_vector(sl_string name) {


			std::ifstream ifs(name, std::ios::binary | std::ios::ate);

			if (!ifs)
				throw std::runtime_error(name + ": " + std::strerror(errno));

			auto end = ifs.tellg();
			ifs.seekg(0, std::ios::beg);

			auto size = std::size_t(end - ifs.tellg());

			if (size == 0) // avoid undefined behavior 
				return {};

			std::vector<std::byte> buffer(size);

			if (!ifs.read((char*)buffer.data(), buffer.size()))
				throw std::runtime_error(name + ": " + std::strerror(errno));

			// Convert the vector of chars to a vector of char_t
			sl_char8_vector chars;
			chars.reserve(buffer.size());
			for (auto& c : buffer) {
				chars.push_back(static_cast<char8_t>(c));
			}

			// if the last char is not a null terminator, add one
			if (chars.back() != '\0')
				chars.push_back('\0');

			return chars;
		}

	}
}