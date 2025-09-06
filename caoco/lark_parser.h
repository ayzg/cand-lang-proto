//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: compiler
// File: lark_parser.h
//---------------------------------------------------------------------------//
// Brief: Left to Right Recursive Right Closure Look-Ahead(k) Parser
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_COMPILER_LARK_PARSER_H
#define HEADER_GUARD_CAOCO_COMPILER_LARK_PARSER_H
// Includes:
#include "cand_syntax.h"
#include "compiler_error.h"
#include "expected.h"
#include "import_stl.h"
#include "token_closure.h"
#include "token_cursor.h"
#include "token_scope.h"

//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

class LarkParser {
  using InternalParseResult = PartialExpected<Ast, TkCursor>;
  static constexpr inline InternalParseResult Success(TkCursor crs,
                                                      const Ast& nd) {
    return InternalParseResult::Success(crs, nd);
  }
  static constexpr inline InternalParseResult Failure(TkCursor crs,
                                                      const std::string& err) {
    return InternalParseResult::Failure(crs, err);
  }

 public:
  //---------------------------------------------------------------------------//
  // Internal Closure Parsers:
  // Note these do not take the entire source as an argument.
  // Only pass the current scope to be parser.
  // The end of TkCursor may or may not be end of source.
  // These methods do not advance the cursor or return a new start point.
  //---------------------------------------------------------------------------//

  //------------------------------------//
  // Primary Value Expression Parser:
  // Use ClosureParsePrimaryExpr to parse.
  // Cursor begin and end must be the start and end of the expression.
  //------------------------------------//
  class RParseValueExpression {
    enum class eNextExpectedHeadToken { kOperative, kOperator, kNone };
    static const Tk kOpenParenTk;
    static const Tk kCloseParenTk;
    //  Expected token type on next iteration.
    eNextExpectedHeadToken next_expected_head_token_ = {
        eNextExpectedHeadToken::kOperative};
    ClosureBuffer closure_buffer_;
    bool is_first_operator_ = {true};
    bool is_resolved_ = {false};

    // Set is_first_operator_ to false if currently true, else do nothing.
    void FirstOperatorSwitch() {
      if (is_first_operator_) {
        is_first_operator_ = false;
      }
    }

    inline void ResolvePrefix() {
      // Find concecutive prefix closures.
      auto nclosures =
          closure_buffer_
              .FindClosureReverseConsecutive(
                  [](ClosureListIter a, ClosureListIter b) -> bool {
                    return (a->Priority() == b->Priority()) &&
                           (a->Operation() == b->Operation());
                  })
              .size();
      auto closure_count = nclosures;
      while (nclosures > 0) {
        closure_buffer_.StreamInsertBeforeClosure(
            std::prev(closure_buffer_.LastClosure(), nclosures - 1),
            kOpenParenTk);
        closure_buffer_.StreamPushBack(kCloseParenTk);
        nclosures--;
      }

      for (std::size_t i = 0; i < closure_count; i++) {
        closure_buffer_.PopClosure();
      }
    }

    inline void ResolvePostfix() {
      // Find concecutive postfix closures.
      auto nclosures =
          closure_buffer_
              .FindClosureReverseConsecutive(
                  [](ClosureListIter a, ClosureListIter b) -> bool {
                    return (a->Priority() == b->Priority()) &&
                           (a->Operation() == b->Operation());
                  })
              .size();
      auto closure_count = nclosures;

      while (nclosures > 0) {
        closure_buffer_.StreamInsertAfterClosure(
            std::prev(closure_buffer_.LastClosure(), closure_count),
            kOpenParenTk);

        closure_buffer_.StreamInsertAfterClosure(
            std::prev(closure_buffer_.LastClosure(), nclosures - 1),
            kCloseParenTk);

        nclosures--;
      }

      for (std::size_t i = 0; i < closure_count; i++) {
        closure_buffer_.PopClosure();
      }
    };

    inline void ResolveBinaryLeftAssoc() {
      std::vector<ClosureListIter> binary_closures =
          closure_buffer_.FindClosureReverseConsecutiveAndIgnore(
              [](ClosureListIter a, ClosureListIter b) -> bool {
                return (a->Priority() == b->Priority()) &&
                       (a->Operation() == b->Operation()) &&
                       (a->Assoc() == b->Assoc());
              },
              [](ClosureListIter a) -> bool {
                return ((a->Priority() == ePriority::kPostfix) ||
                        (a->Priority() == ePriority::kPrefix));
              });

      // Resolve first closure.(last in the list)
      ClosureListIter open_paren_location = std::prev(binary_closures.back());
      // skip any postfix/prefix closures when looking for the open paren loc.
      while (open_paren_location->Priority() == ePriority::kPostfix ||
             open_paren_location->Priority() == ePriority::kPrefix) {
        open_paren_location = std::prev(open_paren_location);
      }

      closure_buffer_.StreamInsertAfterClosure(open_paren_location,
                                               {eTk::kOpenParen, "("});
      closure_buffer_.StreamPushBack({eTk::kCloseParen, ")"});

      // Resolve the rest,skip last in loop.
      std::vector<ClosureListIter>::iterator close_paren_location_iter =
          binary_closures.begin();
      while (close_paren_location_iter != std::prev(binary_closures.end())) {
        closure_buffer_.StreamInsertAfterClosure(open_paren_location,
                                                 {eTk::kOpenParen, "("});

        closure_buffer_.StreamInsertBeforeClosure(*close_paren_location_iter,
                                                  {eTk::kCloseParen, ")"});
        close_paren_location_iter = std::next(close_paren_location_iter);
      }

      // Pop all collected closures.
      for (ClosureListIter closure_iter : binary_closures) {
        closure_buffer_.PopClosure(closure_iter);
      }
    };

    inline void ResolveBinaryRightAssoc() {
      std::vector<ClosureListIter> binary_closures =
          closure_buffer_.FindClosureReverseConsecutiveAndIgnore(
              [](ClosureListIter a, ClosureListIter b) -> bool {
                return (a->Priority() == b->Priority()) &&
                       (a->Operation() == b->Operation()) &&
                       (a->Assoc() == b->Assoc());
              },
              [](ClosureListIter a) -> bool {
                return ((a->Priority() == ePriority::kPostfix) ||
                        (a->Priority() == ePriority::kPrefix));
              });

      // Resolve first closure.(last in the list)
      closure_buffer_.StreamInsertAfterClosure(
          std::prev(binary_closures.back()), {eTk::kOpenParen, "("});
      closure_buffer_.StreamPushBack({eTk::kCloseParen, ")"});
      // Resolve the rest.
      std::vector<ClosureListIter>::iterator open_paren_location_iter =
          std::next(binary_closures.begin());
      while (open_paren_location_iter != binary_closures.end()) {
        closure_buffer_.StreamInsertAfterClosure(*open_paren_location_iter,
                                                 {eTk::kOpenParen, "("});
        closure_buffer_.StreamPushBack({eTk::kCloseParen, ")"});
        open_paren_location_iter = std::next(open_paren_location_iter);
      }
      // Pop all collected closures.
      for (ClosureListIter closure_iter : binary_closures) {
        closure_buffer_.PopClosure(closure_iter);
      }
    };

    inline void ResolveBinary() {
      // Resolve associative binary if there are repeated equivalent priority
      // closures before the last.
      if (closure_buffer_.LastClosure()->Priority() ==
          std::prev(closure_buffer_.LastClosure())->Priority()) {
        if (closure_buffer_.LastClosure()->Assoc() == eAssoc::kLeft) {
          ResolveBinaryLeftAssoc();
        } else if (closure_buffer_.LastClosure()->Assoc() == eAssoc::kRight) {
          ResolveBinaryRightAssoc();
        } else {
          throw "ResolveBinary: Invalid binary token in closure, association "
            "not left or right.\n";
        }
      } else {  // Resolve Singular Binary Closure
        // Special Case: If the binary closure is higher priority than
        // prefix/postfix
        //               Then don't only skip the postfix.

        ClosureListIter open_paren_insertion_loc_iter =
            std::prev(closure_buffer_.LastClosure());

        // Lower:
        if (closure_buffer_.LastClosure()->Priority() < ePriority::kPostfix) {
          // If there are postfix/prefix closures-> skip them when
          // looking for the open paren insertion location.
          ClosureListIter postfix_it = std::prev(closure_buffer_.LastClosure());
          while (postfix_it->Priority() == ePriority::kPostfix) {
            postfix_it = std::prev(postfix_it);
          }

          ClosureListIter prefix_it = postfix_it;
          if (prefix_it->Priority() == ePriority::kPrefix) {
            while (prefix_it->Priority() == ePriority::kPrefix) {
              prefix_it = std::prev(prefix_it);
            }
          }

          open_paren_insertion_loc_iter = prefix_it;
        }
        // Higher(will never be equal):
        else {
          // If there are postfix/prefix closures-> skip them when
          // looking for the open paren insertion location.
          ClosureListIter postfix_it = std::prev(closure_buffer_.LastClosure());
          while (postfix_it->Priority() == ePriority::kPostfix) {
            postfix_it = std::prev(postfix_it);
          }

          open_paren_insertion_loc_iter = postfix_it;
        }

        closure_buffer_.StreamInsertAfterClosure(open_paren_insertion_loc_iter,
                                                 {eTk::kOpenParen, "("});
        closure_buffer_.StreamPushBack({eTk::kCloseParen, ")"});
        closure_buffer_.PopClosure();
      }
    };

    // Resolves the last closure based on type,unchecked.
    inline void ResolveLast() {
      if (closure_buffer_.LastClosure()->Operation() == eOperation::kPrefix) {
        ResolvePrefix();
      } else if (closure_buffer_.LastClosure()->Operation() ==
                 eOperation::kPostfix) {
        ResolvePostfix();
      } else if (closure_buffer_.LastClosure()->Operation() ==
                 eOperation::kBinary) {
        ResolveBinary();
      } else {
        throw "ActionResolveLast: Invalid token type in closure.Must be an "
               "operator.\n";
      }
    }

    // [Action Skip] Push OPERAND to closure output,
    //    advance head by the number of tokens consumed.
    inline BoolError ActionSkip(TkCursor& c) {
      // Check for an open paren-> Subexpression
      // Resolve the subexpr first.
      // Push resolved result to output.
      if (c.TypeIs(eTk::kOpenParen)) {
        TkScope scope = TkScope::find_paren(c);
        if (!scope.Valid()) {
          return "Mismatched parentheses in operand.";
        } else {
          // Resolve but do not parse!the inside of the parentheses.
          RParseValueExpression resolved_subexpr_parser;
          Expected<TkVector> resolved_subexpr_result =
              RParseValueExpression().CreateAndResolveClosures(
                  {scope.ContainedBegin(), scope.ContainedEnd()});
          if (!resolved_subexpr_result.Valid()) {
            return "RParseValueExpression::ActionSkip: Could not resolve "
                   "subexpression.\n" +
                   resolved_subexpr_result.Error();
          }
          const TkVector& resolved_subexpr = resolved_subexpr_result.Value();
          for (const Tk& tk : resolved_subexpr) {
            closure_buffer_.StreamPushBack(tk);
          }
          for (TkVectorConstIter i = c.Iter(); i != scope.End(); i++) {
            c.Advance();
          }
          return true;
        }
      }
      // Check if this is a singular token OPERAND.
      if (c.Operation() == eOperation::kNone || c.TypeIs(eTk::kSubtraction)) {
        // If unary minus - Emplace unary minus into operand.
        if (c.TypeIs(eTk::kSubtraction)) {
          c.Advance();
          if(c.TypeIs(eTk::kNumberLiteral) || c.TypeIs(eTk::kDoubleLiteral)) {
            Tk negative_number = c.Get();
            negative_number.LiteralMutable() = "-" + negative_number.Literal();
            closure_buffer_.StreamPushBack(negative_number);
            c.Advance();
            return true;
          } else {
            return "RParseValueExpression::ActionSkip: Unary minus must be "
                   "followed by a numeric literal.\n";
          }
        }
        // Parse single token operand.
        closure_buffer_.StreamPushBack(c.Get());
        c.Advance();
        return true;
      }

      return "RParseValueExpression::ActionSkip: Could not read operand token, "
             "invalid or unimplemented.";
    }

