#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <limits>
#include <initializer_list>

namespace cil {
	using rstring = std::string;
	using rstring_view = std::string_view;

	template<typename T>
	using rilist = std::initializer_list<T>;
	template<typename T>
	using rvector = std::vector<T>;


	using char_t = char8_t;
	using const_char_t = const char_t;
	using const_char_ptr_t = const_char_t*;
	using const_char_ptr_list_t = std::initializer_list<const_char_ptr_t>;
	using const_char_list = std::initializer_list<const_char_t>;


}