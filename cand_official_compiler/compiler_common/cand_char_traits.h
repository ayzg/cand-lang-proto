//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: cand_official_compiler
// Directory: compiler_common
// File: cand_char_traits.h
//---------------------------------------------------------------------------//
// Brief: Character properties specific to the context of the C& compiler.
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAND_OFFICIAL_COMPILER_COMPILER_COMMON_CAND_CHAR_TRAITS_H
#define HEADER_GUARD_CAND_OFFICIAL_COMPILER_COMPILER_COMMON_CAND_CHAR_TRAITS_H
// Includes:
#include "castd.h"
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
namespace cand_char {
constexpr unsigned char n_chars = std::numeric_limits<unsigned char>::min() -
                                  std::numeric_limits<unsigned char>::max();

//static constexpr inline auto const ALL_CHAR_ARRAY = []() consteval {
//  std::array<char, std::numeric_limits<char>::max()> arr{};
//  auto i = CharLimits::min();
//  for (auto& c : arr) {
//    c = i;
//    i++;
//  }
//  return arr;
//}();
//
//static constexpr inline const auto ALL_CHAR_POINTER =
//    [](const std::array<char, CharLimits::max()>& char_arr) consteval {
//      std::array<const char*, CharLimits::max()> arr{};
//      const char* i = char_arr.data();
//      for (auto& c : arr) {
//        c = i;
//        i++;
//      }
//      return arr;
//    }(ALL_CHAR_ARRAY);
//
//static constexpr inline ConstCharInitializerList ALL_CHARACTERS = ConstCharInitializerList(
//    ALL_CHAR_ARRAY.data(), ALL_CHAR_ARRAY.data() + ALL_CHAR_ARRAY.size());
//
//static constexpr inline ConstCharInitializerList ALPHABETIC_CHARACTERS = {
//    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
//    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
//    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
//    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
//
//static constexpr inline ConstCharInitializerList ALNUMUS_CHARACTERS = {
//    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
//    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
//    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
//    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
//    '_', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
//
//static constexpr inline ConstCharInitializerList NUMERIC_CHARACTERS = {'0', '1', '2', '3', '4',
//                                              '5', '6', '7', '8', '9'};
//
//static constexpr inline ConstCharInitializerList CORE_SYMBOL_CHARACTERS = {
//    '!', '@', '#', '$', '%', '^',  '&', '*', '-',  '+',  '=',
//    '{', '}', '[', ']', '|', '\\', ';', ':', '\'', '\"', '<',
//    '>', '?', '/', '~', '`', '.',  ',', '(', ')',  '_'};
//
//
//static constexpr inline ConstCharInitializerList SYMBOL_CHARACTERS = {
//    '!', '@', '#',  '$', '%', '^',  '&',  '*', '-', '+', '=', '{', '}', '[',
//    ']', '|', '\\', ';', ':', '\'', '\"', '<', '>', '?', '/', '~', '`', '.',
//    ',', '(', ')',  '_'};
//
//static constexpr inline ConstCharInitializerList WHITESPACE_CHARACTERS = {' ',  '\t', '\n',
//                                                 '\r', '\v', '\f'};
//
//static constexpr inline ConstCharInitializerList NEWLINE_CHARACTERS = {'\n', '\r', '\v', '\f'};
//
//static constexpr inline ConstCharInitializerList CORE_CONTROL_CHARACTERS = {
//    '\0', '\a', '\b', '\t', '\n', '\v', '\f', '\r', '\x1b'};
//
//static constexpr inline bool is_alpha(char c) {
//  return std::any_of(ALPHABETIC_CHARACTERS.begin(), ALPHABETIC_CHARACTERS.end(),
//                    [c](const char match) { return match == c; });
//}
//
//static constexpr inline bool is_numeric(char c) {
//  return std::any_of(NUMERIC_CHARACTERS.begin(), NUMERIC_CHARACTERS.end(),
//                    [c](const char match) { return match == c; });
//}
//
//static constexpr inline bool is_underscore(char c) { return c == '_'; }
//
//static constexpr inline bool is_alnum(char c) { return is_alpha(c) || is_numeric(c); }
//
//static constexpr inline bool is_alus(char c) { return is_alpha(c) || is_underscore(c); }
//
//static constexpr inline bool is_alnumus(char c) {
//  return is_alpha(c) || is_numeric(c) || is_underscore(c);
//}
//
//static constexpr inline bool is_symbol(char c) {
//  return std::any_of(SYMBOL_CHARACTERS.begin(), SYMBOL_CHARACTERS.end(),
//                    [c](const char match) { return match == c; });
//}
//
//static constexpr inline bool is_core_symbol(char c) {
//  return std::any_of(CORE_SYMBOL_CHARACTERS.begin(),
//                    CORE_SYMBOL_CHARACTERS.end(),
//                    [c](const char match) { return match == c; });
//}
//
//static constexpr inline bool is_printable_space(char c) { return c == ' ' || c == '\t'; }
//
//static constexpr inline bool is_printable(char c) {
//  return is_alpha(c) || is_numeric(c) || is_core_symbol(c) ||
//         is_printable_space(c);
//}
//
//static constexpr inline bool is_whitespace(char c) {
//  return std::any_of(WHITESPACE_CHARACTERS.begin(), WHITESPACE_CHARACTERS.end(),
//                    [c](const char match) { return match == c; });
//}
//
//static constexpr inline bool is_newline(char c) {
//  return std::any_of(NEWLINE_CHARACTERS.begin(), NEWLINE_CHARACTERS.end(),
//                    [c](const char match) { return match == c; });
//}
//
//static constexpr inline bool is_core_control(char c) {
//  return std::any_of(CORE_CONTROL_CHARACTERS.begin(),
//                    CORE_CONTROL_CHARACTERS.end(),
//                    [c](const char match) { return match == c; });
//}
};  // namespace cand_char

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: cand_official_compiler
// Directory: compiler_common
// File: cand_char_traits.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAND_OFFICIAL_COMPILER_COMPILER_COMMON_CAND_CHAR_TRAITS_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