    // [Action Store] Push OPERATOR to closure output,
    //                Push back a new closure from OPERATOR,
    //                advance head by the number of tokens consumed.
    inline BoolError ActionStore(TkCursor& c) {
      // Check for an open paren-> Function Call
      // Arguments are parsed and resolved in ParseImpl.
      // Push a closure for the function call.
      if (c.TypeIs(eTk::kOpenParen)) {
        TkScope scope = TkScope::find_paren(c);
        if (!scope.Valid()) {
          return "Mismatched parentheses in function call.";
        } else {
          closure_buffer_.StreamPushBack(c.Get());
          TkListIter fn_args_closure_begin = closure_buffer_.LastStreamed();
          c.Advance();
          // Store the rest of the function call.
          for (TkVectorConstIter i = c.Iter(); i != scope.End(); i++) {
            closure_buffer_.StreamPushBack(c.Get());
            c.Advance();
          }
          // Make the closure.
          closure_buffer_.PushBackClosure(fn_args_closure_begin,
                                          closure_buffer_.LastStreamed());
          return true;
        }
      }
      // Check for an open bracket-> Indexing Operator
      if (c.TypeIs(eTk::kOpenBracket)) {
        TkScope scope = TkScope::find_bracket(c);
        if (!scope.Valid()) {
          return "Mismatched bracket in indexing call.";
        } else {
          closure_buffer_.StreamPushBack(c.Get());
          TkListIter fn_args_closure_begin = closure_buffer_.LastStreamed();
          c.Advance();
          for (TkVectorConstIter i = c.Iter(); i != scope.End(); i++) {
            closure_buffer_.StreamPushBack(c.Get());
            c.Advance();
          }
          closure_buffer_.PushBackClosure(fn_args_closure_begin,
                                          closure_buffer_.LastStreamed());
          return true;
        }
      }
      // Listing Operator
      if (c.TypeIs(eTk::kOpenBrace)) {
        TkScope scope = TkScope::find_brace(c);
        if (!scope.Valid()) {
          return "Mismatched brace in listing call.";
        } else {
          closure_buffer_.StreamPushBack(c.Get());
          TkListIter fn_args_closure_begin = closure_buffer_.LastStreamed();
          c.Advance();
          for (TkVectorConstIter i = c.Iter(); i != scope.End(); i++) {
            closure_buffer_.StreamPushBack(c.Get());
            c.Advance();
          }
          closure_buffer_.PushBackClosure(fn_args_closure_begin,
                                          closure_buffer_.LastStreamed());
          return true;
        }
      }
      // Single Token Operator.
      if (c.Operation() == eOperation::kBinary ||
          c.Operation() == eOperation::kPrefix ||
          c.Operation() == eOperation::kPostfix) {
        closure_buffer_.StreamPushBack(c.Get());
        closure_buffer_.PushBackClosure(closure_buffer_.LastStreamed());
        c.Advance();
        return true;
      }

      return "RParseValueExpression::ActionStore: Could not read operator "
             "token, "
             "invalid or unimplemented.";
    }

    // [Action Check] Compares priority of head and last closure.
    //    If head >= last closure, store head.
    //    If head < last closure,
    //        -> resolve all consecutive closures of lower priority.
    //           Then store.
    //    If head is at end, resolve all closures. Return.
    inline BoolError ActionCheck(TkCursor& c) {
      if (c.AtEnd()) {
        while (closure_buffer_.ClosureCount() > 0) {
          ResolveLast();
        }
        return true;
      } else {
        if (c.Priority() >= closure_buffer_.LastClosure()->Priority()) {
          // If: (Head >= Last Closure) => Store
          ActionStore(c);
        } else {
          // While:( Head < Last Closure ) => Resolve Next
          while (c.Priority() < closure_buffer_.LastClosure()->Priority()) {
            ResolveLast();
          }
          ActionStore(c);
        }
        return true;
      }
    }

    // [Choose Action] Decides which action to take.
    //                 Main loop of the closure resolver.
    // Action will be skip , store, or check.
    // - On first occurence of an operator- a store will be performed.
    //    All following operators will be checks.
    // - A skip is performed when an operand is encountered.
    //    Multi-token operands trigger repeated skips.
    // - A check is performed when an operator is encountered.
    // - At the end, always do a check.
    inline BoolError ChooseAction(TkCursor& c) {
      if (c.AtEnd()) {
        BoolError check_result = ActionCheck(c);
        if (check_result) {
          is_resolved_ = true;
          return true;
        } else {
          return "RParseValueExpression::ChooseAction: Could not resolve "
                 "closures.\n" +
                 check_result.Error();
        }
      }
      // Choose action based on expected head token.
      else {
        // Operative-> Looking for an operand,prefix operator,open
        // scope(subexpr)
        // SPECIAL CASE[Unary Minus] -> If binary '-' is encountered.
        //                           -> Bind tightly to the following operand.
        //                           -> Assert Numeric Literal. Emplace - into operand.
        //                           -> Skip Both.
        //                         // If the following is non number, error.
        if (next_expected_head_token_ == eNextExpectedHeadToken::kOperative) {
          if (c.Operation() == eOperation::kNone || c.TypeIs(eTk::kOpenParen)||
            c.TypeIs(eTk::kSubtraction)) {
            BoolError skip_result = ActionSkip(c);
            if (skip_result) {
              next_expected_head_token_ = eNextExpectedHeadToken::kOperator;
              return true;
            } else {
              return "RParseValueExpression::ChooseAction: Could not skip "
                     "operand.\n" +
                     skip_result.Error();
            }
          }
          // Prefix Operator -> Check Single Prefix Operator.
          else if (c.Operation() == eOperation::kPrefix) {
            if (is_first_operator_) {
              BoolError store_result = ActionStore(c);
              if (!store_result) {
                return "RParseValueExpression::ChooseAction: Could not store "
                       "prefix operator.\n" +
                       store_result.Error();
              }
            } else {
              BoolError check_result = ActionCheck(c);
              if (!check_result) {
                return "RParseValueExpression::ChooseAction: Could not check "
                       "prefix operator.\n" +
                       check_result.Error();
              }
            }
            FirstOperatorSwitch();
            next_expected_head_token_ = eNextExpectedHeadToken::kOperative;
            return true;
          }
          // expected operand got operator -> user error
          else if (c.Operation() == eOperation::kBinary ||
                   c.Operation() == eOperation::kPostfix) {
            return "RParseValueExpression::ChooseAction: Invalid token type "
                   "encountered, operator following operator.\n";
          }
          // Else user error -> this token does not belong in a value
          // expression.
          else {
            return "RParseValueExpression::ChooseAction: "
                   "Token type not valid for a primary expression.\n";
          }
        }
        // Operator
        // Looking for a binary operator, postfix operator, or open
        // paren(function call), or open brace(index operator)
        else if (next_expected_head_token_ ==
                 eNextExpectedHeadToken::kOperator) {
          // Open Paren, Open Bracket, Postfix Operator
          //    -> Check, next is Operator.
          if (c.TypeIs(eTk::kOpenParen) || c.TypeIs(eTk::kOpenBracket) ||
              c.TypeIs(eTk::kOpenBrace) ||
              c.Operation() == eOperation::kPostfix) {
            BoolError action_result;
            if (is_first_operator_) {
              action_result = ActionStore(c);
              FirstOperatorSwitch();
              next_expected_head_token_ = eNextExpectedHeadToken::kOperator;
            } else {
              action_result = ActionCheck(c);
              FirstOperatorSwitch();
              next_expected_head_token_ = eNextExpectedHeadToken::kOperator;
            }
          }
          // Binary Operator -> Check, next is Operative.
          else if (c.Operation() == eOperation::kBinary) {
            BoolError action_result;
            if (is_first_operator_) {
              action_result = ActionStore(c);
            } else {
              action_result = ActionCheck(c);
            }
            FirstOperatorSwitch();
            next_expected_head_token_ = eNextExpectedHeadToken::kOperative;
          }
          // Prefix -> user Error, prefix following operand.
          else if (c.Operation() == eOperation::kPrefix) {
            return "RParseValueExpression::ChooseAction: Invalid token type "
                   "encountered, prefix following operand.\n";
          }
          // Operand -> user Error, operand following operand.
          else if (c.Operation() == eOperation::kNone) {
            return "RParseValueExpression::ChooseAction: Invalid token type "
                   "encountered, operand following operand.\n";
          }
          // Else user error -> this token does not belong in a value
          // expression.
          else {
            return "RParseValueExpression::ChooseAction: "
                   "Token type not valid for a primary expression.\n";
          }
        }
        // This should never happen -> critical logic error.
        // eNextExpectedHeadToken enum set to invalid value.
        else {
          throw "[CRITICAL LOGIC ERROR] RParseValueExpression::ChooseAction: "
                "Invalid next expected head token enumeration type.\n";
        }
      }
    }

    Expected<std::vector<Tk>> CreateAndResolveClosures(TkCursor c) {
      while (!is_resolved_) {
        auto action_result = ChooseAction(c);
        if (!action_result) {
          return Expected<std::vector<Tk>>::Failure(
              "RParseValueExpression::CreateAndResolveClosures: Error creating "
              "and resolving closures.\n" +
              action_result.Error());
        }
      }
      return Expected<std::vector<Tk>>::Success(
          closure_buffer_.StreamToVector());
    }

