//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: common
// File: compiler_error.h
//---------------------------------------------------------------------------//
// Brief: Lamdas to pretty-print compiler errors, warnings and notes.
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_COMMON_TOKEN_CLOSURE_H
#define HEADER_GUARD_CAOCO_COMMON_TOKEN_CLOSURE_H
// Includes:
#include "cand_syntax.h"
#include "import_stl.h"


//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

class Closure;
using TkList = std::list<Tk>;
using TkListIter = TkList::iterator;
using CosureList = std::list<Closure>;
using ClosureListIter = CosureList::iterator;

// A closure is a range of tokens representing a singular ast node.
// All tokens from front to back are a single ast node with no children.
// When a closure is a single token, the front and back are the same.
// Example: 1 + 2 -> Closure #2 is +, •+ is the front and +• is the back.
// Example: 1(abcd) + 2 -> Closure #2 is (abcd), •(abcd) is the front 
// and (abcd)• is the back.
class Closure {
  TkListIter front_;
  TkListIter back_;

 public:
  Closure(TkListIter front) : front_(front), back_(front) {}
  Closure(TkListIter front, TkListIter back) : front_(front), back_(back) {}

  // Operation of the front token. ( [ { are postfix operators.
  eOperation Operation() const {
    // Special case for (, [, {, and postfix operators.
    if (front_->TypeIs(eTk::kOpenParen) || front_->TypeIs(eTk::kOpenBrace) ||
        front_->TypeIs(eTk::kOpenBracket)) {
      return eOperation::kPostfix;
    } else {
      return front_->Operation();
    }
  }
  // Priority of the front token. ( [ { are postfix priority.
  ePriority Priority() const {
    // Special case for (, [, {, are postfix priority.
    if (front_->TypeIs(eTk::kOpenParen) || front_->TypeIs(eTk::kOpenBrace) ||
        front_->TypeIs(eTk::kOpenBracket)) {
      return ePriority::kPostfix;
    } else {
      return front_->Priority();
    }
  }
  // Associativity of the front token.
  eAssoc Assoc() const { return front_->Assoc(); }

  TkListIter front() const { return front_; }
  TkListIter back() const { return back_; }
};


class ClosureBuffer {
  static const Tk kSentinelBegin;
  TkList token_stream_;
  CosureList closures_;
  ClosureListIter sentinel_begin_closure_;
 public:
   ClosureBuffer() {
    // Create sentinel begin closure.
    // HAS to be set or else begin is the end before and after first insertion.
    token_stream_.push_back(kSentinelBegin);
    closures_.push_back(Closure(token_stream_.begin()));
    sentinel_begin_closure_ = closures_.begin();
   }
 public: // Closure Operations
  void PushFrontClosure(TkListIter front) { closures_.push_front(Closure(front)); }
  void PushFrontClosure(TkListIter front, TkListIter back) {
    closures_.push_front(Closure(front, back));
  }
  void PushBackClosure(TkListIter front) {
    closures_.push_back(Closure(front));
  }
  void PushBackClosure(TkListIter front, TkListIter back) {
    closures_.push_back(Closure(front, back));
  }
  void PopClosure() { closures_.pop_back(); }
  void PopClosure(ClosureListIter which) { closures_.erase(which);}
  void PopClosureIf(auto&& condition) {
    closures_.remove_if(std::move(condition));
  }

  // Returns the size of the internal closure list minus the sentinel begin.
  std::size_t ClosureCount() const { return closures_.size()-1; }
  // Returns iter to the last closure.(One before the end).
  ClosureListIter LastClosure() { return std::prev(closures_.end()); }
  // Returns the token iterator at the front of the last closure.
  TkListIter LastClosureFront() { return closures_.back().front(); }
  // Returns the token iterator at the back of the last closure.
  TkListIter LastClosureBack() { return closures_.back().back(); }

  // Search from the end of the closure list until first occurence.
  ClosureListIter FindClosureReverse(
      std::function<bool(Closure&)>&& condition) {
    return std::find_if(closures_.rbegin(), closures_.rend(), condition).base();
  }

  // Search from the end of the closure list for repeated occurences.
  std::vector<ClosureListIter> FindClosureReverseConsecutive(
      std::function<bool(ClosureListIter, ClosureListIter)>&& condition) {
    auto it = closures_.rbegin();
    auto end = closures_.rend();
    std::vector<ClosureListIter> accumulated_iterators;
    while (it != end) {
      auto next = std::next(it);
      if (condition(std::prev(it.base()), std::prev(next.base()))) {
        accumulated_iterators.push_back(std::prev(it.base()));
      } else {
        accumulated_iterators.push_back(std::prev(it.base()));
        break;
      }
      ++it;
    }
    return accumulated_iterators;
  }

