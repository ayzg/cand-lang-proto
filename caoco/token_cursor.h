//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: common
// File: token_cursor.h
//---------------------------------------------------------------------------//
// Brief: Iterator for a std::vector<Tk> with:
//    - Bounds checking, returns a default eof token on out-of-bounds access.
//    - Direct access to the current token's methods through the cursor.
//    - Member methods for advance, next, peek, advance_to, jump_to(next to).
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_COMMON_TOKEN_CURSOR_H
#define HEADER_GUARD_CAOCO_COMMON_TOKEN_CURSOR_H
// Includes:
#include "cand_syntax.h"
#include "import_stl.h"
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

static const Tk sentinel_end_token = Tk(eTk::kEof);
class TkCursor {
  TkVectorConstIter beg_;
  TkVectorConstIter end_;
  TkVectorConstIter it_;

 public:
  // Properties
  constexpr TkVectorConstIter End() const { return end_; }
  constexpr TkVectorConstIter Begin() const { return beg_; }
  constexpr TkVectorConstIter Iter() const { return it_; }
  constexpr const Tk& Get() const {
    if (it_ >= end_) {
      return sentinel_end_token;
    }
    return *it_;
  }
  constexpr bool AtEnd() const { return (it_ == end_) || (it_->TypeIs(eTk::kEof)); }
  constexpr const Tk& operator->() const { return Get(); }

  // Iteration
  // advances the cursor by n.
  constexpr TkCursor& Advance(int n = 1) {
    if (n != 0) {
      if (it_ + n >= end_) {
        it_ = end_;
      } else if (it_ + n < beg_) {
        it_ = beg_;
      } else {
        it_ += n;
      }
    }
    return *this;
  }

  // advances the cursor to the new_cursor. Checks that
  // cursor is within beg and end.
  constexpr TkCursor& Advance(TkVectorConstIter new_it) {
    if (new_it < beg_) {
      throw std::out_of_range("tk_cursor passed advance_to outside of begin.");
    } else if (new_it > end_) {
      throw std::out_of_range("tk_cursor passed advance_to outside of end.");
    } else
      it_ = new_it;
    return *this;
  }

  // Specialized Advance for use in the lark_parser.h.
  // Shortcut for accessing .Always().Iter() from an InternalParseResult.
  // before. c.Advance(result.Always().Iter());
  // use. c.Advance(result);
  constexpr TkCursor& Advance(const PartialExpected<Ast, TkCursor>& result) {
    return Advance(result.Always().Iter());
  }

  // returns cursor advanced by N. N may be negative.
  TkCursor Next(int n = 1) const {
    TkCursor next_cursor = *this;
    next_cursor.Advance(n);
    return next_cursor;
  }
  TkCursor Next(TkVectorConstIter new_it) const {
    if (new_it < beg_) {
      throw std::out_of_range("tk_cursor passed advance_to outside of begin.");
    } else if (new_it > end_) {
      throw std::out_of_range("tk_cursor passed advance_to outside of end.");
    } else {
      TkCursor rt = *this;
      rt.it_ = new_it;
      return rt;
    }
  }

  // returns the token at the cursor + n.
  constexpr const Tk& Peek(int n = 0) const { return Next(n).Get(); }

  // True there is a match in the iterator's range.
  // Starting from and including the current token.
  constexpr auto find_forward(TkVector match) {
    auto end = std::next(it_, match.size());
    auto found = std::search(
        it_, end, match.begin(), match.end(),
        [](const Tk& a, const Tk& b) { return a.Type() == b.Type(); });

    if (found != end) {
      return true;
    }
    return false;
  }

  // Returns true if the next n tokens match the
  // match vector. Starting from and including the current token.
  constexpr bool find_forward_exact(TkVectorConstIter cursor, TkVector match) {
    auto end = std::next(cursor, static_cast<std::ptrdiff_t>(match.size()));
    if (std::equal(match.begin(), match.end(), cursor, end)) {
      return true;
    }
    return false;
  }

  // Token Properties
  constexpr inline auto Type() const noexcept { return Get().Type(); }
  constexpr inline auto Size() const { return Get().Size(); }
  constexpr inline auto Line() const noexcept { return Get().Line(); }
  constexpr inline auto Col() const noexcept { return Get().Col(); }
  constexpr inline const std::string& Literal() const {
    return Get().Literal();
  }
  constexpr inline auto Priority() const { return Get().Priority(); }
  constexpr inline auto Assoc() const { return Get().Assoc(); }
  constexpr inline auto Operation() const { return Get().Operation(); }
  constexpr inline auto TypeIs(eTk type) const noexcept {
    return Get().TypeIs(type);
  }
    constexpr inline auto TypeIsnt(eTk type) const noexcept {
    return not(Get().TypeIs(type));
  }
  constexpr inline auto TypeAndLitIs(eTk kind,
                                     const std::string& literal) const {
    return Get().TypeAndLitIs(kind, literal);
  }
  constexpr inline auto IsKeyword() const noexcept { return Get().IsKeyword(); }
  constexpr inline auto IsModifierKeyword() const noexcept {
    return Get().IsModifierKeyword();
  }
constexpr inline auto IsDeclarativeKeyword() const noexcept {
    return Get().IsDeclarativeKeyword();
  }
  constexpr inline auto IsSingularOperand() const noexcept {
    return Get().IsSingularOperand();
  }
  constexpr bool IsSingularPrefixOperator() const noexcept{
    return Get().IsSingularPrefixOperator();
  }
  constexpr inline auto IsOpeningScope() const noexcept {
    return Get().IsOpeningScope();
  }
  constexpr inline auto IsClosingScope() const noexcept {
    return Get().IsClosingScope();
  }
  constexpr inline auto IsClosingScopeOf(eTk open) const {
    return Get().IsClosingScopeOf(open);
  }
  constexpr inline auto NodeType() const noexcept { return Get().NodeType(); }

  constexpr inline auto IsPrimaryExpressionOpening() const noexcept {
    const Tk& c = Get();
    return c.IsSingularOperand() || c.IsSingularPrefixOperator() ||
           c.TypeIs(eTk::kOpenParen);
  }

 public:
  TkCursor() = default;
  TkCursor(TkVectorConstIter begin, TkVectorConstIter end)
      : beg_(begin), end_(end), it_(begin) {}
  TkCursor(TkVectorConstIter begin, TkVectorConstIter end, TkVectorConstIter it)
      : beg_(begin), end_(end), it_(it) {}
  TkCursor(const TkCursor& other)
      : beg_(other.beg_), end_(other.end_), it_(other.it_) {}
  TkCursor& operator=(const TkCursor& other) {
    beg_ = other.beg_;
    end_ = other.end_;
    it_ = other.it_;
    return *this;
  }
};

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: common
// File: token_cursor.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_COMMON_TOKEN_CURSOR_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