    // After the closures are resolved, perform an LL recursive descent parse.
    // Expect the expression to be fully parenthesized.
    Expected<Ast> ParseImpl(TkCursor c) {
      enum class eNextExpectedToken { kOperand, kOperator, kAny };
      // eNextExpectedToken next_expected_token = eNextExpectedToken::kAny;
      Ast final_result_node;

      // Expression will always start with one of:
      // - Operand
      // - Prefix Operator
      // - Open Paren(subexpression)
      if (c.TypeIs(eTk::kOpenParen)) {
        auto scope = TkScope::find_paren(c);
        if (!scope.Valid()) {
          return Expected<Ast>::Failure("Mismatched parentheses in operand.");
        } else {
          // Check for redundant parentheses.
          if (scope.End() == c.End()) {
            // Parse the inside of the parentheses instead.
            return ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
          }
          // Scope is an operand contained in a subexpression.
          else {
            Expected<Ast> subexpr_result =
                ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
            if (!subexpr_result.Valid()) {
              return Expected<Ast>::Failure(
                  "RParseValueExpression::ParseImpl: Error parsing "
                  "subexpression.\n" +
                  subexpr_result.Error());
            }
            auto lhs_operand_node = subexpr_result.Extract();
            c.Advance(scope.End());
            // Operand may be followed by a postfix, or a binary operator.
            if (c.Operation() == eOperation::kPostfix) {
              // Postfix () -> Function Call
              if (c.TypeIs(eTk::kOpenParen)) {
                auto scope = TkScope::find_paren(c);
                if (!scope.Valid()) {
                  throw "Mismatched parentheses in function call.";
                }
                // Parse the arguments
                auto arguments_result = ParseArguments(c);
                if (!arguments_result.Valid()) {
                  return Expected<Ast>::Failure(
                      "RParseValueExpression::ParseImpl: Error parsing "
                      "arguments.\n" +
                      arguments_result.Error());
                }
                final_result_node =
                    Ast(eAst::kFunctionCall, "", lhs_operand_node,
                        arguments_result.Extract());
                c.Advance(scope.End());
              }
              // Postfix [] -> Index Operator
              else if (c.TypeIs(eTk::kOpenBracket)) {
                auto scope = TkScope::find_bracket(c);
                if (!scope.Valid()) {
                  throw "Mismatched brackets in indexing call.";
                }
                auto arguments_result = ParseIndexingArguments(c);
                if (!arguments_result.Valid()) {
                  return Expected<Ast>::Failure(
                      "RParseValueExpression::ParseImpl: Error parsing "
                      "indexing arguments.\n" +
                      arguments_result.Error());
                }
                final_result_node =
                    Ast(eAst::kIndexOperator, "", lhs_operand_node,
                        arguments_result.Extract());
                c.Advance(scope.End());
              }
              // Postfix {} -> Listing Operator
              else if (c.TypeIs(eTk::kOpenBrace)) {
                auto scope = TkScope::find_brace(c);
                if (!scope.Valid()) {
                  throw "Mismatched braces in listing call.";
                }
                auto arguments_result = ParseListingArguments(c);
                if (!arguments_result.Valid()) {
                  return Expected<Ast>::Failure(
                      "RParseValueExpression::ParseImpl: Error parsing "
                      "listing arguments.\n" +
                      arguments_result.Error());
                }
                final_result_node =
                    Ast(eAst::kListingOperator, "", lhs_operand_node,
                        arguments_result.Extract());
                c.Advance(scope.End());
              }
              // Postfix Single Token Operator.
              else {
                final_result_node = Ast(c.Get());
                final_result_node.PushBack(lhs_operand_node);
                c.Advance();
              }
            } else if (c.Operation() == eOperation::kBinary) {
              final_result_node = Ast(c.Get());
              c.Advance();
              // Expecting an operand after a binary operator.
              // Open Paren -> Subexpr
              if (c.TypeIs(eTk::kOpenParen)) {
                auto scope = TkScope::find_paren(c);
                if (!scope.Valid()) {
                  return Expected<Ast>::Failure(
                      "Mismatched parentheses in operand.");
                } else {
                  Expected<Ast> subexpr_result =
                      ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
                  if (!subexpr_result.Valid()) {
                    return Expected<Ast>::Failure(
                        "RParseValueExpression::ParseImpl: Error parsing "
                        "subexpression.\n" +
                        subexpr_result.Error());
                  }
                  auto rhs_operand_node = subexpr_result.Extract();
                  final_result_node.PushBack(lhs_operand_node);
                  final_result_node.PushBack(rhs_operand_node);
                  c.Advance(scope.End());
                }
              }
              // Else it has to be a singular token operand.
              else {
                InternalParseResult operand_result = ParseOperand(c);
                if (!operand_result.Valid()) {
                  return Expected<Ast>::Failure(
                      "RParseValueExpression::ParseImpl: Error parsing "
                      "operand.\n" +
                      operand_result.Error());
                }
                auto rhs_operand_node = operand_result.Extract();
                final_result_node.PushBack(lhs_operand_node);
                final_result_node.PushBack(rhs_operand_node);
                c.Advance(operand_result.Always().Iter());
              }
            } else {
              if (c.AtEnd()) {
                final_result_node = lhs_operand_node;
              } else {
                return Expected<Ast>::Failure(
                    "RParseValueExpression::ParseImpl: Invalid token type "
                    "encountered following binary operator.\n");
              }
            }
          }
        }
      }
      // - Operand
      else if (c.Operation() == eOperation::kNone) {
        InternalParseResult operand_result = ParseOperand(c);
        if (!operand_result.Valid()) {
          return Expected<Ast>::Failure(
              "RParseValueExpression::ParseImpl: Error parsing operand.\n" +
              operand_result.Error());
        }
        auto lhs_operand_node = operand_result.Extract();
        c.Advance(operand_result.Always().Iter());
        // Operand may be followed by a postfix, or a binary operator.
        if (c.Operation() == eOperation::kPostfix) {
          // Postfix () -> Function Call
          if (c.TypeIs(eTk::kOpenParen)) {
            auto scope = TkScope::find_paren(c);
            if (!scope.Valid()) {
              throw "Mismatched parentheses in function call.";
            }
            // Parse the arguments
            auto arguments_result = ParseArguments(c);
            if (!arguments_result.Valid()) {
              return Expected<Ast>::Failure(
                  "RParseValueExpression::ParseImpl: Error parsing "
                  "arguments.\n" +
                  arguments_result.Error());
            }
            final_result_node = Ast(eAst::kFunctionCall, "", lhs_operand_node,
                                    arguments_result.Extract());
            c.Advance(scope.End());
          }
          // Postfix [] -> Index Operator
          else if (c.TypeIs(eTk::kOpenBracket)) {
            auto scope = TkScope::find_bracket(c);
            if (!scope.Valid()) {
              throw "Mismatched brackets in indexing call.";
            }
            auto arguments_result = ParseIndexingArguments(c);
            if (!arguments_result.Valid()) {
              return Expected<Ast>::Failure(
                  "RParseValueExpression::ParseImpl: Error parsing "
                  "indexing arguments.\n" +
                  arguments_result.Error());
            }
            final_result_node =
                Ast(eAst::kIndexOperator, "", lhs_operand_node,
                    arguments_result.Extract());  // TODO: Parse Index Arguments
            c.Advance(scope.End());
          }
          // Postfix {} -> Listing Operator
          else if (c.TypeIs(eTk::kOpenBrace)) {
            auto scope = TkScope::find_brace(c);
            if (!scope.Valid()) {
              throw "Mismatched braces in listing call.";
            }
            auto arguments_result = ParseListingArguments(c);
            if (!arguments_result.Valid()) {
              return Expected<Ast>::Failure(
                  "RParseValueExpression::ParseImpl: Error parsing "
                  "listing arguments.\n" +
                  arguments_result.Error());
            }
            final_result_node =
                Ast(eAst::kListingOperator, "", lhs_operand_node,
                    arguments_result.Extract());
            c.Advance(scope.End());
          }
          // Postfix Single Token Operator.
          else {
            final_result_node = Ast(c.Get());
            final_result_node.PushBack(lhs_operand_node);
            c.Advance();
          }
        }
        // Binary Operator after operand.
        else if (c.Operation() == eOperation::kBinary) {
          final_result_node = Ast(c.Get());
          c.Advance();
          // Expecting an operand after a binary operator.
          // Open Paren -> Subexpr
          if (c.TypeIs(eTk::kOpenParen)) {
            auto scope = TkScope::find_paren(c);
            if (!scope.Valid()) {
              return Expected<Ast>::Failure(
                  "Mismatched parentheses in operand.");
            } else {
              Expected<Ast> subexpr_result =
                  ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
              if (!subexpr_result.Valid()) {
                return Expected<Ast>::Failure(
                    "RParseValueExpression::ParseImpl: Error parsing "
                    "subexpression.\n" +
                    subexpr_result.Error());
              }
              auto rhs_operand_node = subexpr_result.Extract();
              final_result_node.PushBack(lhs_operand_node);
              final_result_node.PushBack(rhs_operand_node);
              c.Advance(scope.End());
            }
          }
          // Else it has to be a singular token operand.
          else {
            InternalParseResult operand_result = ParseOperand(c);
            if (!operand_result.Valid()) {
              return Expected<Ast>::Failure(
                  "RParseValueExpression::ParseImpl: Error parsing operand.\n" +
                  operand_result.Error());
            }
            auto rhs_operand_node = operand_result.Extract();
            final_result_node.PushBack(lhs_operand_node);
            final_result_node.PushBack(rhs_operand_node);
            c.Advance(operand_result.Always().Iter());
          }
        } else {
          // expecting to be at the end of the expression.
          if (c.AtEnd()) {
            final_result_node = lhs_operand_node;
          } else {
            return Expected<Ast>::Failure(
                "RParseValueExpression::ParseImpl: Invalid token type "
                "encountered following binary operator.\n");
          }
        }
      }
      // - Prefix
      else if (c.Operation() == eOperation::kPrefix) {
        // Prefix Operator, must be followed by an operand.
        final_result_node = Ast(c.Get());
        c.Advance();

        if (c.AtEnd()) {
          return Expected<Ast>::Failure(
              "RParseValueExpression::ParseImpl: Prefix operator at end of "
              "expression.\n");
        }

        // Open Paren -> Subexpr
        if (c.TypeIs(eTk::kOpenParen)) {
          auto scope = TkScope::find_paren(c);
          if (!scope.Valid()) {
            return Expected<Ast>::Failure("Mismatched parentheses in operand.");
          } else {
            Expected<Ast> subexpr_result =
                ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
            if (!subexpr_result.Valid()) {
              return Expected<Ast>::Failure(
                  "RParseValueExpression::ParseImpl: Error parsing "
                  "subexpression.\n" +
                  subexpr_result.Error());
            }
            final_result_node.PushBack(subexpr_result.Extract());
            c.Advance(scope.End());
          }
        }
        // Else it has to be a singular token operand.
        else if (c.Operation() == eOperation::kNone) {
          InternalParseResult operand_result = ParseOperand(c);
          if (!operand_result.Valid()) {
            return Expected<Ast>::Failure(
                "RParseValueExpression::ParseImpl: Error parsing operand.\n" +
                operand_result.Error());
          }
          final_result_node.PushBack(operand_result.Extract());
          c.Advance(operand_result.Always().Iter());
        } else {
          return Expected<Ast>::Failure(
              "RParseValueExpression::ParseImpl: Invalid token type "
              "encountered following prefix operator.\n");
        }

      }
      // Error -> Invalid token type at beginning of expression.
      else {
        return Expected<Ast>::Failure(
            "RParseValueExpression::ParseImpl: Invalid token type at beginning "
            "of expression.\n");
      }

      return Expected<Ast>::Success(final_result_node);  // temp? unreachable
    }

   public:
    Expected<Ast> Parse(TkCursor c) {
      Expected<std::vector<Tk>> resolved_closures_result =
          CreateAndResolveClosures(c);
      if (!resolved_closures_result.Valid()) {
        return Expected<Ast>::Failure(
            "RParseValueExpression::Parse: Error creating and resolving "
            "closures.\n" +
            resolved_closures_result.Error());
      }

      const std::vector<Tk>& resolved_closures =
          resolved_closures_result.Value();

      TkCursor intermediate_expr(resolved_closures.cbegin(),
                                 resolved_closures.cend());
      Expected<Ast> expected_parse_result = ParseImpl(intermediate_expr);

      if (!expected_parse_result.Valid()) {
        return Expected<Ast>::Failure(
            "RParseValueExpression::Parse: Error parsing resolved closures.\n" +
            expected_parse_result.Error());
      }
      return Expected<Ast>::Success(expected_parse_result.Extract());
    }
  };
  static Expected<Ast> ClosureParsePrimaryExpr(TkCursor c) {
    return RParseValueExpression().Parse(c);
  }
  //------------------------------------//
  //------------------------------------//

  //---------------------------------------------------------------------------//
  //---------------------------------------------------------------------------//

  //---------------------------------------------------------------------------//
  // Internal parsing methods.
  // Ordered from the most to least specific.
  // Each method is responsible for parsing a specific type of statement or
  // declaration.
  // Each method returns an InternalParseResult.
  // The result contains the parsed AST node, and the location of the last token
  // parsed. Begin parsing the next statement from this location.
  // The result also contains an error message if the parsing failed.
  // In this case parsing should stop and the error message should be reported.
  //---------------------------------------------------------------------------//

  //------------------------------------//
  // Parse Operand
  // - Singular operands only, not subexpressions.
  //------------------------------------//
  static InternalParseResult ParseOperand(TkCursor current);

  //------------------------------------//
  // Parse Arguments (<primary_expr?*>,)
  // - Arguments for method calls.
  //------------------------------------//
  static InternalParseResult ParseArguments(TkCursor current);

  //------------------------------------//
  // Parse Indexing Arguments [<primary_expr?*>,]
  // - Arguments for indexing operator.
  //------------------------------------//
  static InternalParseResult ParseIndexingArguments(TkCursor current);

  //------------------------------------//
  // Parse Listing Arguments {<primary_expr?*>,}
  // - Arguments for listing operator.
  //------------------------------------//
  static InternalParseResult ParseListingArguments(TkCursor current);

  //------------------------------------//
  // Parse Primary Statement <primary_expr>;
  // - A single primary expression ending with a semicolon.
  //------------------------------------//
  static InternalParseResult ParsePrimaryStatement(TkCursor current);

  //------------------------------------//
  // ParseConditionalSubExpression (<primary_expr?*>,)
  // - Handles parsing of conditional arguments to For and While loops.
  // - Format will account for a maximum of 3 arguments.
  // - Format will account for a minimum of 1 argument.
  // - Format will account for (i in collection) syntax.
  //------------------------------------//
  static InternalParseResult ParseConditionalSubExpression(TkCursor c);

  //------------------------------------//
  // ParsePrimaryPreIdentifier <primary_expr>@
  // - Handles parsing of primary expressions that are followed by an
  // identifier.
  // - Used in declarations
  //------------------------------------//
  static InternalParseResult ParsePrimaryPreIdentifier(TkCursor current);

  //------------------------------------//
  // ParsePrimaryPostIdentifier <primary_expr>: or <primary_expr>;
  // - Handles parsing of primary expressions that are followed by a colon or
  // semicolon.
  // - Used in declarations, method signatures.
  //------------------------------------//
  static InternalParseResult ParsePrimaryPostIdentifier(TkCursor current);

