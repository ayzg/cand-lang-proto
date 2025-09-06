//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: compiler
// File: lexer.h
//---------------------------------------------------------------------------//
// Brief: {description}
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_COMPILER_LEXER_H
#define HEADER_GUARD_CAOCO_COMPILER_LEXER_H
// Includes:
#include "cand_char_traits.h"
#include "cand_syntax.h"
#include "compiler_error.h"
#include "expected.h"
#include "import_stl.h"

//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

using CharVector = std::vector<char>;
using CharVectorConstIter = CharVector::const_iterator;
class Lexer {
  // Constants
  static constexpr char kEofChar = grammar::characters::kEofile::value;

 public:
  using LexMethodResult = PartialExpected<Tk, CharVectorConstIter>;
  using LexerResult = Expected<TkVector>;

 private:
  constexpr inline LexMethodResult SuccessResult(eTk type,
                                                 CharVectorConstIter beg_it,
                                                 CharVectorConstIter end_it);
  constexpr inline LexMethodResult NoneResult(CharVectorConstIter beg_it);
  constexpr inline LexMethodResult FailureResult(CharVectorConstIter beg_it,
                                                 const std::string& error);

  // Members
  CharVectorConstIter beg_;
  CharVectorConstIter end_;

  // Lexer's Utility functions
  constexpr bool NotAtEof(CharVectorConstIter it) const { return it != end_; }
  constexpr char Get(CharVectorConstIter it) const;
  constexpr char Peek(CharVectorConstIter it, int n);

  constexpr bool FindForward(CharVectorConstIter it,
                             std::string_view characters) const;

  constexpr CharVectorConstIter& Advance(CharVectorConstIter& it, int n = 1);

  // Lexers
  constexpr LexMethodResult LexSolidus(CharVectorConstIter it);
  constexpr LexMethodResult LexQuotation(CharVectorConstIter it);
  constexpr LexMethodResult LexNewline(CharVectorConstIter it);
  constexpr LexMethodResult LexWhitespace(CharVectorConstIter it);
  constexpr LexMethodResult LexEof(CharVectorConstIter it);
  constexpr LexMethodResult LexNumber(CharVectorConstIter it);
  constexpr LexMethodResult LexIdentifier(CharVectorConstIter it);
  constexpr LexMethodResult LexDirective(CharVectorConstIter it);
  constexpr LexMethodResult LexKeyword(CharVectorConstIter it);
  constexpr LexMethodResult LexOperator(CharVectorConstIter it);
  constexpr LexMethodResult LexScopes(CharVectorConstIter it);
  constexpr LexMethodResult LexSemicolon(CharVectorConstIter it);
  constexpr LexMethodResult LexColon(CharVectorConstIter it);
  constexpr LexMethodResult LexComma(CharVectorConstIter it);
  constexpr LexMethodResult LexPeriod(CharVectorConstIter it);

  constexpr LexerResult Lex();

 public:
  constexpr explicit Lexer(CharVectorConstIter beg, CharVectorConstIter end)
      : beg_(beg), end_(end) {}
  constexpr LexerResult operator()() {
    // Check for empty input
    if (beg_ == end_) {
      return LexerResult::Failure("Empty input");
    }
    return Lex();
  }

  // Util static methods for easy lexing of vectors or strings
  static constexpr inline LexerResult Lex(CharVectorConstIter beg,
                                          CharVectorConstIter end) {
    Lexer lexer(beg, end);
    return lexer();
  }

  static constexpr inline LexerResult Lex(const CharVector& input) {
    Lexer lexer(input.cbegin(), input.cend());
    return lexer();
  }

  static constexpr inline LexerResult Lex(const std::string& input) {
    CharVector input_vec(input.cbegin(), input.cend());
    Lexer lexer(input_vec.cbegin(), input_vec.cend());
    return lexer();
  }
};
// Lexer's Utility methods
constexpr inline Lexer::LexMethodResult Lexer::SuccessResult(
    eTk type, CharVectorConstIter beg_it, CharVectorConstIter end_it) {
  return LexMethodResult::Success(end_it, Tk(type, beg_it, end_it));
}

constexpr inline Lexer::LexMethodResult Lexer::NoneResult(
    CharVectorConstIter beg_it) {
  return LexMethodResult::Success(beg_it, Tk(eTk::kNone, beg_it, beg_it));
}