  // Search from the end of the closure list for repeated occurences 
  // ignoring certain elements based on ignore_condition.
  std::vector<ClosureListIter> FindClosureReverseConsecutiveAndIgnore(
      std::function<bool(ClosureListIter, ClosureListIter)>&& condition,
      std::function<bool(ClosureListIter)>&& ignore_condition) {
    auto it = closures_.rbegin();
    auto end = closures_.rend();
    std::vector<ClosureListIter> accumulated_iterators;
    while (it != end) {
      auto next = std::next(it);
      if (ignore_condition(next.base())) {
        it++;
        continue;
      } else {
        if (condition(std::prev(it.base()), std::prev(next.base()))) {
          accumulated_iterators.push_back(std::prev(it.base()));
          it++;
        } else {
          accumulated_iterators.push_back(std::prev(it.base()));
          break;
        }
      }
    }
    return accumulated_iterators;
  }

 public:  // Token stream operations
  // Insert token before the front of the specified closure in the token stream.
  // Closure must be retrieved from this ClosureBuffer instance.
  void StreamInsertBeforeClosure(ClosureListIter closure_it, Tk token) {
    if (closure_it == sentinel_begin_closure_) {
      token_stream_.insert(std::next(token_stream_.begin()),token);
      return;
    }
    auto front = closure_it->front();
    token_stream_.insert(front, token);
  }

   // Insert token after the back of the specified closure in the token stream.
  // Closure must be retrieved from this ClosureBuffer instance.
  void StreamInsertAfterClosure(ClosureListIter closure_it, Tk token) {
    auto back = closure_it->back();
    auto next = std::next(back);
    if (next == token_stream_.end()) {
      token_stream_.push_back(token);
    } else {
      token_stream_.insert(next, token);
    }
  }

  // Get the last pushed token in the token stream.
  TkListIter LastStreamed() { return std::prev(token_stream_.end()); }

  void StreamPushBack(Tk token) { token_stream_.push_back(token); }

  TkVector StreamToVector() {
    // Temporarily remove sentinel begin token.
    token_stream_.pop_front();
    TkVector vec;
    vec.reserve(token_stream_.size());
    for (auto& tk : token_stream_) {
      vec.push_back(tk);
    }
    // Re-add sentinel begin token.
    token_stream_.push_front(kSentinelBegin);
    return vec;
  }
  //// Upon resolution all closures of higher priority than the head are resolved.
  //// When the head is at the end, the sentinel token is the head- which has the
  //// lowest priority. After a resolution or series of resolutions, a closure
  //// store and head advance should be performed.

  //// Resolves the last closure based on type-unchecked.
  //void resolve_next() {
  //  if (last_closure_front_tk().operation() == e_operation::prefix_) {
  //    resolve_prefix();
  //  } else if (last_closure_front_tk().operation() == e_operation::postfix_ ||
  //             last_closure_front_tk().type_is(e_tk::open_paren_) ||
  //             last_closure_front_tk().type_is(e_tk::open_brace_) ||
  //             last_closure_front_tk().type_is(
  //                 e_tk::open_bracket_)) {  // because ( [ { are postfix
  //                                          // operators.
  //    resolve_postfix();
  //  } else if (last_closure_front_tk().operation() == e_operation::binary_) {
  //    resolve_binary();
  //  } else {
  //    throw "Invalid token type in closure.";
  //  }
  //}

  //void resolve_postfix() {
  //  // Find concecutive postfix closures.
  //  auto nclosures =
  //      find_backward_consecutive([](ClosureListIter a, ClosureListIter b) {
  //        return a->priority() == b->priority();
  //      }).size();

  //  auto closure_count = nclosures;
  //  while (nclosures > 0) {
  //    output_insert_tk_after_closure(
  //        std::prev(last_closure(), static_cast<std::ptrdiff_t>(closure_count)),
  //        tk{e_tk::open_paren_, u8"("});
  //    output_insert_tk_after_closure(
  //        std::prev(last_closure(), static_cast<std::ptrdiff_t>(nclosures - 1)),
  //        tk{e_tk::close_paren_, u8")"});
  //    nclosures--;
  //  }

  //  for (sl_size i = 0; i < closure_count; i++) {
  //    pop();
  //  }
  //}

  //void resolve_binary_left_associative() {
  //  auto binary_closures = find_backward_consecutive_and_ignore(
  //      [](ClosureListIter a, ClosureListIter b) {
  //        return a->priority() == b->priority();
  //      },
  //      [](ClosureListIter a) {
  //        return ((a->priority() == priority::e_priority::postfix_) ||
  //                (a->priority() == priority::e_priority::prefix_));
  //      });
  //  priority::e_priority association_priority =
  //      binary_closures.back()->priority();
  //  // Resolve last
  //  auto open_paren_it = std::prev(binary_closures.back());
  //  output_insert_tk_after_closure(open_paren_it, tk{e_tk::open_paren_, u8"("});
  //  output.push_back(tk{e_tk::close_paren_, u8")"});