  //------------------------------------//
  // Parse Modifiers <modifier?*>
  // - Parses a list of keyword modifiers.
  //------------------------------------//
  static InternalParseResult ParseModifiers(TkCursor current);
  static InternalParseResult ParseReturnStmt(TkCursor current);
  static InternalParseResult ParseMethodParameters(TkCursor current);
  static InternalParseResult ParseMethodReturnParameters(TkCursor current);
  static InternalParseResult ParseMethodSignature(TkCursor current);
  static InternalParseResult ParseMethodDef(TkCursor current);
  static InternalParseResult ParseMainDef(TkCursor current);
  static InternalParseResult ParseClassDef(TkCursor current);
  static InternalParseResult ParseLibDef(TkCursor current);
  static InternalParseResult ParsePragmaticStmt(TkCursor current);
  static InternalParseResult ParseFunctionalStmt(TkCursor current);
  static InternalParseResult ParseConditionalStmt(TkCursor c);
  static InternalParseResult ParseIfDecl(TkCursor c);
  static InternalParseResult ParseMainDecl(TkCursor current);
  static InternalParseResult ParseLibDecl(TkCursor current);
  static InternalParseResult ParseImportDecl(TkCursor current);
  static InternalParseResult ParseWhileDecl(TkCursor c);
  static InternalParseResult ParseForDecl(TkCursor c);
  static InternalParseResult ParseUsingDecl(TkCursor current);
  static InternalParseResult ParseVariableDecl(TkCursor current);
  static InternalParseResult ParseMethodDecl(TkCursor current);
  static InternalParseResult ParseClassDecl(TkCursor current);
  static InternalParseResult ParseProgram(TkCursor current);
  //---------------------------------------------------------------------------//
  //---------------------------------------------------------------------------//
 public:
  static Expected<Ast> Parse(const TkVector & c) {
    auto parsed = ParseProgram({c.cbegin(), c.cend()});
    if (parsed) {
      return Expected<Ast>::Success(parsed.Extract());
    } else {
      return Expected<Ast>::Failure(parsed.Error());
    }
  }
  LarkParser() = default;
  ~LarkParser() = default;
};
// Static const members of LarkParser.
const Tk LarkParser::RParseValueExpression::kOpenParenTk =
    Tk{eTk::kOpenParen, "("};
const Tk LarkParser::RParseValueExpression::kCloseParenTk =
    Tk{eTk::kCloseParen, ")"};

LarkParser::InternalParseResult LarkParser::ParseOperand(TkCursor c) {
  using namespace compiler_error::parser;
  if (c.IsSingularOperand())
    return Success(c.Next(), c.Get());
  else
    return Failure(c,
                   xProgrammerLogicError(eAst::kValue, c.Iter(),
                                         "Could not parse singular operand."));
};

LarkParser::InternalParseResult LarkParser::ParseArguments(TkCursor c) {
  using namespace compiler_error::parser;
  if (c.TypeIs(eTk::kOpenParen)) {
    if (c.Next().TypeIs(eTk::kCloseParen)) {  // Empty Arg.
      return Success(c.Advance(2), Ast(eAst::kArguments));
    }

    std::vector<TkScope> arg_scopes =
        TkScope::find_seperated_paren(c, eTk::kComma);
    if (not arg_scopes.front()) {
      return Failure(c, xMismatchedParentheses(c.Iter()));
    } else {
      Ast arguments_node = eAst::kArguments;
      for (const TkScope& arg_scope : arg_scopes) {
        Expected<Ast> arg_result =
            ClosureParsePrimaryExpr(arg_scope.Contained());
        if (!arg_result) {
          // TODO: Add error message type for this.
          return Failure(c, arg_result.Error());
        }
        arguments_node.PushBack(arg_result.Extract());
      }
      return Success(c.Advance(arg_scopes.back().End()), arguments_node);
    }
  } else {
    return Failure(c, xProgrammerLogicError(eAst::kExpression, c.Iter()));
  }
}

LarkParser::InternalParseResult LarkParser::ParseIndexingArguments(TkCursor c) {
  if (c.TypeIs(eTk::kOpenBracket)) {
    // Check for empty arguments.
    if (c.Next().TypeIs(eTk::kCloseBracket)) {
      return Success(c.Advance(2), Ast(eAst::kArguments));
    }

    std::vector<TkScope> arg_scopes =
        TkScope::find_seperated_bracket(c, eTk::kComma);
    if (!arg_scopes.front().Valid()) {
      return Failure(c, compiler_error::parser::xMismatchedParentheses(
                            c.Iter(),
                            "[LarkParser::ParseIndexingArguments] Mismatched "
                            "brackets in indexing call."));
    } else {
      Ast arguments_node(eAst::kArguments);
      for (const TkScope& arg_scope : arg_scopes) {
        Expected<Ast> arg_result = RParseValueExpression().Parse(
            {arg_scope.ContainedBegin(), arg_scope.ContainedEnd()});
        if (!arg_result.Valid()) {
          return Failure(
              c, std::string(
                     "[LarkParser::ParseIndexingArguments] Error parsing ") +
                     "argument in indexing call.\n" + arg_result.Error());
        }
        arguments_node.PushBack(arg_result.Extract());
      }
      return Success(c.Advance(arg_scopes.back().End()), arguments_node);
    }
  } else {
    return Failure(c, compiler_error::parser::xProgrammerLogicError(
                          Ast(c.Get()).Type(), c.Iter(),
                          "[LarkParser::ParseIndexingArguments] Token on begin "
                          "cursor does not evaluate to an indexing argument "
                          "node."));
  }
}

LarkParser::InternalParseResult LarkParser::ParseListingArguments(TkCursor c) {
  if (c.TypeIs(eTk::kOpenBrace)) {
    // Check for empty arguments.
    if (c.Next().TypeIs(eTk::kCloseBrace)) {
      return Success(c.Advance(2), Ast(eAst::kArguments));
    }

    std::vector<TkScope> arg_scopes =
        TkScope::find_seperated_brace(c, eTk::kComma);
    if (!arg_scopes.front().Valid()) {
      return Failure(c, compiler_error::parser::xMismatchedParentheses(
                            c.Iter(),
                            "[LarkParser::ParseListingArguments] Mismatched "
                            "brackets in indexing call."));
    } else {
      Ast arguments_node(eAst::kArguments);
      for (const TkScope& arg_scope : arg_scopes) {
        Expected<Ast> arg_result = RParseValueExpression().Parse(
            {arg_scope.ContainedBegin(), arg_scope.ContainedEnd()});
        if (!arg_result.Valid()) {
          return Failure(
              c, std::string(
                     "[LarkParser::ParseListingArguments] Error parsing ") +
                     "argument in indexing call.\n" + arg_result.Error());
        }
        arguments_node.PushBack(arg_result.Extract());
      }
      return Success(c.Advance(arg_scopes.back().End()), arguments_node);
    }
  } else {
    return Failure(c, compiler_error::parser::xProgrammerLogicError(
                          Ast(c.Get()).Type(), c.Iter(),
                          "[LarkParser::ParseListingArguments] Token on begin "
                          "cursor does not evaluate to an indexing argument "
                          "node."));
  }
}

LarkParser::InternalParseResult LarkParser::ParsePrimaryStatement(TkCursor c) {
  // Primary statement will begin with:
  // - A singular token operand.
  // - A prefix operator.
  // - An open paren which is a subexpression.
  if (c.IsSingularOperand() || c.IsSingularPrefixOperator() ||
      c.TypeIs(eTk::kOpenParen)) {
    TkScope statement_scope = TkScope::find_program_statement(c);
    if (statement_scope.Valid()) {
      Expected<Ast> statement_result = RParseValueExpression().Parse(
          {statement_scope.Begin(), statement_scope.ContainedEnd()});
      if (!statement_result.Valid()) {
        return Failure(c,
                       compiler_error::parser::xProgrammerLogicError(
                           Ast(c.Get()).Type(), c.Iter(),
                           "[LarkParser::ParsePrimaryStatement] Error parsing "
                           "primary statement.\n" +
                               statement_result.Error()));
      }
      return Success(c.Advance(statement_scope.End()),
                     statement_result.Extract());
    } else {
      return Failure(c, compiler_error::parser::xMismatchedParentheses(
                            c.Iter(),
                            "[LarkParser::ParsePrimaryStatement] Mismatched "
                            "parentheses in primary statement." +
                                statement_scope.Error()));
    }
  } else {
    return Failure(c, compiler_error::parser::xProgrammerLogicError(
                          Ast(c.Get()).Type(), c.Iter(),
                          "[LarkParser::ParsePrimaryStatement] Token on begin "
                          "cursor does not evaluate to a primary statement "
                          "node."));
  }
}

LarkParser::InternalParseResult LarkParser::ParseConditionalSubExpression(
    TkCursor c) {
  TkScope paren_scope = TkScope::find_paren(c);
  if (paren_scope.Valid()) {
    if (paren_scope.ContainedBegin()->IsPrimaryExpressionOpening()) {
      Expected<Ast> subexpr_result = RParseValueExpression().Parse(
          {paren_scope.ContainedBegin(), paren_scope.ContainedEnd()});
      if (!subexpr_result.Valid()) {
        return Failure(c, compiler_error::parser::xProgrammerLogicError(
                              Ast(c.Get()).Type(), c.Iter(),
                              "[ParseConditionalSubExpression] Error parsing "
                              "conditional subexpression.\n" +
                                  subexpr_result.Error()));
      }
      return Success(c.Advance(paren_scope.End()), subexpr_result.Extract());
    } else {
      return Failure(
          c,
          compiler_error::parser::xUserSyntaxError(
              c.Iter(), "Invalid begining of conditional primary expression."));
    }
  } else {
    return Failure(c, compiler_error::parser::xMismatchedParentheses(
                          c.Iter(),
                          "[ParseConditionalSubExpression] Mismatched "
                          "parentheses in conditional subexpression."));
  }
}

LarkParser::InternalParseResult LarkParser::ParsePrimaryPreIdentifier(
    TkCursor c) {
  if (c.IsSingularOperand() || c.IsSingularPrefixOperator() ||
      c.TypeIs(eTk::kOpenParen)) {
    TkScope statement_scope = TkScope::find_open_statement(
        c.Get().Type(), {eTk::kCommercialAt}, c.Iter(), c.End());
    if (statement_scope.Valid()) {
      Expected<Ast> statement_result = RParseValueExpression().Parse(
          {statement_scope.Begin(), statement_scope.ContainedEnd()});
      if (!statement_result.Valid()) {
        return Failure(c, compiler_error::parser::xProgrammerLogicError(
                              Ast(c.Get()).Type(), c.Iter(),
                              "[ParsePrimaryPreIdentifier] Error parsing "
                              "primary PreIdentifier.\n" +
                                  statement_result.Error()));
      }
      return Success(c.Advance(statement_scope.End()),
                     statement_result.Extract());
    } else {
      return Failure(c, compiler_error::parser::xMismatchedParentheses(
                            c.Iter(),
                            "[ParsePrimaryPreIdentifier] Mismatched "
                            "parentheses in primary PreIdentifier." +
                                statement_scope.Error()));
    }
  } else {
    // Else the type is implicitly an any type.
    // Expecting a @
    if (c.TypeIs(eTk::kCommercialAt)) {
      return Success(c.Advance(), Ast(eAst::kAny));
    }

    return Failure(
        c, compiler_error::parser::xProgrammerLogicError(
               Ast(c.Get()).Type(), c.Iter(), "[ParsePrimaryPreIdentifier]"));
  }
}

LarkParser::InternalParseResult LarkParser::ParsePrimaryPostIdentifier(
    TkCursor c) {
  if (c.IsSingularOperand() || c.IsSingularPrefixOperator() ||
      c.TypeIs(eTk::kOpenParen)) {
    TkScope statement_scope = TkScope::find_open_statement(
        c.Get().Type(), {eTk::kColon, eTk::kSemicolon}, c.Iter(), c.End());
    if (statement_scope.Valid()) {
      Expected<Ast> statement_result = RParseValueExpression().Parse(
          {statement_scope.Begin(), statement_scope.ContainedEnd()});
      if (!statement_result.Valid()) {
        return Failure(c, compiler_error::parser::xProgrammerLogicError(
                              Ast(c.Get()).Type(), c.Iter(),
                              "[ParsePrimaryPostIdentifier] Error parsing "
                              "primary PreIdentifier.\n" +
                                  statement_result.Error()));
      }
      // Special case: if end of statement is a semicolon, go 1 back.
      // This will allow the callee to determine if the statement is
      // terminated or not.
      if (statement_scope.ContainedEnd()->TypeIs(eTk::kSemicolon)) {
        return Success(c.Advance(statement_scope.ContainedEnd()),
                       statement_result.Extract());
      }

      return Success(c.Advance(statement_scope.End()),
                     statement_result.Extract());
    } else {
      return Failure(c, compiler_error::parser::xMismatchedParentheses(
                            c.Iter(),
                            "[ParsePrimaryPostIdentifier] Mismatched "
                            "parentheses in primary PreIdentifier." +
                                statement_scope.Error()));
    }
  } else {
    // fallback, should not be called. ParseMethodSignature should handle.
    if (c.TypeIs(eTk::kColon)) {
      return Success(c.Advance(), Ast(eAst::kAny));
    }

    return Failure(c, compiler_error::parser::xProgrammerLogicError(
                          Ast(c.Get()).Type(), c.Iter(),
                          "[ParsePrimaryPostIdentifier]expected colon"));
  }
}

