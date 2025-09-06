//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: common
// File: token_scope.h
//---------------------------------------------------------------------------//
// Brief: {description}
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_COMMON_TOKEN_SCOPE_H
#define HEADER_GUARD_CAOCO_COMMON_TOKEN_SCOPE_H
// Includes:
#include "cand_syntax.h"
#include "import_stl.h"
#include "token_cursor.h"
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

class TkScope {
  bool valid_{false};
  TkVectorConstIter begin_;
  TkVectorConstIter end_;
  std::string error_message_;

 public:
  const std::string& Error() const { return error_message_; }
  bool Valid() const { return valid_; }
  // <@method:ContainedEnd> Returns the end of the scope, not including the
  // close token.
  auto ContainedEnd() const { return end_ - 1; }
  // <@method:ContainedBegin> Returns the beginning of the scope, not including
  // the open token.
  auto ContainedBegin() const { return begin_ + 1; }
  // <@method:is_empty> Returns true if the scope is empty.
  auto IsEmpty() const { return ContainedBegin() == ContainedEnd(); }
  // <@method:scope_end> Returns the end of the scope, including the close
  // token.
  auto End() const { return end_; }
  // <@method:scope_begin> Returns the beginning of the scope, including the
  // open token.
  auto Begin() const { return begin_; }

  TkCursor Contained() const {
    return TkCursor{ContainedBegin(), ContainedEnd()};
  }

  constexpr operator bool() const { return valid_; }
  TkScope() = default;
  TkScope(bool valid, TkVectorConstIter begin, TkVectorConstIter end)
      : valid_(valid), begin_(begin), end_(end) {}