  //  auto close_paren_it = binary_closures.begin();
  //  // skip last in loop.
  //  while (close_paren_it != std::prev(binary_closures.end())) {
  //    output_insert_tk_after_closure(open_paren_it,
  //                                   tk{e_tk::open_paren_, u8"("});
  //    output_insert_tk_before_closure(*close_paren_it,
  //                                    tk{e_tk::close_paren_, u8")"});
  //    close_paren_it = std::next(close_paren_it);
  //  }

  //  // Pop all binary closures of the same priority.
  //  pop_if([&association_priority](closure& a) {
  //    return a.priority() == association_priority;
  //  });
  //  // for (auto i = 0; i < binary_closures.size(); i++) {
  //  //	pop();
  //  // }
  //}

  //void resolve_binary_right_associative() {
  //  auto binary_closures = find_backward_consecutive_and_ignore(
  //      [](ClosureListIter a, ClosureListIter b) {
  //        return a->priority() == b->priority();
  //      },
  //      [](ClosureListIter a) {
  //        return ((a->priority() == priority::e_priority::postfix_) ||
  //                (a->priority() == priority::e_priority::prefix_));
  //      });
  //  priority::e_priority association_priority =
  //      binary_closures.back()->priority();
  //  // Resolve last
  //  output_insert_tk_after_closure(std::prev(binary_closures.back()),
  //                                 tk{e_tk::open_paren_, u8"("});
  //  output.push_back(tk{e_tk::close_paren_, u8")"});

  //  auto open_paren_it = std::next(binary_closures.begin());
  //  while (open_paren_it != binary_closures.end()) {
  //    output_insert_tk_after_closure(*open_paren_it,
  //                                   tk{e_tk::open_paren_, u8"("});
  //    output.push_back(tk{e_tk::close_paren_, u8")"});
  //    open_paren_it = std::next(open_paren_it);
  //  }

  //  // Pop all binary closures of the same priority.
  //  pop_if([&association_priority](closure& a) {
  //    return a.priority() == association_priority;
  //  });
  //  // for (auto i = 0; i < binary_closures.size(); i++) {
  //  //	pop();
  //  // }
  //}

  //void resolve_binary() {
  //  assert(closures_.size() >= 2 &&
  //         closures_.front().front()->type_is(
  //             e_tk::none_));  // Sentinel begin closure must exist.

  //  // Resolve associative binary if there are repeated equivalent priority
  //  // closures before the last.
  //  if (std::prev(last_closure())->priority() ==
  //      last_closure_front_tk().priority()) {
  //    if (last_closure_front_tk().assoc() == e_assoc::left_) {
  //      resolve_binary_left_associative();
  //    } else if (last_closure_front_tk().assoc() == e_assoc::right_) {
  //      resolve_binary_right_associative();
  //    }
  //  } else {  // Resolve Single Binary Closure
  //    // If there are postfix closures-> skip them when looking for the open
  //    // paren location.
  //    auto postfix_it = std::prev(last_closure());
  //    while (postfix_it->priority() == priority::e_priority::postfix_) {
  //      postfix_it = std::prev(postfix_it);
  //    }

  //    auto prefix_it = postfix_it;
  //    if (postfix_it->priority() == priority::e_priority::prefix_) {
  //      while (prefix_it->priority() == priority::e_priority::prefix_) {
  //        prefix_it = std::prev(prefix_it);
  //      }
  //    }

  //    output_insert_tk_after_closure(prefix_it, tk{e_tk::open_paren_, u8"("});
  //    output.push_back(tk{e_tk::close_paren_, u8")"});
  //    pop();
  //  }
  //}

  //void resolve_prefix() {
  //  assert(closures_.size() >= 2 &&
  //         closures_.front().front()->type_is(
  //             e_tk::none_));  // Sentinel begin closure must exist.
  //  output_insert_tk_before_closure(last_closure(),
  //                                  tk{e_tk::open_paren_, u8"("});
  //  // binary_dump_.push_back(tk{ e_tk::close_paren_ ,u8")" });
  //  output.push_back(tk{e_tk::close_paren_, u8")"});
  //  pop();
  //}
};
const Tk ClosureBuffer::kSentinelBegin = {eTk::kNone, "ClosureBuffer sentinel begin."};



//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: common
// File: token_closure.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_COMMON_TOKEN_CLOSURE_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