LarkParser::InternalParseResult LarkParser::ParseModifiers(TkCursor c) {
  if (c.IsModifierKeyword()) {
    Ast modifiers_node(eAst::kModifiers);
    while (c.IsModifierKeyword()) {
      modifiers_node.PushBack(Ast(c.Get()));
      c.Advance();
    }
    return Success(c, modifiers_node);
  } else {
    return Failure(c, compiler_error::parser::xProgrammerLogicError(
                          Ast(c.Get()).Type(), c.Iter(),
                          "[LarkParser::ParseModifiers] Token on begin cursor "
                          "does not evaluate to a modifier node."));
  }
}

LarkParser::InternalParseResult LarkParser::ParseReturnStmt(TkCursor c) {
  // Expect a return keyword.
  if (c.TypeIs(eTk::kReturn)) {
    c.Advance();

    if (c.TypeIs(eTk::kSemicolon)) {
      Ast return_statement_node(eAst::kReturn);
      return Success(c.Advance(), return_statement_node);
    }

    // Expecting a value expression.
    InternalParseResult value_expr_result = ParsePrimaryStatement(c);
    if (!value_expr_result) {
      return Failure(c, compiler_error::parser::xProgrammerLogicError(
                            Ast(c.Get()).Type(), c.Iter(),
                            "[LarkParser::ParseReturnStmt] Error parsing "
                            "value expression.\n" +
                                value_expr_result.Error()));
    }
    Ast return_statement_node(eAst::kReturn);
    return_statement_node.PushBack(value_expr_result.Extract());
    return Success(c, return_statement_node);
  } else {
    return Failure(c, compiler_error::parser::xExpectedToken(
                          ToStr(eTk::kReturn), c.Literal(),
                          "[LarkParser::ParseReturnStmt] Expected return "
                          "keyword."));
  }
}

LarkParser::InternalParseResult LarkParser::ParsePragmaticStmt(TkCursor c) {
  // Format: <modifiers?><decl keyword>
  if (c.TypeIs(eTk::kSemicolon)) {
    // Empty statement
    std::cout << "Warning: Empty statement in code.";
    return Success(c.Advance(), Ast(eAst::kNone));
  }
  // 0. Handle declarations which cannot have modifiers.
  //    - use
  //    - main  TODO: implement main.
  //    - import TODO: implement import.
  if (c.IsDeclarativeKeyword()) {
    switch (c.Type()) {
      case eTk::kUse:
        return ParseUsingDecl(c);
      case eTk::kMain:
        return ParseMainDecl(c);
      case eTk::kImport:
        return ParseImportDecl(c);
      default:
        break;
    }
  }

  // 1. Store begin and skip any number of modifiers when looking for decl type.
  TkCursor start_of_decl = c;
  while (c.IsModifierKeyword()) {
    c.Advance();
  }

  // 2. Expecting a declarative keyword.
  if (c.IsDeclarativeKeyword()) {
    // 3. Decide what kind of declaration it is based on keyword.
    //    All declrations which could not have modifiers were handled above.
    //    Return specific error when trying to modify unmodifiable declaration.
    switch (c.Type()) {
      case eTk::kDef:
        return ParseVariableDecl(c);
      case eTk::kFn:
        return ParseMethodDecl(c);
      case eTk::kClass:
        return ParseClassDecl(c);
      case eTk::kLib:
        return ParseLibDecl(c);
      case eTk::kUse:
      case eTk::kMain:
        return Failure(c, compiler_error::parser::xUserSyntaxError(
                              c.Iter(),
                              "[LarkParser::ParseDeclaration] Declarative "
                              "Keyword cannot be modified."));
      default:
        return Failure(c, compiler_error::parser::xProgrammerLogicError(
                              Ast(c.Get()).Type(), c.Iter(),
                              "[LarkParser::ParseDeclaration] Declarative "
                              "Keyword not implemented in ParseDeclaration"));
    }
  } else {
    return Failure(
        c, compiler_error::parser::xExpectedToken(
               ToStr(eTk::kDef), c.Literal(),
               "[LarkParser::ParseDeclaration] Expected declarative token."));
  }
}

LarkParser::InternalParseResult LarkParser::ParseFunctionalStmt(TkCursor c) {
  if (c.TypeIs(eTk::kSemicolon)) {
    // Empty statement
    std::cout << "Warning: Empty statement in code.";
    return Success(c.Advance(), Ast(eAst::kNone));
  }
  
  // 0. Handle declarations which cannot have modifiers.
  //    - use
  //    - main  TODO: implement main.
  //    - import TODO: implement import.
  if (c.IsDeclarativeKeyword()) {
    switch (c.Type()) {
      case eTk::kIf:
        return ParseIfDecl(c);
      case eTk::kWhile:
        return ParseWhileDecl(c);
      case eTk::kFor:
        return ParseForDecl(c);
      case eTk::kReturn:
        return ParseReturnStmt(c);
      case eTk::kUse:
        return ParseUsingDecl(c);
      case eTk::kImport:
        return ParseImportDecl(c);
      default:
        break;
    }
  }

  // 1. Store begin and skip any number of modifiers when looking for decl type.
  TkCursor start_of_decl = c;
  while (c.IsModifierKeyword()) {
    c.Advance();
  }

  // 2. Expecting a declarative keyword.
  if (c.IsDeclarativeKeyword()) {
    // 3. Decide what kind of declaration it is based on keyword.
    //    All declrations which could not have modifiers were handled above.
    //    Return specific error when trying to modify unmodifiable declaration.
    switch (c.Type()) {
      case eTk::kDef:
        return ParseVariableDecl(c);
      case eTk::kFn:
        return ParseMethodDecl(c);
      case eTk::kClass:
        return ParseClassDecl(c);
      case eTk::kUse:
        return Failure(c, compiler_error::parser::xUserSyntaxError(
                              c.Iter(),
                              "[ParseFunctionalStmt] Declarative "
                              "Keyword cannot be modified."));
      default:
        return Failure(c, compiler_error::parser::xProgrammerLogicError(
                              Ast(c.Get()).Type(), c.Iter(),
                              "[ParseFunctionalStmt] Declarative "
                              "Keyword type not allowed in functional block."));
    }
  } else {
    return Failure(c, compiler_error::parser::xExpectedToken(
                          ToStr(eTk::kDef), c.Literal(),
                          "[ParseFunctionalStmt] Expected declarative token."));
  }
}

LarkParser::InternalParseResult LarkParser::ParseConditionalStmt(TkCursor c) {
  // Handle declarations which cannot have modifiers.
  if (c.IsDeclarativeKeyword()) {
    switch (c.Type()) {
      case eTk::kUse:
        return ParseUsingDecl(c);
      case eTk::kImport:
        return ParseImportDecl(c);
      default:
        break;
    }
  }
  // Store begin and skip any number of modifiers when looking for decl type.
  TkCursor start_of_decl = c;
  while (c.IsModifierKeyword()) {
    c.Advance();
  }
  // Expecting a declarative keyword.
  if (c.IsDeclarativeKeyword()) {
    // Decide what kind of declaration it is based on keyword.
    // All declrations which could not have modifiers were handled above.
    // Return specific error when trying to modify unmodifiable declaration.
    switch (c.Type()) {
      case eTk::kDef:
        return ParseVariableDecl(c);
      case eTk::kFn:
        return ParseMethodDecl(c);
      case eTk::kClass:
        return ParseClassDecl(c);
      case eTk::kUse:
        return Failure(c, compiler_error::parser::xUserSyntaxError(
                              c.Iter(),
                              "[ParseConditionalStmt] Declarative "
                              "Keyword cannot be modified."));
      default:
        return Failure(c, compiler_error::parser::xProgrammerLogicError(
                              Ast(c.Get()).Type(), c.Iter(),
                              "[ParseConditionalStmt] Declarative "
                              "Keyword type not allowed in functional block."));
    }
  } else {
    return Failure(c,
                   compiler_error::parser::xExpectedToken(
                       ToStr(eTk::kDef), c.Literal(),
                       "[ParseConditionalStmt] Expected declarative token."));
  }
}

LarkParser::InternalParseResult LarkParser::ParseIfDecl(TkCursor c) {
  using namespace compiler_error::parser;
  lambda xParseIfElifElse = [&c](eAst ast_type) {
    if (c.TypeIs(eTk::kIf) or c.TypeIs(eTk::kElif)) {
      c.Advance();
      auto condition_result = ParseConditionalSubExpression(c);
      if (not condition_result) {
        return Failure(c, xExpectedToken(ToStr(eTk::kOpenParen), c.Literal()));
      }
      c.Advance(condition_result);
      // Parse the body.
      auto body_result = ParseMethodDef(c);
      if (not body_result) {
        return Failure(c, xExpectedToken(ToStr(eTk::kOpenBrace), c.Literal()));
      }
      c.Advance(body_result);
      return Success(c, Ast(ast_type, "", condition_result.Extract(),
                            body_result.Extract()));
    } else if (c.TypeIs(eTk::kElse)) {
      c.Advance();
      auto body_result = ParseMethodDef(c);
      if (not body_result) {
        return Failure(c, xExpectedToken(ToStr(eTk::kOpenBrace), c.Literal()));
      }
      c.Advance(body_result);
      return Success(c, Ast(ast_type, "", body_result.Extract()));
    } else {
      throw "Expected if or elif or else.";
    }
  };

  if (c.TypeIsnt(eTk::kIf)) {
    return Failure(c, compiler_error::parser::xExpectedToken(ToStr(eTk::kIf),
                                                             c.Literal()));
  }

  Ast ifelifelse_statement = eAst::kIfStatement;
  auto if_statement = xParseIfElifElse(eAst::kIf);
  if (not if_statement) {
    return if_statement;
  }
  ifelifelse_statement.PushBack(if_statement.Extract());

  if (c.TypeIs(eTk::kSemicolon)) {
    return Success(c.Advance(), ifelifelse_statement);
  }

  while (c.TypeIs(eTk::kElif)) {
    auto elif_statement = xParseIfElifElse(eAst::kElif);
    if (not elif_statement) {
      return elif_statement;
    }
    ifelifelse_statement.PushBack(elif_statement.Extract());
  }

  if (c.TypeIs(eTk::kElse)) {
    auto else_statement = xParseIfElifElse(eAst::kElse);
    if (not else_statement) {
      return else_statement;
    }
    ifelifelse_statement.PushBack(else_statement.Extract());
  }

  return Success(c, ifelifelse_statement);
}

LarkParser::InternalParseResult LarkParser::ParseWhileDecl(TkCursor c) {
  // While Keyword, Conditional subexpression, Functional Block, Semicolon.

  if (c.TypeIsnt(eTk::kWhile)) {
    return Failure(c, compiler_error::parser::xExpectedToken(ToStr(eTk::kWhile),
                                                             c.Literal()));
  }

  c.Advance();

  auto condition_result = ParseConditionalSubExpression(c);
  if (not condition_result) {
    return Failure(c, compiler_error::parser::xExpectedToken(
                          ToStr(eTk::kOpenParen), c.Literal()));
  }
  c.Advance(condition_result);

  auto body_result = ParseMethodDef(c);
  if (not body_result) {
    return Failure(c, compiler_error::parser::xExpectedToken(
                          ToStr(eTk::kOpenBrace), c.Literal()));
  }
  c.Advance(body_result);

  if (c.TypeIs(eTk::kSemicolon)) {
    return Success(c.Advance(),
                   Ast(eAst::kWhile, "", condition_result.Extract(),
                       body_result.Extract()));
  }

  return Failure(c, compiler_error::parser::xExpectedToken(
                        ToStr(eTk::kSemicolon), c.Literal()));
}