constexpr inline Lexer::LexMethodResult Lexer::FailureResult(
    CharVectorConstIter beg_it, const std::string& error) {
  return LexMethodResult::Failure(beg_it, error);
}

constexpr char Lexer::Get(CharVectorConstIter it) const {
  // kEofChar if it is anything but a valid iterator
  if (it >= end_) return kEofChar;
  if (it < beg_) return kEofChar;
  return *it;
}

constexpr char Lexer::Peek(CharVectorConstIter it, int n) {
  if (std::distance(it, end_) < n) return kEofChar;  // Out of bounds cant Peek
  return Get(it + n);
}

constexpr bool Lexer::FindForward(CharVectorConstIter it,
                                  std::string_view characters) const {
  // Searches forward for a complete match of characters. Starting from it,
  // inclusive.
  if (std::distance(it, end_) < static_cast<std::ptrdiff_t>(characters.size()))
    return false;  // Out of bounds cant match
  auto end = std::next(it, static_cast<std::ptrdiff_t>(characters.size()));
  if (std::equal(it, end, characters.cbegin(), characters.cend())) return true;
  return false;
}

constexpr CharVectorConstIter& Lexer::Advance(CharVectorConstIter& it, int n) {
  // No checks performed. Use with caution.
  std::advance(it, n);
  return it;
}

