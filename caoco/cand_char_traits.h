//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: common
// File: cand_char_traits.h
//---------------------------------------------------------------------------//
// Brief: Character properties specific to the context of the C& compiler.
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_COMMON_CAND_CHAR_TRAITS_H
#define HEADER_GUARD_CAOCO_COMMON_CAND_CHAR_TRAITS_H
// Includes:
#include "import_stl.h"
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
namespace cand_char {
using ConstCharInitializerList = std::initializer_list<char>;
using CharLimits = std::numeric_limits<unsigned char>;

static constexpr inline ConstCharInitializerList kAlphaChars = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

static constexpr inline ConstCharInitializerList kAlnumusChars = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '_', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

static constexpr inline ConstCharInitializerList kNumericChars = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

static constexpr inline ConstCharInitializerList kSymbolChars = {
    '!', '@', '#', '$', '%', '^',  '&', '*', '-',  '+',  '=',
    '{', '}', '[', ']', '|', '\\', ';', ':', '\'', '\"', '<',
    '>', '?', '/', '~', '`', '.',  ',', '(', ')',  '_'};

static constexpr inline ConstCharInitializerList kWhiteSpaceChars =
    {' ', '\n', '\r', '\v', '\f'};

static constexpr inline ConstCharInitializerList kNewLineChars = {
    '\n', '\r', '\v', '\f'};

static constexpr inline ConstCharInitializerList kCoreControlChars = {
    '\0', '\a', '\b', '\t', '\n', '\v', '\f', '\r', '\x1b'};

static constexpr inline bool IsAlpha(char c) {
  switch (c) {
    case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':
    case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't':
    case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':
    case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':
    case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T':
    case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':
      return true;
    default:
      return false;
      break;
  }
}

static constexpr inline bool IsNumeric(char c) {
  switch (c) {
    case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
      return true;
    default:
      return false;
      break;
  }
}

static constexpr inline bool IsUnderscore(char c) { return c == '_'; }

static constexpr inline bool IsAlphanumeric(char c) {
  return IsAlpha(c) || IsNumeric(c);
}

static constexpr inline bool IsAlphaUnderscore(char c) {
  return IsAlpha(c) || IsUnderscore(c);
}

static constexpr inline bool IsAlnumus(char c) {
  return IsAlpha(c) || IsNumeric(c) || IsUnderscore(c);
}

static constexpr inline bool IsSymbol(char c) {
  switch (c) {
    //'!', '@', '#', '$', '%', '^', '&', '*', '-', '+', '=', '{', '}', '[', ']',
    //    '|', '\\', ';', ':', '\'', '\"', '<', '>', '?', '/', '~', '`', '.', ',',
    //    '(', ')', '_'
    case '!':case '@':case '#':case '$':case '%':case '^':case '&':case '*':case '-':case '+':
    case '=':case '{':case '}':case '[':case ']':case '|':case '\\':case ';':case ':':case '\'':
    case '\"':case '<':case '>':case '?':case '/':case '~':case '`':case '.':case ',':case '(':
    case ')':case '_':
      return true;
    default:
      return false;
      break;
  }
}

static constexpr inline bool IsCoreSymbol(char c) { 
  return IsSymbol(c); // temp same as symbols 
}

static constexpr inline bool IsPrintableSpace(char c) {
  return c == ' ' || c == '\t';
}

static constexpr inline bool IsPrintable(char c) {
  return IsAlpha(c) || IsNumeric(c) || IsCoreSymbol(c) ||
         IsPrintableSpace(c);
}

static constexpr inline bool IsWhitespace(char c) {
  // ' ', '\n', '\r', '\v', '\f'
  switch (c) {
    case ' ':
    case '\n':
    case '\r':
    case '\v':
    case '\f':
    case '\t':
      return true;
    default:
      return false;
      break;
  }
}

static constexpr inline bool IsNewline(char c) {
  //  '\n', '\r', '\v', '\f'
  switch (c) {
    case '\n':case '\r':case '\v':case '\f':
      return true;
    default:
      return false;
      break;
  }
}

static constexpr inline bool IsCoreControl(char c) {
  // '\0', '\a', '\b', '\t', '\n', '\v', '\f', '\r', '\x1b'
  switch (c) {
    case '\0':case '\a':case '\b':case '\t':case '\n':case '\v':case '\f':case '\r':case '\x1b':
      return true;
    default:
      return false;
      break;
  }
}

// Overloads for iterators (use with caution, bounds checking is not performed)
// - used in the lexer
static constexpr inline bool IsAlpha(std::vector<char>::const_iterator c) {
  return IsAlpha(*c);
}

static constexpr inline bool IsNumeric(std::vector<char>::const_iterator c) {
  return IsNumeric(*c);
}

static constexpr inline bool IsUnderscore(std::vector<char>::const_iterator c) {
  return IsUnderscore(*c);
}

static constexpr inline bool IsAlphanumeric(
    std::vector<char>::const_iterator c) {
  return IsAlphanumeric(*c);

}

static constexpr inline bool IsAlphaUnderscore(
    std::vector<char>::const_iterator c) {
  return IsAlphaUnderscore(*c);

}

static constexpr inline bool IsAlnumus(std::vector<char>::const_iterator c) {
  return IsAlnumus(*c);
}

static constexpr inline bool IsSymbol(std::vector<char>::const_iterator c) {
  return IsSymbol(*c);
}

static constexpr inline bool IsCoreSymbol(std::vector<char>::const_iterator c) {
  return IsCoreSymbol(*c);
}

static constexpr inline bool IsPrintableSpace(
    std::vector<char>::const_iterator c) {
  return IsPrintableSpace(*c);
}

static constexpr inline bool IsPrintable(std::vector<char>::const_iterator c) {
  return IsPrintable(*c);
}

static constexpr inline bool IsWhitespace(std::vector<char>::const_iterator c) {
  return IsWhitespace(*c);
}

static constexpr inline bool IsNewline(std::vector<char>::const_iterator c) {
  return IsNewline(*c);
}

static constexpr inline bool IsCoreControl(
    std::vector<char>::const_iterator c) {
  return IsCoreControl(*c);
}
};  // namespace cand_char

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: common
// File: cand_char_traits.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_COMMON_CAND_CHAR_TRAITS_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