LarkParser::InternalParseResult LarkParser::ParseForDecl(TkCursor c) {
  using namespace compiler_error::parser;
  if (c.TypeIsnt(eTk::kFor)) {
    return Failure(c, xExpectedToken(ToStr(eTk::kFor), c.Literal()));
  }
  c.Advance();

  TkScope condition_scope = TkScope::find_paren(c);
  if (not condition_scope.Valid()) {
    return Failure(c, xMismatchedParentheses(c.Iter()));
  }

  std::vector<TkScope> condition_scopes =
      TkScope::find_seperated_paren(c, eTk::kSemicolon);
  if (condition_scopes.size() > 3) {
    return Failure(
        c, xInvalidForLoopConditionSyntax(
               c.Iter(),
               "For condition may have a maximum of 3 statements.Detected:" +
                   std::to_string(condition_scopes.size())));
  }

  auto init_var_result = ParseVariableDecl(
      {condition_scopes[0].ContainedBegin(), condition_scopes[0].End()});
  if (not init_var_result) {
    return init_var_result;
  }

  auto condition_result = ParsePrimaryStatement(
      {condition_scopes[1].ContainedBegin(), condition_scopes[1].End()});
  if (not condition_result) {
    return condition_result;
  }

  auto increment_result = RParseValueExpression().Parse(
      {condition_scopes[2].ContainedBegin(), condition_scope.ContainedEnd()});
  if (not increment_result) {
    return Failure(c, increment_result.Error());
  }
  c.Advance(condition_scope.End());

  auto body_result = ParseMethodDef(c);
  if (not body_result) {
    return body_result;
  }
  c.Advance(body_result);

  if (c.TypeIs(eTk::kSemicolon)) {
    return Success(c.Advance(),
                   Ast(eAst::kFor, "", init_var_result.Extract(),
                       condition_result.Extract(), increment_result.Extract(),
                       body_result.Extract()));
  }

  return Failure(c, xExpectedToken(ToStr(eTk::kSemicolon), c.Literal()));
}

LarkParser::InternalParseResult LarkParser::ParseUsingDecl(TkCursor c) {
  if (c.TypeIs(eTk::kUse)) {
    c.Advance();
    // Next may be:
    // - @ commercial at -> Type Alias
    // - @name: lib-> Library Type Alias.
    if (c.TypeIs(eTk::kCommercialAt)) {
      c.Advance();
      if (!c.TypeIs(eTk::kIdentifier)) {
        return Failure(
            c, compiler_error::parser::xExpectedToken(
                   ToStr(eTk::kIdentifier), c.Literal(),
                   "[LarkParser::ParseUsingDecl] Expected identifier."));
      }
      Ast typedef_identifier = Ast(c.Get());
      c.Advance();

      if (!c.TypeIs(eTk::kColon)) {
        return Failure(c, compiler_error::parser::xExpectedToken(
                              ToStr(eTk::kColon), c.Literal(),
                              "[LarkParser::ParseUsingDecl] Expected colon."));
      }
      c.Advance();

      // If next is lib, then it is a library type alias.
      // Everything following lib must be a value expression closed by a
      // semicolon. Wether it is reduced to a type is determined at a later
      // stage. Builds a kLibraryTypeAlias else it is a type alias.
      if (c.TypeIs(eTk::kLib)) {
        c.Advance();
        InternalParseResult value_expr_result = ParsePrimaryStatement(c);
        if (!value_expr_result.Valid()) {
          return Failure(
              c,
              "[LarkParser::ParseUsingDecl] Error parsing value expression." +
                  value_expr_result.Error());
        }
        c.Advance(value_expr_result.Always().Iter());
        return Success(c, Ast(eAst::kLibraryTypeAlias, "", typedef_identifier,
                              value_expr_result.Extract()));

      } else {
        InternalParseResult value_expr_result = ParsePrimaryStatement(c);
        if (!value_expr_result.Valid()) {
          return Failure(
              c,
              "[LarkParser::ParseUsingDecl] Error parsing value expression." +
                  value_expr_result.Error());
        }
        c.Advance(value_expr_result.Always().Iter());
        return Success(c, Ast(eAst::kTypeAlias, "", typedef_identifier,
                              value_expr_result.Extract()));
      }
    }
    // - lib-> Library Namespace Inclusion.
    else if (c.TypeIs(eTk::kLib)) {
      c.Advance();
      InternalParseResult value_expr_result = ParsePrimaryStatement(c);
      if (!value_expr_result.Valid()) {
        return Failure(
            c, "[LarkParser::ParseUsingDecl] Error parsing value expression." +
                   value_expr_result.Error());
      }
      c.Advance(value_expr_result.Always().Iter());
      return Success(c, Ast(eAst::kLibraryNamespaceInclusion, "",
                            value_expr_result.Extract()));

    }
    // - namespace-> Namespace Inclusion.
    else if (c.TypeIs(eTk::kNamespace)) {
      c.Advance();
      InternalParseResult value_expr_result = ParsePrimaryStatement(c);
      if (!value_expr_result.Valid()) {
        return Failure(
            c, "[LarkParser::ParseUsingDecl] Error parsing value expression." +
                   value_expr_result.Error());
      }
      c.Advance(value_expr_result.Always().Iter());
      return Success(
          c, Ast(eAst::kNamespaceInclusion, "", value_expr_result.Extract()));
    } else if (c.IsSingularOperand() || c.IsSingularPrefixOperator() ||
               c.TypeIs(eTk::kOpenParen)) {
      InternalParseResult primary_result = ParsePrimaryStatement(c);
      if (!primary_result) {
        return primary_result.ChainFailure(
            "[ParseUsingDecl: Failed to parse primary expression.]");
      }
      c.Advance(primary_result.Always().Iter());
      return Success(c, Ast(eAst::kNamespaceObjectInclusion, "",
                            primary_result.Extract()));

    } else {
      return Failure(
          c, compiler_error::parser::xExpectedToken(
                 ToStr(eTk::kCommercialAt) + " or " + ToStr(eTk::kLib),
                 c.Literal(),
                 "[LarkParser::ParseUsingDecl] Expected commercial at or "
                 "lib or namespace or type expression."));
    }

  } else {
    return Failure(c, compiler_error::parser::xProgrammerLogicError(
                          Ast(c.Get()).Type(), c.Iter(),
                          "[LarkParser::ParseUsingDecl] Token on begin cursor "
                          "does not evaluate to a use declaration node."));
  }
};

LarkParser::InternalParseResult LarkParser::ParseVariableDecl(TkCursor c) {
  // Format:
  // <modifiers?><def><what*?><commercial_at?><name?><colon?><definition?>
  // <semicolon>
  Ast modifiers_node;  // Check if there are any modifiers.
  if (c.IsModifierKeyword()) {
    InternalParseResult modifiers_result = ParseModifiers(c);
    if (!modifiers_result.Valid()) {
      return Failure(c, modifiers_result.Error());
    }
    modifiers_node = modifiers_result.Extract();
    c.Advance(modifiers_result.Always().Iter());
  } else {
    // No modifiers found.
    modifiers_node = Ast(eAst::kModifiers);
  }

  // Expect a def keyword
  if (c.TypeIs(eTk::kDef)) {
    c.Advance();
    // 2. Expect a what token(s).
    // Everything following def until kCommericalAt is a type constraint
    // describing what is being created.
    InternalParseResult what_result = ParsePrimaryPreIdentifier(c);
    if (!what_result.Valid()) {
      return Failure(c, what_result.Error());
    }
    Ast what_node = what_result.Extract();
    c.Advance(what_result.Always().Iter());

    // Expecting an identifier.
    if (!c.TypeIs(eTk::kIdentifier)) {
      return Failure(
          c, compiler_error::parser::xExpectedToken(
                 ToStr(eTk::kIdentifier), c.Literal(),
                 "[LarkParser::ParseVariableDecl] Expected identifier."));
    }
    Ast identifier_node = Ast(c.Get());
    c.Advance();

    // If there is a colon, this is a Definition.
    // If there is a semicolon, this is a Declaration.
    if (c.TypeIs(eTk::kColon)) {
      c.Advance();
      InternalParseResult definition_result = ParsePrimaryStatement(c);
      if (!definition_result.Valid()) {
        return Failure(c, definition_result.Error());
      }
      Ast definition_node =
          Ast(eAst::kVariableDefinition, "", definition_result.Extract());
      c.Advance(definition_result.Always().Iter());
      return Success(c, Ast(eAst::kVariableDeclaration, "", modifiers_node,
                            what_node, identifier_node, definition_node));
    } else if (c.TypeIs(eTk::kSemicolon)) {
      c.Advance();
      return Success(c, Ast(eAst::kVariableDeclaration, "", modifiers_node,
                            what_node, identifier_node));
    } else {
      return Failure(
          c,
          compiler_error::parser::xExpectedToken(
              ToStr(eTk::kColon) + " or " + ToStr(eTk::kSemicolon), c.Literal(),
              "[LarkParser::ParseVariableDecl] Expected colon or "
              "semicolon."));
    }

  } else {
    return Failure(c,
                   compiler_error::parser::xProgrammerLogicError(
                       Ast(c.Get()).Type(), c.Iter(),
                       "[LarkParser::ParseVariableDecl] Token on begin cursor "
                       "does not evaluate to a variable declaration node."));
  }
}

LarkParser::InternalParseResult LarkParser::ParseMethodDecl(TkCursor c) {
  // Format:
  // <modifiers?><fn><commercial_at?><name?><signature?><colon?>
  // <definition?> <semicolon>
  Ast modifiers_node;  // Check if there are any modifiers.
  if (c.IsModifierKeyword()) {
    InternalParseResult modifiers_result = ParseModifiers(c);
    if (!modifiers_result) {
      return modifiers_result.ChainFailure("[ParseMethodDecl]");
    }
    modifiers_node = modifiers_result.Extract();
    c.Advance(modifiers_result.Always().Iter());
  } else {
    // No modifiers found.
    modifiers_node = Ast(eAst::kModifiers);
  }

  // Expect a fn keyword
  if (c.TypeIs(eTk::kFn)) {
    c.Advance();
    // Expecting a commercial @
    if (c.TypeIsnt(eTk::kCommercialAt)) {
      return Failure(
          c, compiler_error::parser::xExpectedToken(
                 ToStr(eTk::kCommercialAt), c.Literal(), "[ParseMethodDecl]"));
    }
    c.Advance();

    // Expecting an identifier.
    if (c.TypeIsnt(eTk::kIdentifier)) {
      return Failure(
          c, compiler_error::parser::xExpectedToken(
                 ToStr(eTk::kIdentifier), c.Literal(), "[ParseMethodDecl]"));
    }
    Ast identifier_node = Ast(c.Get());
    c.Advance();

    Ast signature_node;
    // Expecting a signature or colon/semicolon.
    if (c.TypeIsnt(eTk::kColon) && c.TypeIsnt(eTk::kSemicolon)) {
      InternalParseResult signature_result = ParseMethodSignature(c);
      if (!signature_result.Valid()) {
        return Failure(c, signature_result.Error());
      }
      signature_node = signature_result.Extract();
      c.Advance(signature_result.Always().Iter());
    } else {
      signature_node = Ast(eAst::kMethodSignature);
    }

    // If there is a colon, this is a Definition.
    // If there is a semicolon, this is a Declaration.
    if (c.TypeIs(eTk::kColon)) {
      c.Advance();
      InternalParseResult definition_result = ParseMethodDef(c);
      if (!definition_result.Valid()) {
        return Failure(c, definition_result.Error());
      }
      Ast definition_node = definition_result.Extract();
      c.Advance(definition_result.Always().Iter());
      // Expect semicolon
      if (c.TypeIsnt(eTk::kSemicolon)) {
        return Failure(
            c, compiler_error::parser::xExpectedToken(
                   ToStr(eTk::kSemicolon), c.Literal(),
                   "[LarkParser::ParseMethodDecl] Expected semicolon."));
      }
      c.Advance();
      return Success(c, Ast(eAst::kMethodDeclaration, "", modifiers_node,
                            identifier_node, signature_node, definition_node));

    } else if (c.TypeIs(eTk::kSemicolon)) {
      c.Advance();
      return Success(c, Ast(eAst::kMethodDeclaration, "", modifiers_node,
                            identifier_node, signature_node));

    } else {
      return Failure(
          c,
          compiler_error::parser::xExpectedToken(
              ToStr(eTk::kColon) + " or " + ToStr(eTk::kSemicolon), c.Literal(),
              "[LarkParser::ParseMethodDecl] Expected colon or semicolon."));
    }

  } else {
    return Failure(c, compiler_error::parser::xProgrammerLogicError(
                          Ast(c.Get()).Type(), c.Iter(),
                          "[LarkParser::ParseMethodDecl] Token on begin cursor "
                          "does not evaluate to a method declaration node."));
  }
}