// Lexers
constexpr Lexer::LexMethodResult Lexer::LexSolidus(CharVectorConstIter it) {
  using namespace grammar::characters;
  CharVectorConstIter begin = it;
  if (Get(it) == kDiv::value) {
    if (Peek(it, 1) == kDiv::value &&
        Peek(it, 2) !=
            kDiv::value) {  // Line comment two solidus '//' closed by '\n'
      while (!cand_char::IsNewline(Get(it)) && Get(it) != kEofChar) {
        Advance(it);
      }
      return SuccessResult(eTk::kLineComment, begin, it);
    } else if (Peek(it, 1) == kDiv::value &&
               Peek(it, 2) == kDiv::value) {  // Block comment three solidus
                                              // '///' closed by '///'
      Advance(it, 3);
      while (!FindForward(it, grammar::kBlockComment::value)) {
        Advance(it);
      }
      Advance(it, 3);
      /* Cursor is at end of block,
              1. move to next character, (1)
              2. Advance past block comment closing(3)
              3. then go 1 past to the end.(1)
              Total: 5
      */
      return SuccessResult(eTk::kBlockComment, begin, it);
    } else {
      Advance(it);
      // if the next character is a '=' then we have a division assignment
      // operator
      if (Get(it) == kEq::value) {
        Advance(it);
        return SuccessResult(eTk::kDivisionAssignment, begin, it);
      }
      // otherwise we have a division operator
      else
        return SuccessResult(eTk::kDivision, begin, it);
    }
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexQuotation(CharVectorConstIter it) {
  using namespace grammar::characters;
  CharVectorConstIter begin = it;
  if (Get(it) == kApostrophe::value) {
    Advance(it);

    while (
        !(Get(it) == kApostrophe::value && Peek(it, -1) != kBacklash::value)) {
      Advance(it);
    }
    Advance(it);

    // Check for byte literal
    if (Get(it) == u8'c') {
      Advance(it);
      return SuccessResult(eTk::kByteLiteral, begin, it);
    } else
      return SuccessResult(eTk::kStringLiteral, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexNewline(CharVectorConstIter it) {
  CharVectorConstIter begin = it;
  if (NotAtEof(it) && cand_char::IsNewline(it)) {
    while (NotAtEof(it) && cand_char::IsNewline(it)) {
      Advance(it);
    }
    return SuccessResult(eTk::kNewline, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexWhitespace(CharVectorConstIter it) {
  CharVectorConstIter begin = it;
  if (cand_char::IsWhitespace(Get(it))) {
    while (cand_char::IsWhitespace(Get(it))) {
      Advance(it);
    }
    return SuccessResult(eTk::kWhitespace, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexEof(CharVectorConstIter it) {
  CharVectorConstIter begin = it;
  if (Get(it) == kEofChar) {
    Advance(it);
    return SuccessResult(eTk::kEof, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexNumber(CharVectorConstIter it) {
  using namespace grammar;
  CharVectorConstIter begin = it;
  if (cand_char::IsNumeric(Get(it))) {
    // Special case for 1b and 0b
    if (Get(it) == '1' && Peek(it, 1) == 'b') {
      Advance(it, 2);
      return SuccessResult(eTk::kBoolLiteral, begin, it);
    } else if (Get(it) == '0' && Peek(it, 1) == 'b') {
      Advance(it, 2);
      return SuccessResult(eTk::kBoolLiteral, begin, it);
    }

    while (cand_char::IsNumeric(Get(it))) {
      Advance(it);
    }

    // Special case for unsigned literal (overflow is handled by the parser)
    if (Get(it) == 'u') {
      Advance(it);
      return SuccessResult(eTk::kUnsignedLiteral, begin, it);
    }

    // Special case for byte literal(overflow is handled by the parser)
    if (Get(it) == 'c') {
      Advance(it);
      return SuccessResult(eTk::kByteLiteral, begin, it);
    }

    // If number is followed by elipsis. Return the number.
    if (FindForward(it, scopes::kEllipsis::value)) {
      return SuccessResult(eTk::kNumberLiteral, begin, it);
    }

    // Else process a floating literal.
    if (Get(it) == characters::kPeriod::value) {
      Advance(it);
      while (cand_char::IsNumeric(Get(it))) {
        Advance(it);
      }
      return SuccessResult(eTk::kDoubleLiteral, begin, it);
    }

    return SuccessResult(eTk::kNumberLiteral, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexIdentifier(CharVectorConstIter it) {
  CharVectorConstIter begin = it;
  if (cand_char::IsAlpha(Get(it))) {
    while (cand_char::IsAlnumus(Get(it))) {
      Advance(it);
    }
    return SuccessResult(eTk::kIdentifier, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexDirective(CharVectorConstIter it) {
  CharVectorConstIter beg = it;
  if (Get(it) == grammar::characters::kHash::value) {
    Advance(it);
    while (NotAtEof(it) && cand_char::IsAlnumus(it)) {
      Advance(it);
    }

    LexMethodResult temp_result =
        FailureResult(beg, "Invalid directive:" + std::string(beg, it));
    std::apply(
        [&](const auto&... traits) -> void {
          [](...) {}((
              [&]<typename T>(const T& x) {
                if (FindForward(beg, std::decay_t<T>::literal)) {
                  temp_result = SuccessResult(std::decay_t<T>::type, beg, it);
                }
              }(std::forward<decltype(traits)>(traits)),
              false)...);
        },
        tk_traits::kAllDirectivesTuple);
    return temp_result;
  } else {
    return NoneResult(beg);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexKeyword(CharVectorConstIter it) {
  CharVectorConstIter beg = it;
  if (cand_char::IsAlpha(it)) {
    Advance(it);
    while (NotAtEof(it) && cand_char::IsAlnumus(it)) {
      Advance(it);
    }
    // What is this black magic??
    // https://stackoverflow.com/questions/43541241/iterate-over-tuple-elements-with-stdapply
    LexMethodResult temp_result = NoneResult(beg);
    std::apply(
        [&](const auto&... traits) -> void {
          [](...) {}((
              [&]<typename T>(const T& x) {
                if (FindForward(beg, std::decay_t<T>::literal)) {
                  temp_result = SuccessResult(std::decay_t<T>::type, beg, it);
                }
              }(std::forward<decltype(traits)>(traits)),
              false)...);
        },
        tk_traits::kAllKeywordsTuple);
    return temp_result;

  } else {
    return NoneResult(beg);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexOperator(CharVectorConstIter it) {
  using namespace grammar::characters;
  CharVectorConstIter begin = it;
  if (Get(it) == kEq::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kEqual, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kSimpleAssignment, begin, it);
    }
  } else if (Get(it) == kAdd::value) {
    if (Peek(it, 1) == kAdd::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kIncrement, begin, it);
    } else if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kAdditionAssignment, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kAddition, begin, it);
    }
  } else if (Get(it) == kSub::value) {
    if (Peek(it, 1) == kSub::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kDecrement, begin, it);
    } else if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kSubtractionAssignment, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kSubtraction, begin, it);
    }
  } else if (Get(it) == kMul::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kMultiplicationAssignment, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kMultiplication, begin, it);
    }
  } else if (Get(it) == kDiv::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kDivisionAssignment, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kDivision, begin, it);
    }
  } else if (Get(it) == kMod::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kRemainderAssignment, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kRemainder, begin, it);
    }
  } else if (Get(it) == kAnd::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kBitwiseAndAssignment, begin, it);
    } else if (Peek(it, 1) == kAnd::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kLogicalAnd, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kBitwiseAnd, begin, it);
    }
  } else if (Get(it) == kOr::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kBitwiseOrAssignment, begin, it);
    } else if (Peek(it, 1) == kOr::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kLogicalOr, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kBitwiseOr, begin, it);
    }
  } else if (Get(it) == kXor::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kBitwiseXorAssignment, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kBitwiseXor, begin, it);
    }
  } else if (Get(it) == kLsh::value) {
    if (Peek(it, 1) == kLsh::value) {
      if (Peek(it, 2) == kEq::value) {
        Advance(it, 3);
        return SuccessResult(eTk::kLeftShiftAssignment, begin, it);
      } else {
        Advance(it, 2);
        return SuccessResult(eTk::kBitwiseLeftShift, begin, it);
      }
    } else if (Peek(it, 1) == kEq::value) {
      if (Peek(it, 2) == kRsh::value) {
        Advance(it, 3);
        return SuccessResult(eTk::kThreeWayComparison, begin, it);
      }
      Advance(it, 2);
      return SuccessResult(eTk::kLessThanOrEqual, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kLessThan, begin, it);
    }
  } else if (Get(it) == kRsh::value) {
    if (Peek(it, 1) == kRsh::value) {
      if (Peek(it, 2) == kEq::value) {
        Advance(it, 3);
        return SuccessResult(eTk::kRightShiftAssignment, begin, it);
      } else {
        Advance(it, 2);
        return SuccessResult(eTk::kBitwiseRightShift, begin, it);
      }
    } else if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kGreaterThanOrEqual, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kGreaterThan, begin, it);
    }
  } else if (Get(it) == kNot::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kNotEqual, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kNegation, begin, it);
    }
  } else if (Get(it) == kTilde::value) {
    Advance(it);
    return SuccessResult(eTk::kBitwiseNot, begin, it);
  } else if (Get(it) == kCommercialAt::value) {
    Advance(it);
    return SuccessResult(eTk::kCommercialAt, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexScopes(CharVectorConstIter it) {
  using namespace grammar::characters;
  CharVectorConstIter begin = it;
  if (Get(it) == kLparen::value) {
    Advance(it);
    return SuccessResult(eTk::kOpenParen, begin, it);
  } else if (Get(it) == kRparen::value) {
    Advance(it);
    return SuccessResult(eTk::kCloseParen, begin, it);
  } else if (Get(it) == kLbrace::value) {
    Advance(it);
    return SuccessResult(eTk::kOpenBrace, begin, it);
  } else if (Get(it) == kRbrace::value) {
    Advance(it);
    return SuccessResult(eTk::kCloseBrace, begin, it);
  } else if (Get(it) == kLbracket::value) {
    Advance(it);
    return SuccessResult(eTk::kOpenBracket, begin, it);
  } else if (Get(it) == kRbracket::value) {
    Advance(it);
    return SuccessResult(eTk::kCloseBracket, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexSemicolon(CharVectorConstIter it) {
  CharVectorConstIter begin = it;
  if (Get(it) == grammar::characters::kSemicolon::value) {
    Advance(it);
    return SuccessResult(eTk::kSemicolon, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexColon(CharVectorConstIter it) {
  CharVectorConstIter begin = it;
  if (Get(it) == grammar::characters::kColon::value) {
    Advance(it);
    if (Get(it) == grammar::characters::kColon::value) {
      Advance(it);
      return SuccessResult(eTk::kDoubleColon, begin, it);
    }
    return SuccessResult(eTk::kColon, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexComma(CharVectorConstIter it) {
  CharVectorConstIter begin = it;
  if (Get(it) == grammar::characters::kComma::value) {
    Advance(it);
    return SuccessResult(eTk::kComma, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexPeriod(CharVectorConstIter it) {
  CharVectorConstIter begin = it;
  if (FindForward(it, grammar::scopes::kEllipsis::value)) {
    Advance(it, 3);
    return SuccessResult(eTk::kEllipsis, begin, it);
  } else if (Get(it) == grammar::characters::kPeriod::value) {
    Advance(it);
    return SuccessResult(eTk::kPeriod, begin, it);
  } else {
    return NoneResult(begin);
  }
}

// Main tokenizer method
constexpr Lexer::LexerResult Lexer::Lex() {
  CharVectorConstIter it = beg_;
  TkVector output_tokens;
  std::size_t current_line = 1;
  std::size_t current_col = 1;

  // Lambda for executing a lexer and updating the iterator.
  lambda xPerformLex = [&](auto lexer) constexpr -> Expected<bool> {
  // Disable warning for uninitialized variable, it is initialized above.
#pragma warning(disable : 6001)
    LexMethodResult lex_result = (this->*lexer)(it);
#pragma warning(default : 6001)
    if (!lex_result.Valid()) {
      return Expected<bool>::Failure(lex_result.Error());
    }
    Tk result_token = lex_result.Value();
    CharVectorConstIter result_end = lex_result.Always();

    if (result_token.Type() == eTk::kNone) {  // No match, try next lexer
      return Expected<bool>::Success(false);
    }

    else {  // Lexing was successful
      // Update position based on the number of characters consumed
      current_line += std::count(it, result_end, '\n');

      // Find the last newline before the current character
      CharVectorConstIter last_newline =
          std::find(std::reverse_iterator(result_end),
                    std::reverse_iterator(it), '\n')
              .base();
      // If there is no newline before the current character, use the start of
      // the string
      if (last_newline == end_) {
        last_newline = beg_;
      }

      // Calculate the character index within the line
      current_col = static_cast<std::size_t>(std::distance(
          last_newline, result_end));  // Will always be a positive number

      // Set the line and col of the resulting token and emplace it into the
      // output vector
      result_token.SetLine(current_line);
      result_token.SetCol(current_col);
      output_tokens.push_back(result_token);
      it = result_end;  // Advance the iterator to the end of lexing. Note lex
                        // end and token end may differ.
      return Expected<bool>::Success(true);
    }
  }; // end xPerformLex

  // Attempt to lex a token using one of the lexers until one succeeds. If none
  // succeed, report error. Order of lexers is important. For example, the
  // identifier lexer will match keywords, so it must come after the keyword
  // lexer.
  while (it != end_) {
    bool match = false;
    for (auto lexer :
         {&Lexer::LexSolidus, &Lexer::LexQuotation, &Lexer::LexNewline,
          &Lexer::LexWhitespace, &Lexer::LexEof, &Lexer::LexKeyword,
          &Lexer::LexDirective, &Lexer::LexNumber, &Lexer::LexIdentifier,
          &Lexer::LexOperator, &Lexer::LexScopes, &Lexer::LexSemicolon,
          &Lexer::LexColon, &Lexer::LexComma, &Lexer::LexPeriod}) {
      auto lex_result = xPerformLex(lexer);
      if (!lex_result.Valid()) {  // Error inside one of the lexers
        return LexerResult::Failure(
            compiler_error::tokenizer::xLexerSyntaxError(
                current_line, current_col, Get(it), lex_result.Error()));
      } else if (lex_result.Value()) {
        // Note: The iterator 'it' is advanced in perform_lex lambda.
        match = true;
        break;  // Exit for-loop
      }
    }

    if (!match) {  // None of the lexers matched, report an error
      return LexerResult::Failure(compiler_error::tokenizer::xInvalidChar(
          current_line, current_col, Get(it)));
    }
  } // end while

  // Remove redundant tokens after lexing
  // Note: "i" is used instead of "it" to avoid ambiguity with the iterator
  // above.
  TkVector sanitized = [&]() constexpr {
    TkVector new_output;
    for (auto i = output_tokens.cbegin(); i != output_tokens.cend(); ++i) {
      const std::initializer_list<eTk> REDUNDANT_TOKEN_KINDS{
          eTk::kWhitespace, eTk::kLineComment, eTk::kBlockComment,
          eTk::kNewline};

      if (std::any_of(REDUNDANT_TOKEN_KINDS.begin(),
                      REDUNDANT_TOKEN_KINDS.end(),
                      [i](eTk match) { return match == i->Type(); })) {
        continue;
      } else {  // Push back non-redundant tokens
        new_output.push_back(*i);
      }
    }
    return new_output;
  }();  // Note: The lambda is immediately called.

  return LexerResult::Success(sanitized);
}  // end tokenize

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: compiler
// File: lexer.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_COMPILER_LEXER_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//