  // Methods for determining the start and end of a scope.
  static TkScope find_paren(TkVectorConstIter begin, TkVectorConstIter end) {
    auto paren_scope_depth = 0;
    auto frame_scope_depth = 0;
    auto list_scope_depth = 0;
    std::stack<eTk> scope_type_history;
    TkCursor last_open = {begin, end};
    TkVectorConstIter last_closed = begin;
    TkVectorConstIter error_last_closed = begin;

    if (last_open.AtEnd()) {  // Open is at end therefore cannot be closed.
      auto failure = TkScope{false, begin, end};
      failure.error_message_ =
          "find_paren: Open token is at end of token vector.";
      return failure;
    }

    if (!last_open.TypeIs(
            eTk::kOpenParen)) {  // No open list token to start with.
      auto failure = TkScope{false, begin, end};
      failure.error_message_ =
          "find_paren: Open token is not an open scope token.";
      return failure;
    }

    if (last_open.Next().AtEnd()) {  // End right after open, cannot be closed.
      auto failure = TkScope{false, begin, end};
      failure.error_message_ =
          "find_paren: Open token is at end of token vector.";
      return failure;
    }

    if (last_open.Next().TypeIs(eTk::kCloseParen)) {  // Empty list
      return TkScope{true, begin, last_open.Next(2).Iter()};
    }

    // find the last matching close token that is not within a () [] or {}
    // scope, if there is no matching close token, return false
    for (auto tk_it = (begin + 1); tk_it != end; tk_it++) {
      if (tk_it->TypeIs(eTk::kOpenParen)) {
        paren_scope_depth++;
        scope_type_history.push(eTk::kOpenParen);
      } else if (tk_it->TypeIs(eTk::kCloseParen)) {
        if (scope_type_history.empty()) {
          // This is the end of the scope
          last_closed = tk_it;
          break;
        }

        if (scope_type_history.top() != eTk::kOpenParen) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_paren: Close token ')' without open token '('.";
          return failure;
        }
        scope_type_history.pop();
        paren_scope_depth--;
      } else if (tk_it->TypeIs(eTk::kOpenBracket)) {
        frame_scope_depth++;
        scope_type_history.push(eTk::kOpenBracket);
      } else if (tk_it->TypeIs(eTk::kCloseBracket)) {
        if (scope_type_history.empty()) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_paren: Close token ']' without open token '['.";
          return failure;
        }

        if (scope_type_history.top() != eTk::kOpenBracket) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_paren: Close token ']' without open token '['.";
          return failure;
        }
        scope_type_history.pop();
        frame_scope_depth--;
      } else if (tk_it->TypeIs(eTk::kOpenBrace)) {
        list_scope_depth++;
        scope_type_history.push(eTk::kOpenBrace);
      } else if (tk_it->TypeIs(eTk::kCloseBrace)) {
        if (scope_type_history.empty()) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_paren: Close token '}' without open token '{'.";
          return failure;
        }

        if (scope_type_history.top() != eTk::kOpenBrace) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_paren: Close token '}' without open token '{'.";
          return failure;
        }
        scope_type_history.pop();
        list_scope_depth--;
      }  // end if
      error_last_closed = tk_it;
    }  // end for

    if (paren_scope_depth != 0) {
      auto failure = TkScope{false, begin, last_closed + 1};
      failure.error_message_ =
          "find_paren: Parentheses scope contains mismatched parentheses.";
      return failure;
    }
    if (frame_scope_depth != 0) {
      auto failure = TkScope{false, begin, last_closed + 1};
      failure.error_message_ =
          "find_paren: Parentheses scope contains mismatched frames.";
      return failure;
    }
    if (list_scope_depth != 0) {
      auto failure = TkScope{false, begin, last_closed + 1};
      failure.error_message_ =
          "find_paren: Parentheses scope contains mismatched list scopes.";
      return failure;
    }
    if (last_closed == begin) {
      auto failure = TkScope{false, begin, end};
      failure.error_message_ =
          "find_paren: Parentheses scope at Line:" +
          std::to_string(last_open.Line()) +
          " column:" + std::to_string(last_open.Col()) +
          " was never closed.\n" + "Expected a close token ')' at Line:" +
          std::to_string(error_last_closed->Line()) +
          " column:" + std::to_string(error_last_closed->Col());
      return failure;
    }

    return TkScope{true, begin, last_closed + 1};
  }  // end find_paren
  static TkScope find_paren(TkCursor crsr) {
    return find_paren(crsr.Iter(), crsr.End());
  }
  static TkScope find_brace(TkVectorConstIter begin, TkVectorConstIter end) {
    auto paren_scope_depth = 0;
    auto frame_scope_depth = 0;
    auto list_scope_depth = 0;
    std::stack<eTk> scope_type_history;
    TkCursor last_open = {begin, end};
    TkVectorConstIter last_closed = begin;
    TkVectorConstIter error_last_closed = begin;
    if (last_open.AtEnd()) {  // Open is at end therefore cannot be closed.
      auto failure = TkScope{false, begin, end};
      failure.error_message_ =
          "find_brace: Open token is at end of token vector.";
      return failure;
    }

    if (!last_open.TypeIs(
            eTk::kOpenBrace)) {  // No open list token to start with.
      auto failure = TkScope{false, begin, end};
      failure.error_message_ =
          "find_brace: Open token is not an open list token.";
      return failure;
    }

    if (last_open.Next().AtEnd()) {  // End right after open, cannot be closed.
      auto failure = TkScope{false, begin, end};
      failure.error_message_ =
          "find_brace: Open token is at end of token vector.";
      return failure;
    }

    if (last_open.Next().TypeIs(eTk::kCloseBrace)) {  // Empty list
      return TkScope{true, begin, last_open.Next(2).Iter()};
    }

    // find the last matching close token that is not within a () [] or {}
    // scope, if there is no matching close token, return false
    for (auto tk_it = (begin + 1); tk_it < end; tk_it++) {
      if (tk_it->TypeIs(eTk::kOpenParen)) {
        paren_scope_depth++;
        scope_type_history.push(eTk::kOpenParen);
      } else if (tk_it->TypeIs(eTk::kCloseParen)) {
        if (scope_type_history.empty()) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_brace: Close token ')' without open token '('.";
          return failure;
        }

        if (scope_type_history.top() != eTk::kOpenParen) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_brace: Close token ')' without open token '('.";
          return failure;
        }
        scope_type_history.pop();
        paren_scope_depth--;
      } else if (tk_it->TypeIs(eTk::kOpenBracket)) {
        frame_scope_depth++;
        scope_type_history.push(eTk::kOpenBracket);
      } else if (tk_it->TypeIs(eTk::kCloseBracket)) {
        if (scope_type_history.empty()) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_brace: Close token ']' without open token '['.";
          return failure;
        }

        if (scope_type_history.top() != eTk::kOpenBracket) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_brace: Close token ']' without open token '['.";
          return failure;
        }
        scope_type_history.pop();
        frame_scope_depth--;
      } else if (tk_it->TypeIs(eTk::kOpenBrace)) {
        list_scope_depth++;
        scope_type_history.push(eTk::kOpenBrace);
      } else if (tk_it->TypeIs(eTk::kCloseBrace)) {
        if (scope_type_history.empty()) {
          last_closed = tk_it;
          break;  // This is the end of the scope
        }

        if (scope_type_history.top() != eTk::kOpenBrace) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_brace: Close token '}' without open token '{'.";
          return failure;
        }
        scope_type_history.pop();
        list_scope_depth--;
      }

      error_last_closed = tk_it;
    }

    if (paren_scope_depth != 0) {
      auto failure = TkScope{false, begin, last_closed + 1};
      failure.error_message_ =
          "find_brace: List scope contains mismatched parentheses.";
      return failure;
    }
    if (frame_scope_depth != 0) {
      auto failure = TkScope{false, begin, last_closed + 1};
      failure.error_message_ =
          "find_brace: List scope contains mismatched frames.";
      return failure;
    }
    if (list_scope_depth != 0) {
      auto failure = TkScope{false, begin, last_closed + 1};
      failure.error_message_ =
          "find_brace: List scope contains mismatched list scopes.";
      return failure;
    }
    if (last_closed == begin) {
      auto failure = TkScope{false, begin, end};
      failure.error_message_ =
          "find_brace: List scope at Line:" + std::to_string(last_open.Line()) +
          " column:" + std::to_string(last_open.Col()) +
          " was never closed.\n" + "Expected a close token ']' at Line:" +
          std::to_string(error_last_closed->Line()) +
          " column:" + std::to_string(error_last_closed->Col());
      return failure;
    }

    return TkScope{true, begin, last_closed + 1};
  }
  static TkScope find_brace(TkCursor crsr) {
    return find_brace(crsr.Iter(), crsr.End());
  }
  static TkScope find_bracket(TkVectorConstIter begin, TkVectorConstIter end) {
    auto paren_scope_depth = 0;
    auto frame_scope_depth = 0;
    auto list_scope_depth = 0;
    std::stack<eTk> scope_type_history;
    TkCursor last_open = {begin, end};
    TkVectorConstIter last_closed = begin;
    TkVectorConstIter error_last_closed = begin;

    if (last_open.AtEnd()) {  // Open is at end therefore cannot be closed.
      auto failure = TkScope{false, begin, end};
      failure.error_message_ =
          "find_bracket: Open token is at end of token vector.";
    }

    if (!last_open.TypeIs(
            eTk::kOpenBracket)) {  // No open list token to start with.
      auto failure = TkScope{false, begin, end};
      failure.error_message_ =
          "find_bracket: Open token is not an open frame token.";
      return failure;
    }

    if (last_open.Next().AtEnd()) {  // End right after open, cannot be closed.
      auto failure = TkScope{false, begin, end};
      failure.error_message_ =
          "find_bracket: Open token is at end of token vector.";
      return failure;
    }

    if (last_open.Next().TypeIs(eTk::kCloseBracket)) {  // Empty list
      return TkScope{true, begin, last_open.Next(2).Iter()};
    }

    // find the last matching close token that is not within a () [] or {}
    // scope, if there is no matching close token, return false find the last
    // matching close token that is not within a () [] or {} scope, if there is
    // no matching close token, return false
    for (auto tk_it = (begin + 1); tk_it != end; tk_it++) {
      if (tk_it->TypeIs(eTk::kOpenBracket)) {
        frame_scope_depth++;
        scope_type_history.push(eTk::kOpenBracket);
      } else if (tk_it->TypeIs(eTk::kCloseBracket)) {
        if (scope_type_history.empty()) {
          // This is the end of the scope
          last_closed = tk_it;
          break;
        }

        if (scope_type_history.top() != eTk::kOpenBracket) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_bracket: Close token ']' without open token '['.";
          return failure;
        }
        scope_type_history.pop();
        frame_scope_depth--;
      } else if (tk_it->TypeIs(eTk::kOpenParen)) {
        paren_scope_depth++;
        scope_type_history.push(eTk::kOpenParen);
      } else if (tk_it->TypeIs(eTk::kCloseParen)) {
        if (scope_type_history.empty()) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_bracket: Close token ')' without open token '('.";
          return failure;
        }

        if (scope_type_history.top() != eTk::kOpenParen) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_bracket: Close token ')' without open token '('.";
          return failure;
        }
        scope_type_history.pop();
        paren_scope_depth--;
      } else if (tk_it->TypeIs(eTk::kOpenBrace)) {
        list_scope_depth++;
        scope_type_history.push(eTk::kOpenBrace);
      } else if (tk_it->TypeIs(eTk::kCloseBrace)) {
        if (scope_type_history.empty()) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_bracket: Close token '}' without open token '{'.";
          return failure;
        }

        if (scope_type_history.top() != eTk::kOpenBrace) {
          auto failure = TkScope{false, begin, end};
          failure.error_message_ =
              "find_bracket: Close token '}' without open token '{'.";
          return failure;
        }
        scope_type_history.pop();
        list_scope_depth--;
      }  // end switch

      error_last_closed = tk_it;
    }

    if (paren_scope_depth != 0) {
      auto failure = TkScope{false, begin, last_closed + 1};
      failure.error_message_ =
          "find_bracket: Frame scope contains mismatched parentheses.";
      return failure;
    }
    if (frame_scope_depth != 0) {
      auto failure = TkScope{false, begin, last_closed + 1};
      failure.error_message_ =
          "find_bracket: Frame scope contains mismatched frames.";
      return failure;
    }
    if (list_scope_depth != 0) {
      auto failure = TkScope{false, begin, last_closed + 1};
      failure.error_message_ =
          "find_bracket: Frame scope contains mismatched list scopes.";
      return failure;
    }
    if (last_closed == begin) {
      auto failure = TkScope{false, begin, end};
      failure.error_message_ =
          "find_bracket: Frame scope at Line:" +
          std::to_string(last_open.Line()) +
          " column:" + std::to_string(last_open.Col()) +
          " was never closed.\n" + "Expected a close token ']' at Line:" +
          std::to_string(error_last_closed->Line()) +
          " column:" + std::to_string(error_last_closed->Col());
      return failure;
    }

    return TkScope{true, begin, last_closed + 1};
  }
  static TkScope find_bracket(TkCursor crsr) {
    return find_bracket(crsr.Iter(), crsr.End());
  }

  // Method for extracting a seperated parentheses scope. (<separator>)
  static std::vector<TkScope> find_seperated_paren(TkVectorConstIter begin,
                                                   TkVectorConstIter end,
                                                   eTk separator) {
    std::vector<TkScope> scopes;
    if (begin->Type() != eTk::kOpenParen) {
      scopes.push_back(TkScope{false, begin, end});
      return scopes;
    }
    std::stack<eTk> scope_type_history;
    TkVectorConstIter last_closed = begin;
    begin++;  // Skip the open list token
    for (TkVectorConstIter i = begin; i < end;) {
      if (i->TypeIs(separator) && scope_type_history.empty()) {
        scopes.push_back(TkScope{true, last_closed, i + 1});
        last_closed = i;
      } else if (i->IsOpeningScope()) {
        scope_type_history.push(i->Type());
      } else if (i->IsClosingScope() && !scope_type_history.empty()) {
        if (i->IsClosingScopeOf(scope_type_history.top())) {
          scope_type_history.pop();
        } else {
          scopes.push_back(TkScope{false, i, end});  // Wrong closing scope
          return scopes;
        }
      } else if (i->TypeIs(eTk::kCloseParen) && scope_type_history.empty()) {
        // end of list
        scopes.push_back(TkScope{true, last_closed, i + 1});
        return scopes;
      } else if (i->TypeIs(eTk::kEof)) {
        scopes.push_back(TkScope{false, i, end});  // End of file
        return scopes;
      }
      std::advance(i, 1);
    }
  }
  static std::vector<TkScope> find_seperated_paren(TkCursor crsr,
                                                   eTk separator) {
    return find_seperated_paren(crsr.Iter(), crsr.End(), separator);
  }
  static std::vector<TkScope> find_seperated_paren(TkScope ls, eTk separator) {
    return find_seperated_paren(ls.Begin(), ls.End(), separator);
  }

  // Method for extracting a seperated list scope. {<separator>}
  static std::vector<TkScope> find_seperated_brace(TkVectorConstIter begin,
                                                   TkVectorConstIter end,
                                                   eTk separator) {
    std::vector<TkScope> scopes;
    if (begin->Type() != eTk::kOpenBrace) {
      scopes.push_back(TkScope{false, begin, end});
      return scopes;
    }
    std::stack<eTk> scope_type_history;
    TkVectorConstIter last_closed = begin;
    begin++;  // Skip the open list token
    for (TkVectorConstIter i = begin; i < end;) {
      if (i->TypeIs(separator) && scope_type_history.empty()) {
        scopes.push_back(TkScope{true, last_closed, i + 1});
        last_closed = i;
      } else if (i->IsOpeningScope()) {
        scope_type_history.push(i->Type());
      } else if (i->IsClosingScope() && !scope_type_history.empty()) {
        if (i->IsClosingScopeOf(scope_type_history.top())) {
          scope_type_history.pop();
        } else {
          scopes.push_back(TkScope{false, i, end});  // Wrong closing scope
          return scopes;
        }
      } else if (i->TypeIs(eTk::kCloseBrace) && scope_type_history.empty()) {
        // end of list
        scopes.push_back(TkScope{true, last_closed, i + 1});
        return scopes;
      } else if (i->TypeIs(eTk::kEof)) {
        scopes.push_back(TkScope{false, i, end});  // End of file
        return scopes;
      }
      std::advance(i, 1);
    }
  }
  static std::vector<TkScope> find_seperated_brace(TkCursor crsr,
                                                   eTk separator) {
    return find_seperated_brace(crsr.Iter(), crsr.End(), separator);
  }
  static std::vector<TkScope> find_seperated_brace(TkScope ls, eTk separator) {
    return find_seperated_brace(ls.Begin(), ls.End(), separator);
  }

  // Method for extracting a seperated frame scope. [<separator>]
  static std::vector<TkScope> find_seperated_bracket(TkVectorConstIter begin,
                                                     TkVectorConstIter end,
                                                     eTk separator) {
    std::vector<TkScope> scopes;
    if (begin->Type() != eTk::kOpenBracket) {
      scopes.push_back(TkScope{false, begin, end});
      return scopes;
    }
    std::stack<eTk> scope_type_history;
    TkVectorConstIter last_closed = begin;
    begin++;  // Skip the open list token
    for (TkVectorConstIter i = begin; i < end;) {
      if (i->TypeIs(separator) && scope_type_history.empty()) {
        scopes.push_back(TkScope{true, last_closed, i + 1});
        last_closed = i;
      } else if (i->IsOpeningScope()) {
        scope_type_history.push(i->Type());
      } else if (i->IsClosingScope() && !scope_type_history.empty()) {
        if (i->IsClosingScopeOf(scope_type_history.top())) {
          scope_type_history.pop();
        } else {
          scopes.push_back(TkScope{false, i, end});  // Wrong closing scope
          return scopes;
        }
      } else if (i->TypeIs(eTk::kCloseBracket) && scope_type_history.empty()) {
        // end of list
        scopes.push_back(TkScope{true, last_closed, i + 1});
        return scopes;
      } else if (i->TypeIs(eTk::kEof)) {
        scopes.push_back(TkScope{false, i, end});  // End of file
        return scopes;
      }
      std::advance(i, 1);
    }
  }
  static std::vector<TkScope> find_seperated_bracket(TkCursor crsr,
                                                     eTk separator) {
    return find_seperated_bracket(crsr.Iter(), crsr.End(), separator);
  }
  static std::vector<TkScope> find_seperated_bracket(const TkScope& ls,
                                                     eTk separator) {
    return find_seperated_bracket(ls.Begin(), ls.End(), separator);
  }

  // Open token may NOT be repeated.
  // TODO: refactor to use TkCursor instead of TkVectorConstIter next time you
  // use this. Also make sure it does not throw.
  static TkScope find_statement(eTk open, eTk close, TkVectorConstIter begin,
                                TkVectorConstIter end) {
    auto paren_scope_depth = 0;
    auto frame_scope_depth = 0;
    auto list_scope_depth = 0;
    std::stack<eTk> scope_type_history;
    TkVectorConstIter last_open = begin;
    TkVectorConstIter last_closed = begin;

    if (begin->Type() != open) {
      throw std::runtime_error(
          "find_statement: begin iterator not on an open token.");
    }
    if (std::next(begin)->Type() == close)
      return TkScope{true, begin, begin + 2};  // Empty statement

    // find the last matching close token that is not within a () [] or {}
    // scope, if there is no matching close token, return false
    for (auto it = begin + 1; it < end; it++) {
      if (it->Type() == eTk::kOpenParen) {
        paren_scope_depth++;
        scope_type_history.push(eTk::kOpenParen);
        // currrent_scope_type = eTk::kOpenParen;
      } else if (it->Type() == eTk::kCloseParen) {
        if (scope_type_history.empty() ||
            scope_type_history.top() != eTk::kOpenParen) {
          // Has to be a close or error
          if (it->Type() == close) {
            last_closed = it;
            break;
          } else
            throw std::runtime_error(
                "find_statement: Found a close_scope before an open_scope.");
        }
        scope_type_history.pop();
        paren_scope_depth--;
      } else if (it->Type() == eTk::kOpenBracket) {
        frame_scope_depth++;
        // currrent_scope_type = eTk::kOpenBracket;
        scope_type_history.push(eTk::kOpenBracket);
      } else if (it->Type() == eTk::kCloseBracket) {
        if (scope_type_history.empty() ||
            scope_type_history.top() != eTk::kOpenBracket) {
          // Has to be a close or error
          if (it->Type() == close) {
            last_closed = it;
            break;
          } else
            throw std::runtime_error(
                "find_statement: Found a close frame token before an open "
                "frame token.");
        }
        scope_type_history.pop();
        frame_scope_depth--;
      } else if (it->Type() == eTk::kOpenBrace) {
        list_scope_depth++;
        // currrent_scope_type = eTk::kOpenBrace;
        scope_type_history.push(eTk::kOpenBrace);
      } else if (it->Type() == eTk::kCloseBrace) {
        if (scope_type_history.empty() ||
            scope_type_history.top() != eTk::kOpenBrace) {
          // Has to be a close or error
          if (it->Type() == close) {
            last_closed = it;
            break;
          } else
            throw std::runtime_error(
                "find_statement: Found a close list token before an open list "
                "token.");
        }
        scope_type_history.pop();
        list_scope_depth--;
      }

      if (paren_scope_depth == 0 && frame_scope_depth == 0 &&
          list_scope_depth == 0) {
        if (it->Type() == close) {
          last_closed = it;
          break;
        } else if (it->Type() == open)
          throw std::runtime_error(
              "find_statement: Found a repeated open token before a close "
              "token.");
      }
    }

    // if(last_closed == end)
    //	return TkScope{ false, begin, last_closed + 1 }; // No matching close
    // token

    // NOTE: We are adding 1 to last closed because the end is 1 past the last
    // token.
    if (paren_scope_depth != 0) {
      return TkScope{
          false, begin,
          last_closed + 1};  // No matching close token for parenthesis
    } else if (frame_scope_depth != 0) {
      return TkScope{false, begin,
                     last_closed + 1};  // No matching close token for frame
    } else if (list_scope_depth != 0) {
      return TkScope{false, begin,
                     last_closed + 1};  // No matching close token for list
    } else {
      return TkScope{true, begin, last_closed + 1};
    }

  }  // end find_scope

  // Open token may be repeated.
  // TODO: refactor to use TkCursor instead of TkVectorConstIter next time you
  // use this.
  static TkScope find_open_statement(eTk open, eTk close,
                                     TkVectorConstIter begin,
                                     TkVectorConstIter end) {
    auto paren_scope_depth = 0;
    auto frame_scope_depth = 0;
    auto list_scope_depth = 0;
    // eTk currrent_scope_type = eTk::none_;
    std::stack<eTk> scope_type_history;
    TkVectorConstIter last_open = begin;
    TkVectorConstIter last_closed = begin;

    if (begin + 1 == end)
      return TkScope{false, begin, end + 1};  // Empty statement

    if (begin->Type() != open) {
      throw std::runtime_error(
          "find_statement: begin iterator not on an open token.");
    }

    if (std::next(begin)->Type() == close)
      return TkScope{true, begin, begin + 2};  // Empty statement

    // SPECIAL CASE: if the open token is a list,paren or frame-> set
    // scope_type_history and scope depth of the list,paren or frame.
    if (begin->Type() == eTk::kOpenParen) {
      paren_scope_depth++;
      scope_type_history.push(eTk::kOpenParen);
    } else if (begin->Type() == eTk::kOpenBracket) {
      frame_scope_depth++;
      scope_type_history.push(eTk::kOpenBracket);
    } else if (begin->Type() == eTk::kOpenBrace) {
      list_scope_depth++;
      scope_type_history.push(eTk::kOpenBrace);
    }

    // find the last matching close token that is not within a () [] or {}
    // scope, if there is no matching close token, return false
    for (auto it = begin + 1; it < end; it++) {
      if (it->Type() == eTk::kOpenParen) {
        paren_scope_depth++;
        scope_type_history.push(eTk::kOpenParen);
        // currrent_scope_type = eTk::kOpenParen;
      } else if (it->Type() == eTk::kCloseParen) {
        if (scope_type_history.empty()) {
          return TkScope{false, begin, end};  // Close token without open token
        }

        if (scope_type_history.top() != eTk::kOpenParen) {
          // Has to be a close or error
          if (it->Type() == close) {
            last_closed = it;
            break;
          } else
            throw std::runtime_error(
                "find_statement: Found a close_scope before an open_scope.");
        }
        scope_type_history.pop();
        paren_scope_depth--;
      } else if (it->Type() == eTk::kOpenBracket) {
        frame_scope_depth++;
        // currrent_scope_type = eTk::kOpenBracket;
        scope_type_history.push(eTk::kOpenBracket);
      } else if (it->Type() == eTk::kCloseBracket) {
        if (scope_type_history.top() != eTk::kOpenBracket) {
          // Has to be a close or error
          if (it->Type() == close) {
            last_closed = it;
            break;
          } else
            throw std::runtime_error(
                "find_statement: Found a close frame token before an open "
                "frame token.");
        }
        scope_type_history.pop();
        frame_scope_depth--;
      } else if (it->Type() == eTk::kOpenBrace) {
        list_scope_depth++;
        // currrent_scope_type = eTk::kOpenBrace;
        scope_type_history.push(eTk::kOpenBrace);
      } else if (it->Type() == eTk::kCloseBrace) {
        if (scope_type_history.top() != eTk::kOpenBrace) {
          // Has to be a close or error
          if (it->Type() == close) {
            last_closed = it;
            break;
          } else
            throw std::runtime_error(
                "find_statement: Found a close list token before an open list "
                "token.");
        }
        scope_type_history.pop();
        list_scope_depth--;
      }

      if (paren_scope_depth == 0 && frame_scope_depth == 0 &&
          list_scope_depth == 0) {
        if (it->Type() == close) {
          last_closed = it;
          break;
        }
      }
    }

    // if(last_closed == end)
    //	return TkScope{ false, begin, last_closed + 1 }; // No matching close
    // token

    // NOTE: We are adding 1 to last closed because the end is 1 past the last
    // token.
    if (paren_scope_depth != 0) {
      return TkScope{false, begin, last_closed + 1};
    } else if (frame_scope_depth != 0) {
      return TkScope{false, begin, last_closed + 1};
    } else if (list_scope_depth != 0) {
      return TkScope{false, begin, last_closed + 1};
    } else {
      if (last_closed == begin)  // Expression was never closed
        return TkScope{false, begin, last_closed + 1};
      else
        return TkScope{true, begin, last_closed + 1};
    }

  }  // end find_scope

  // Open token may be repeated.
  // TODO: refactor to use TkCursor instead of TkVectorConstIter next time you
  // use this.
  static TkScope find_open_statement(eTk open, std::vector<eTk> close,
                                     TkVectorConstIter begin,
                                     TkVectorConstIter end) {
    auto paren_scope_depth = 0;
    auto frame_scope_depth = 0;
    auto list_scope_depth = 0;
    // eTk currrent_scope_type = eTk::none_;
    std::stack<eTk> scope_type_history;
    TkVectorConstIter last_open = begin;
    TkVectorConstIter last_closed = begin;

    if (begin + 1 == end)
      return TkScope{false, begin, end + 1};  // Empty statement

    if (begin->Type() != open) {
      throw std::runtime_error(
          "find_statement: begin iterator not on an open token.");
    }

    if (std::any_of(close.begin(), close.end(),
                    [begin](eTk tk) { return std::next(begin)->Type() == tk; }))
      return TkScope{true, begin, begin + 2};  // Empty statement

    // SPECIAL CASE: if the open token is a list,paren or frame-> set
    // scope_type_history and scope depth of the list,paren or frame.
    if (begin->Type() == eTk::kOpenParen) {
      paren_scope_depth++;
      scope_type_history.push(eTk::kOpenParen);
    } else if (begin->Type() == eTk::kOpenBracket) {
      frame_scope_depth++;
      scope_type_history.push(eTk::kOpenBracket);
    } else if (begin->Type() == eTk::kOpenBrace) {
      list_scope_depth++;
      scope_type_history.push(eTk::kOpenBrace);
    }

    // find the last matching close token that is not within a () [] or {}
    // scope, if there is no matching close token, return false
    for (auto it = begin + 1; it < end; it++) {
      if (it->Type() == eTk::kOpenParen) {
        paren_scope_depth++;
        scope_type_history.push(eTk::kOpenParen);
        // currrent_scope_type = eTk::kOpenParen;
      } else if (it->Type() == eTk::kCloseParen) {
        if (scope_type_history.empty()) {
          return TkScope{false, begin, end};  // Close token without open token
        }

        if (scope_type_history.top() != eTk::kOpenParen) {
          // Has to be a close or error
          if (std::any_of(close.begin(), close.end(),
                          [it](eTk tk) { return it->Type() == tk; })) {
            last_closed = it;
            break;
          } else
            throw std::runtime_error(
                "find_statement: Found a close_scope before an open_scope.");
        }
        scope_type_history.pop();
        paren_scope_depth--;
      } else if (it->Type() == eTk::kOpenBracket) {
        frame_scope_depth++;
        // currrent_scope_type = eTk::kOpenBracket;
        scope_type_history.push(eTk::kOpenBracket);
      } else if (it->Type() == eTk::kCloseBracket) {
        if (scope_type_history.top() != eTk::kOpenBracket) {
          // Has to be a close or error
          if (std::any_of(close.begin(), close.end(),
                          [it](eTk tk) { return it->Type() == tk; })) {
            last_closed = it;
            break;
          } else
            throw std::runtime_error(
                "find_statement: Found a close frame token before an open "
                "frame token.");
        }
        scope_type_history.pop();
        frame_scope_depth--;
      } else if (it->Type() == eTk::kOpenBrace) {
        list_scope_depth++;
        // currrent_scope_type = eTk::kOpenBrace;
        scope_type_history.push(eTk::kOpenBrace);
      } else if (it->Type() == eTk::kCloseBrace) {
        if (scope_type_history.top() != eTk::kOpenBrace) {
          // Has to be a close or error
          if (std::any_of(close.begin(), close.end(),
                          [it](eTk tk) { return it->Type() == tk; })) {
            last_closed = it;
            break;
          } else
            throw std::runtime_error(
                "find_statement: Found a close list token before an open list "
                "token.");
        }
        scope_type_history.pop();
        list_scope_depth--;
      }

      if (paren_scope_depth == 0 && frame_scope_depth == 0 &&
          list_scope_depth == 0) {
        if (std::any_of(close.begin(), close.end(),
                        [it](eTk tk) { return it->Type() == tk; })) {
          last_closed = it;
          break;
        }
      }
    }

    // if(last_closed == end)
    //	return TkScope{ false, begin, last_closed + 1 }; // No matching close
    // token

    // NOTE: We are adding 1 to last closed because the end is 1 past the last
    // token.
    if (paren_scope_depth != 0) {
      return TkScope{false, begin, last_closed + 1};
    } else if (frame_scope_depth != 0) {
      return TkScope{false, begin, last_closed + 1};
    } else if (list_scope_depth != 0) {
      return TkScope{false, begin, last_closed + 1};
    } else {
      if (last_closed == begin)  // Expression was never closed
        return TkScope{false, begin, last_closed + 1};
      else
        return TkScope{true, begin, last_closed + 1};
    }

  }  // end find_scope

  // Starts with the begin token which may be repeated, ends with a semicolon_
  // ';'
  static TkScope find_program_statement(TkVectorConstIter begin,
                                        TkVectorConstIter end) {
    return find_open_statement(begin->Type(), eTk::kSemicolon, begin, end);
  }

  static TkScope find_program_statement(TkCursor cursor) {
    return find_open_statement(cursor.Get().Type(), eTk::kSemicolon,
                               cursor.Iter(), cursor.End());
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
// File: token_scope.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_COMMON_TOKEN_SCOPE_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