LarkParser::InternalParseResult LarkParser::ParseClassDecl(TkCursor c) {
  // Format:
  // <modifiers?><class><commercial_at?><name?><colon?>
  // <class_definition?> <semicolon>
  Ast modifiers_node;  // Check if there are any modifiers.
  if (c.IsModifierKeyword()) {
    InternalParseResult modifiers_result = ParseModifiers(c);
    if (!modifiers_result) {
      return modifiers_result.ChainFailure("[ParseClassDecl]");
    }
    modifiers_node = modifiers_result.Extract();
    c.Advance(modifiers_result.Always().Iter());
  } else {
    // No modifiers found.
    modifiers_node = Ast(eAst::kModifiers);
  }

  // Expect a class keyword
  if (c.TypeIs(eTk::kClass)) {
    c.Advance();

    // Expecting a commercial @
    if (c.TypeIsnt(eTk::kCommercialAt)) {
      return Failure(
          c, compiler_error::parser::xExpectedToken(
                 ToStr(eTk::kCommercialAt), c.Literal(), "[ParseClassDecl]"));
    }
    c.Advance();

    // Expecting an identifier.
    if (!c.TypeIs(eTk::kIdentifier)) {
      return Failure(c, compiler_error::parser::xExpectedToken(
                            ToStr(eTk::kIdentifier), c.Literal(),
                            "[ParseClassDecl] Expected identifier."));
    }
    Ast identifier_node = Ast(c.Get());
    c.Advance();

    // If there is a colon, this is a Definition.
    // If there is a semicolon, this is a Declaration.
    if (c.TypeIs(eTk::kColon)) {
      c.Advance();
      InternalParseResult definition_result = ParseClassDef(c);
      if (!definition_result) {
        return definition_result.ChainFailure("[ParseClassDecl]");
      }
      Ast definition_node = definition_result.Extract();
      c.Advance(definition_result.Always().Iter());
      return Success(c, Ast(eAst::kClassDeclaration, "", modifiers_node,
                            identifier_node, definition_node));
    } else if (c.TypeIs(eTk::kSemicolon)) {
      c.Advance();
      return Success(
          c, Ast(eAst::kClassDeclaration, "", modifiers_node, identifier_node));
    } else {
      return Failure(
          c, compiler_error::parser::xExpectedToken(
                 ToStr(eTk::kColon) + " or " + ToStr(eTk::kSemicolon),
                 c.Literal(), "[ParseClassDecl] Expected colon or semicolon."));
    }
  } else {
    return Failure(c, compiler_error::parser::xProgrammerLogicError(
                          Ast(c.Get()).Type(), c.Iter(), "[ParseClassDecl]"));
  }
}

LarkParser::InternalParseResult LarkParser::ParseImportDecl(TkCursor c) {
  // Format: <import><name><semicolon>
  if (c.TypeIs(eTk::kImport)) {
    c.Advance();
    // Expecting an identifier.
    if (!c.TypeIs(eTk::kIdentifier)) {
      return Failure(c, compiler_error::parser::xExpectedToken(
                            ToStr(eTk::kIdentifier), c.Literal(),
                            "[ParseImportDecl] Expected identifier."));
    }
    Ast identifier_node = Ast(c.Get());
    c.Advance();

    // Expecting a semicolon.
    if (c.TypeIs(eTk::kSemicolon)) {
      c.Advance();
      return Success(c, Ast(eAst::kImportDeclaration, "", identifier_node));
    } else {
      return Failure(c, compiler_error::parser::xExpectedToken(
                            ToStr(eTk::kSemicolon), c.Literal(),
                            "[ParseImportDecl] Expected semicolon."));
    }
  } else {
    return Failure(c, compiler_error::parser::xProgrammerLogicError(
                          Ast(c.Get()).Type(), c.Iter(), "[ParseImportDecl]"));
  }
}

LarkParser::InternalParseResult LarkParser::ParseLibDecl(TkCursor c) {
  // Format:
  // <modifiers?><lib><commercial_at?><name?><colon?>
  // <lib_definition?> <semicolon>
  Ast modifiers_node;  // Check if there are any modifiers.
  if (c.IsModifierKeyword()) {
    InternalParseResult modifiers_result = ParseModifiers(c);
    if (!modifiers_result) {
      return modifiers_result.ChainFailure("[ParseLibDecl]");
    }
    modifiers_node = modifiers_result.Extract();
    c.Advance(modifiers_result.Always().Iter());
  } else {
    // No modifiers found.
    modifiers_node = Ast(eAst::kModifiers);
  }

  // Expect a lib keyword
  if (c.TypeIs(eTk::kLib)) {
    c.Advance();
    // If there is a colon, this is an unnamed library.
    // Expect a definition ending in a semicolon.
    if (c.TypeIs(eTk::kColon)) {
      c.Advance();
      InternalParseResult definition_result = ParseLibDef(c);
      if (!definition_result) {
        return definition_result.ChainFailure("[ParseLibDecl]");
      }
      Ast definition_node = definition_result.Extract();
      c.Advance(definition_result.Always().Iter());
      return Success(c, Ast(eAst::kLibraryDeclaration, "", modifiers_node,
                            definition_node));
    }

    // If there is a @ following the lib keyword, this is a named library.
    // Expecting a commercial @ followed by an identifier.
    if (!c.TypeIs(eTk::kCommercialAt)) {
      return Failure(c, compiler_error::parser::xExpectedToken(
                            ToStr(eTk::kCommercialAt), c.Literal(),
                            "[ParseLibDecl] Expected commercial at."));
    }
    c.Advance();

    // Expecting an identifier.
    if (!c.TypeIs(eTk::kIdentifier)) {
      return Failure(c, compiler_error::parser::xExpectedToken(
                            ToStr(eTk::kIdentifier), c.Literal(),
                            "[ParseLibDecl] Expected identifier."));
    }
    Ast identifier_node = Ast(c.Get());
    c.Advance();

    if (c.TypeIs(eTk::kSemicolon)) {
      c.Advance();
      return Success(c, Ast(eAst::kLibraryDeclaration, "", modifiers_node,
                            identifier_node));
    }

    // Expecting a colon and a definition.
    if (c.TypeIsnt(eTk::kColon)) {
      return Failure(c, compiler_error::parser::xExpectedToken(
                            ToStr(eTk::kColon), c.Literal(),
                            "[ParseLibDecl] Expected colon."));
    }
    c.Advance();

    InternalParseResult definition_result = ParseLibDef(c);
    if (!definition_result) {
      return definition_result.ChainFailure("[ParseLibDecl]");
    }
    Ast definition_node = definition_result.Extract();
    c.Advance(definition_result.Always().Iter());
    return Success(c, Ast(eAst::kLibraryDeclaration, "", modifiers_node,
                          identifier_node, definition_node));
  } else {
    return Failure(c, compiler_error::parser::xProgrammerLogicError(
                          Ast(c.Get()).Type(), c.Iter(), "[ParseLibDecl]"));
  }
}

LarkParser::InternalParseResult LarkParser::ParseMainDecl(TkCursor c) {
  // Format:
  // <main><commerical_at?><name?><function_signature><colon?><definition?>
  // <semicolon>

  // Expect a main keyword
  if (c.TypeIs(eTk::kMain)) {
    c.Advance();

    // If there is a commercial at following the main keyword, this is a named
    // main. Expecting a commercial @ followed by an identifier.
    if (c.TypeIs(eTk::kCommercialAt)) {
      return Failure(
          c, compiler_error::parser::xUserSyntaxError(
                 c.Iter(), "[ParseMainDecl] Named main not implemented."));
    } else {
      // This is an unnamed main.
      // Expecting a function signature followed by a colon and a definition.
      // Expecting a colon.
      Ast signature_node;
      if (c.TypeIs(eTk::kColon)) {
        signature_node = Ast(eAst::kMethodSignature);
        c.Advance();
      } else {
        InternalParseResult signature_result = ParseMethodSignature(c);
        if (!signature_result) {
          return signature_result.ChainFailure("[ParseMainDecl]");
        }
        signature_node = signature_result.Extract();
        c.Advance(signature_result.Always().Iter());
      }

      // Expecting a definition.
      InternalParseResult definition_result = ParseMainDef(c);
      if (!definition_result) {
        return definition_result.ChainFailure("[ParseMainDecl]");
      }

      Ast definition_node = definition_result.Extract();
      c.Advance(definition_result.Always().Iter());
      // Expect semicolon
      if (c.TypeIsnt(eTk::kSemicolon)) {
        return Failure(c,
                       compiler_error::parser::xExpectedToken(
                           ToStr(eTk::kSemicolon), c.Literal(),
                           "[LarkParser::ParseMainDecl] Expected semicolon."));
      }
      c.Advance();

      return Success(
          c, Ast(eAst::kMainDeclaration, "", signature_node, definition_node));
    }

  } else {
    return Failure(c, compiler_error::parser::xProgrammerLogicError(
                          Ast(c.Get()).Type(), c.Iter(), "[ParseMainDecl]"));
  }
}

LarkParser::InternalParseResult LarkParser::ParseMethodParameters(TkCursor c) {
  if (c.TypeIsnt(eTk::kOpenParen)) {
    return Failure(c,
                   compiler_error::parser::xExpectedToken(
                       ToStr(eTk::kOpenParen), c.Literal(),
                       "[ParseMethodParameters] Expected open parenthesis."));
  }

  // Check for empty arguments.
  if (c.Next().TypeIs(eTk::kCloseParen)) {
    return Success(c.Advance(2), Ast(eAst::kMethodParameterList, "",
                                     Ast(eAst::kMethodParameter, "",
                                         Ast(eAst::kMethodVoid))));
  }

  std::vector<TkScope> arg_scopes =
      TkScope::find_seperated_paren(c, eTk::kComma);
  if (!arg_scopes.front().Valid()) {
    return Failure(c, compiler_error::parser::xMismatchedParentheses(
                          c.Iter(),
                          "[ParseMethodParameters] Mismatched "
                          "parentheses in method parameters."));
  }

  lambda xFindCommercialAt = [](TkVectorConstIter beg,
                                TkVectorConstIter end) -> bool {
    TkCursor c(beg, end);
    while (c.TypeIsnt(eTk::kCommercialAt)) {
      if (not c.AtEnd())
        c.Advance();
      else
        return false;
    }
    return true;
  };

  // For each arg:
  // - If there is no @ then it is an identifier of type any -> Expecting an
  // identifier.
  // - If there is an @ then it is a typed argument -> Expect possible mods,
  // followed a primaryPreIdentifier.
  Ast method_parameters_list_node(eAst::kMethodParameterList);
  for (const auto& scope : arg_scopes) {
    Ast method_parameter_node(eAst::kMethodParameter);
    c.Advance(scope.ContainedBegin());
    if (xFindCommercialAt(scope.ContainedBegin(), scope.ContainedEnd())) {
      // Modifiers
      if (c.IsModifierKeyword()) {
        InternalParseResult method_mods_result = ParseModifiers(c);
        if (!method_mods_result) {
          return method_mods_result.ChainFailure("[ParseMethodParameters]");
        }
        c.Advance(method_mods_result.Always().Iter());
        method_parameter_node.PushBack(method_mods_result.Extract());
      } else {
        method_parameter_node.PushBack(
            Ast(eAst::kModifiers, "", Ast(eAst::kNone)));
      }
      // Type
      if (c.IsPrimaryExpressionOpening()) {
        InternalParseResult method_parameter_type_result =
            ParsePrimaryPreIdentifier(c);
        if (!method_parameter_type_result) {
          return method_parameter_type_result.ChainFailure(
              "[ParseMethodParameters] Error parsing method parameter type.");
        }
        c.Advance(method_parameter_type_result.Always().Iter());
        method_parameter_node.PushBack(method_parameter_type_result.Extract());
      } else if (c.TypeIs(eTk::kCommercialAt)) {
        c.Advance();
        // Implied any type.
        method_parameter_node.PushBack(Ast(eAst::kAny));
      } else {
        return Failure(c, compiler_error::parser::xExpectedToken(
                              "Primary Expression", c.Literal(),
                              "[ParseMethodParameters] Expected primary "
                              "expression opening."));
      }
      // Identity
      if (c.TypeIs(eTk::kIdentifier)) {
        method_parameter_node.PushBack(c.Get());
      } else {
        return Failure(c, compiler_error::parser::xExpectedToken(
                              ToStr(eTk::kIdentifier), c.Literal(),
                              "[ParseMethodParameters] Expected identifier."));
      }

      // Add to list
      method_parameters_list_node.PushBack(method_parameter_node);
    } else {
      if (scope.ContainedBegin()->TypeIs(eTk::kIdentifier)) {
        method_parameters_list_node.PushBack(
            Ast(eAst::kMethodParameter, "",
                Ast(eAst::kModifiers, "", Ast(eAst::kNone)), Ast(eAst::kAny),
                Ast(*scope.ContainedBegin())));
      } else {
        return Failure(
            c, compiler_error::parser::xExpectedToken(
                   ToStr(eTk::kIdentifier), scope.ContainedBegin()->Literal(),
                   "[ParseMethodParameters]"));
      }
    }
  }
  return Success(c.Advance(arg_scopes.back().End()),
                 method_parameters_list_node);
}

LarkParser::InternalParseResult LarkParser::ParseMethodReturnParameters(
    TkCursor c) {
  Ast method_return_type_node(eAst::kMethodReturnType);
  // Expect any number of modifiers.
  if (c.IsModifierKeyword()) {
    InternalParseResult method_mods_result = ParseModifiers(c);
    if (!method_mods_result) {
      return method_mods_result.ChainFailure("[ParseMethodReturnParameters]");
    }
    c.Advance(method_mods_result.Always().Iter());
    method_return_type_node.PushBack(method_mods_result.Extract());
  }

  // Expecting a primary expression ending in a colon.
  if (c.IsPrimaryExpressionOpening()) {
    auto ret_type_result = ParsePrimaryPostIdentifier(c);
    if (!ret_type_result) {
      return ret_type_result.ChainFailure("[ParseMethodReturnParameters]");
    }
    c.Advance(ret_type_result.Always().Iter());
    method_return_type_node.PushBack(ret_type_result.Extract());
  } else {
    return Failure(c, compiler_error::parser::xExpectedToken(
                          "Primary Expression", c.Literal(),
                          "[ParseMethodReturnParameters]Expected Primary "
                          "expression opening."));
  }

  return Success(c, method_return_type_node);
}

LarkParser::LarkParser::InternalParseResult LarkParser::ParseMethodSignature(
    TkCursor c) {
  const lambda xMakeSingleParamMethodSignatureAst = [](Ast ret, Ast param) {
    return Ast(eAst::kMethodSignature, "",
               Ast(eAst::kMethodParameterList, "",
                   Ast(eAst::kMethodParameter, "", param)),
               Ast(eAst::kMethodReturnType, "", ret));
  };

  const lambda xMakeMethodSignatureAst = [](Ast ret_type_ast,
                                            Ast param_list_ast) {
    return Ast(eAst::kMethodSignature, "", param_list_ast, ret_type_ast);
  };
  // Colon after identifier.
  // Implicit void arg, no return.
  if (c.TypeIs(eTk::kColon)) {
    return Success(c, xMakeSingleParamMethodSignatureAst(eAst::kMethodVoid,
                                                         eAst::kMethodVoid));
  }
  // GreaterThan after identifier.
  // Some sort of void arg with a return.
  else if (c.TypeIs(eTk::kGreaterThan)) {
    c.Advance();
    if (c.TypeIs(eTk::kColon)) {  // Implicit any return void method.
      c.Advance();
      return Success(
          c, xMakeSingleParamMethodSignatureAst(eAst::kAny, eAst::kMethodVoid));
    }

    if (c.TypeIs(eTk::kSemicolon)) {
      return Success(
          c, xMakeSingleParamMethodSignatureAst(eAst::kAny, eAst::kMethodVoid));
    }

    // Parse the return type.
    InternalParseResult return_type_result = ParseMethodReturnParameters(c);
    if (!return_type_result) {
      return return_type_result.ChainFailure("[ParseMethodSignature]");
    }

    // Return type must end in a colon. If it doesn't, error above.
    c.Advance(return_type_result.Always().Iter());
    return Success(
        c, xMakeMethodSignatureAst(
               return_type_result.Extract(),
               Ast{eAst::kMethodParameterList, "",
                   Ast{eAst::kMethodParameter, "", Ast{eAst::kMethodVoid}}}));
  }
  // Open Paren After Identifier.
  // Method with arguments.
  // Parse the method parameters.
  else if (c.TypeIs(eTk::kOpenParen)) {
    // Parse the method parameters.
    InternalParseResult method_params_result = ParseMethodParameters(c);
    if (!method_params_result) {
      return method_params_result.ChainFailure("[ParseMethodSignature]");
    }
    c.Advance(method_params_result.Always().Iter());
    if (c.TypeIs(eTk::kSemicolon)) {
      return Success(
          c, xMakeMethodSignatureAst(
                 Ast(eAst::kMethodReturnType, "", Ast(eAst::kMethodVoid)),
                 method_params_result.Extract()));
    }

    // Expecting a colon or a greater than.
    if (c.TypeIs(eTk::kColon)) {
      c.Advance();
      return Success(
          c, xMakeMethodSignatureAst(
                 Ast(eAst::kMethodReturnType, "", Ast(eAst::kMethodVoid)),
                 method_params_result.Extract()));
    } else if (c.TypeIs(eTk::kGreaterThan)) {
      c.Advance();
      // if the next token is a colon, then the return type is any.
      if (c.TypeIs(eTk::kColon)) {
        c.Advance();
        return Success(c, xMakeMethodSignatureAst(
                              Ast(eAst::kMethodReturnType, "", Ast(eAst::kAny)),
                              method_params_result.Extract()));
      }

      if (c.TypeIs(eTk::kSemicolon)) {
        return Success(c, xMakeMethodSignatureAst(
                              Ast(eAst::kMethodReturnType, "", Ast(eAst::kAny)),
                              method_params_result.Extract()));
      }

      InternalParseResult return_type_result = ParseMethodReturnParameters(c);
      if (!return_type_result) {
        return return_type_result.ChainFailure("[ParseMethodSignature]");
      }
      c.Advance(return_type_result.Always().Iter());
      return Success(c,
                     xMakeMethodSignatureAst(return_type_result.Extract(),
                                             method_params_result.Extract()));
    } else {
      return Failure(c,
                     compiler_error::parser::xExpectedToken(
                         ToStr(eTk::kColon) + " or " + ToStr(eTk::kGreaterThan),
                         c.Literal(),
                         "[ParseMethodSignature] Expected colon or "
                         "greater than."));
    }
  } else {
    return Failure(
        c, compiler_error::parser::xProgrammerLogicError(
               Ast(c.Get()).Type(), c.Iter(),
               "[ParseMethodSignature] Invalid token following method name."));
  }
}

LarkParser::InternalParseResult LarkParser::ParseMethodDef(TkCursor c) {
  Ast node(eAst::kMethodDefinition);
  TkScope statement_scope = TkScope::find_brace(c);
  if (!statement_scope.Valid()) {
    return Failure(c, compiler_error::parser::xMismatchedParentheses(
                          c.Iter(), "[ParseLibDef] Mismatched braces."));
  }
  c.Advance();  // advance past the opening brace.
  // Parse until end of scope.
  while (c.Iter() != statement_scope.ContainedEnd()) {
    if (c.IsModifierKeyword() || c.IsDeclarativeKeyword()) {
      InternalParseResult decl_result = ParseFunctionalStmt(c);
      if (!decl_result) {
        return decl_result.ChainFailure(
            "[Parsing Method Declarative Statement]");
      }
      node.PushBack(decl_result.Extract());
      c.Advance(decl_result.Always().Iter());
    } else if (c.IsPrimaryExpressionOpening()) {
      InternalParseResult primary_result = ParsePrimaryStatement(c);
      if (!primary_result) {
        return primary_result.ChainFailure(
            "[Parsing Method Primary Statement]");
      }
      node.PushBack(primary_result.Extract());
      c.Advance(primary_result.Always().Iter());
    } else {
      return Failure(c, compiler_error::parser::xUserSyntaxError(
                            c.Iter(), "[Parsing Method Primary Statement]"));
    }
  }
  c.Advance();  // advance to scope end.
  return Success(c, node);
}

LarkParser::InternalParseResult LarkParser::ParseMainDef(TkCursor c) {
  Ast node(eAst::kMainDefinition);
  TkScope statement_scope = TkScope::find_brace(c);
  if (!statement_scope.Valid()) {
    return Failure(c, compiler_error::parser::xMismatchedParentheses(
                          c.Iter(), "[ParseLibDef] Mismatched braces."));
  }
  c.Advance();  // advance past the opening brace.
  // Parse until end of scope.
  while (c.Iter() != statement_scope.ContainedEnd()) {
    if (c.IsModifierKeyword() || c.IsDeclarativeKeyword()) {
      InternalParseResult decl_result = ParseFunctionalStmt(c);
      if (!decl_result) {
        return decl_result.ChainFailure(
            "[Parsing Method Declarative Statement]");
      }
      node.PushBack(decl_result.Extract());
      c.Advance(decl_result.Always().Iter());
    } else if (c.IsPrimaryExpressionOpening()) {
      InternalParseResult primary_result = ParsePrimaryStatement(c);
      if (!primary_result) {
        return primary_result.ChainFailure(
            "[Parsing Method Primary Statement]");
      }
      node.PushBack(primary_result.Extract());
      c.Advance(primary_result.Always().Iter());
    } else {
      return Failure(c, compiler_error::parser::xUserSyntaxError(
                            c.Iter(), "[Parsing Method Primary Statement]"));
    }
  }
  c.Advance();  // advance to scope end.
  // expect semicolon
  return Success(c, node);
}

LarkParser::InternalParseResult LarkParser::ParseClassDef(TkCursor c) {
  // Format: <open_brace> <statement?*> <close_brace>
  Ast node(eAst::kClassDefinition);
  TkScope statement_scope = TkScope::find_brace(c);
  if (!statement_scope.Valid()) {
    return Failure(c, compiler_error::parser::xMismatchedParentheses(
                          c.Iter(), "[ParseLibDef] Mismatched braces."));
  }
  c.Advance();  // advance past the opening brace.
  // Parse until end of scope.
  while (c.Iter() != statement_scope.ContainedEnd()) {
    if (c.IsModifierKeyword() || c.IsDeclarativeKeyword()) {
      InternalParseResult decl_result = ParsePragmaticStmt(c);
      if (!decl_result) {
        return decl_result.ChainFailure(
            "[Parsing Global Declarative Statement]");
      }
      node.PushBack(decl_result.Extract());
      c.Advance(decl_result.Always().Iter());
    } else {
      return Failure(c, compiler_error::parser::xUserSyntaxError(
                            c.Iter(), "[Parsing Global Primary Statement]"));
    }
  }
  c.Advance();  // advance to scope end.
  // expect semicolon
  if (c.TypeIs(eTk::kSemicolon)) {
    c.Advance();
  } else {
    return Failure(c, compiler_error::parser::xExpectedToken(
                          ToStr(eTk::kSemicolon), c.Literal(),
                          "[ParseLibDef] Expected semicolon."));
  }
  return Success(c, node);
}

LarkParser::InternalParseResult LarkParser::ParseLibDef(TkCursor c) {
  // Format: <open_brace> <statement?*> <close_brace>
  Ast node(eAst::kLibraryDefinition);
  TkScope statement_scope = TkScope::find_brace(c);
  if (!statement_scope.Valid()) {
    return Failure(c, compiler_error::parser::xMismatchedParentheses(
                          c.Iter(), "[ParseLibDef] Mismatched braces."));
  }
  c.Advance();  // advance past the opening brace.
  // Parse until end of scope.
  while (c.Iter() != statement_scope.ContainedEnd()) {
    if (c.IsModifierKeyword() || c.IsDeclarativeKeyword()) {
      InternalParseResult decl_result = ParsePragmaticStmt(c);
      if (!decl_result) {
        return decl_result.ChainFailure(
            "[Parsing Global Declarative Statement]");
      }
      node.PushBack(decl_result.Extract());
      c.Advance(decl_result.Always().Iter());
    } else {
      return Failure(c, compiler_error::parser::xUserSyntaxError(
                            c.Iter(), "[Parsing Global Primary Statement]"));
    }
  }
  c.Advance();  // advance to scope end.
  // expect semicolon
  if (c.TypeIs(eTk::kSemicolon)) {
    c.Advance();
  } else {
    return Failure(c, compiler_error::parser::xExpectedToken(
                          ToStr(eTk::kSemicolon), c.Literal(),
                          "[ParseLibDef] Expected semicolon."));
  }
  return Success(c, node);
}

LarkParser::InternalParseResult LarkParser::ParseProgram(TkCursor c) {
  Ast program_node(eAst::kProgram);
  while (!c.AtEnd()) {
    if (c.IsModifierKeyword() || c.IsDeclarativeKeyword()) {
      InternalParseResult decl_result = ParsePragmaticStmt(c);
      if (!decl_result) {
        return decl_result.ChainFailure(
            "[Parsing Global Declarative Statement]");
      }
      program_node.PushBack(decl_result.Extract());
      c.Advance(decl_result.Always().Iter());
    } else {
      return Failure(c, compiler_error::parser::xUserSyntaxError(
                            c.Iter(), "[Parsing Global Primary Statement]"));
    }
  }
  return Success(c, program_node);
}

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
//   All Rights Reserved | Copyright 2024 NTONE INC.
//   Author: Anton Yashchenko
//   Email: ntondev@gmail.com
//   Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
//   Project: caoco
//   Directory: dir
//   File: lark_parser.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_COMPILER_LARK_PARSER_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//