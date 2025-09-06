#pragma once
#include "import_stl.h"
#include "tk_traits.h"
#include "token.h"

class Ast {
 private:
  eAst type_{eAst::kInvalid};
  std::string literal_{""};
  Ast* parent_{nullptr};
  std::list<Ast> children_;

 public:
  Ast() : type_(eAst::kInvalid) {}

  Ast(eAst type) : type_(type) {}

  Ast(eAst type, const std::string& literal) : type_(type), literal_(literal) {}

  Ast(eAst type, std::string&& literal)
      : type_(type), literal_(std::move(literal)) {}

  Ast(eAst type, const char* literal) : type_(type), literal_(literal) {}

  template <std::size_t LIT_SIZE>
  Ast(eAst type, const char literal[LIT_SIZE])
      : type_(type), literal_(literal) {}

  Ast(const Tk& t);
  Ast(eAst type, std::vector<Tk>::iterator beg, std::vector<Tk>::iterator end);

  template <typename... ChildTs>
    requires(std::is_same_v<Ast, std::decay_t<ChildTs>> && ...)
  Ast(eAst type, const std::string& literal, ChildTs... children)
      : type_(type), literal_(literal) {
    (PushBack(children), ...);
  }

  template <typename... ChildTs>
    requires(std::is_same_v<Ast, std::decay_t<ChildTs>> && ...)
  Ast(eAst type, std::vector<Tk>::iterator beg, std::vector<Tk>::iterator end,
      ChildTs... children)
      : type_(type) {
    literal_ = u8"";
    for (std::vector<Tk>::iterator it = beg; it != end; it++) {
      literal_ += it->Literal();
    }
    (PushBack(children), ...);
  }

 public:
  // Properties
  constexpr eAst Type() const noexcept;
  constexpr const std::string& Literal() const noexcept;
  bool Leaf() const noexcept;
  constexpr bool Root() const noexcept;
  bool Branch() const noexcept;

  // Parent operations.
  Ast& Parent();
  constexpr void SetParent(Ast* parent);
  constexpr void PopParent();

  // Child operations.
  bool Empty() const noexcept;
  std::size_t Size() const;
  Ast& PushBack(const Ast& nd);
  Ast& PushFront(const Ast& nd);
  Ast& PushBack(Ast&& nd);
  Ast& PushFront(Ast&& nd);
  Ast PopBack();
  Ast PopFront();
  Ast& Front();
  Ast& Back();
  Ast& At(std::size_t index);
  const std::list<Ast>& Children() const;
  // Index operator accesses children.
  const Ast& operator[](std::size_t index) const;
  Ast& operator[](std::size_t index);

  //// Fast type queries.
  bool TypeIs(eAst type) const noexcept;
  bool TypeIsnt(eAst type) const noexcept;
  bool IsLiteral() const noexcept {
    return type_ == eAst::kStringLiteral || type_ == eAst::kNumberLiteral ||
           type_ == eAst::kDoubleLiteral || type_ == eAst::kBoolLiteral ||
           type_ == eAst::kByteLiteral || type_ == eAst::kNoneLiteral ||
           type_ == eAst::kTrueLiteral || type_ == eAst::kFalseLiteral;
  }
  bool IsArithmeticBinaryOp() const noexcept {
    return type_ == eAst::kAddition || type_ == eAst::kSubtraction ||
           type_ == eAst::kMultiplication || type_ == eAst::kDivision ||
           type_ == eAst::kRemainder;
  }
};

Ast::Ast(const Tk& t)
    : type_(tk_traits::kTkTypeToAstNodeType(t.Type())), literal_(t.Literal()) {}

Ast::Ast(eAst type, std::vector<Tk>::iterator beg,
         std::vector<Tk>::iterator end)
    : type_(type) {
  literal_ = "";
  for (std::vector<Tk>::iterator it = beg; it != end; it++) {
    literal_ += it->Literal();
  }
}

constexpr eAst Ast::Type() const noexcept { return type_; }

constexpr const std::string& Ast::Literal() const noexcept { return literal_; }

bool Ast::Leaf() const noexcept { return children_.empty(); }

constexpr bool Ast::Root() const noexcept { return parent_ == nullptr; }

bool Ast::Branch() const noexcept { return !children_.empty(); }

Ast& Ast::Parent() {
  if (parent_ != nullptr)
    return *parent_;
  else
    throw std::out_of_range("ast node parent() called on node with no parent.");
}

constexpr void Ast::SetParent(Ast* parent) { parent_ = parent; }

constexpr void Ast::PopParent() {
  if (parent_ != nullptr)
    parent_ = nullptr;
  else
    throw std::out_of_range(
        "Ast node PopParent() called on node with no parent.");
}

bool Ast::Empty() const noexcept { return children_.empty(); }
std::size_t Ast::Size() const { return children_.size(); }

Ast& Ast::PushBack(const Ast& nd) {
  children_.push_back(nd);
  auto& pushed = children_.back();
  pushed.SetParent(this);
  return pushed;
}

Ast& Ast::PushFront(const Ast& nd) {
  children_.push_front(nd);
  auto& pushed = children_.front();
  pushed.SetParent(this);
  return pushed;
}

Ast& Ast::PushBack(Ast&& nd) {
  children_.push_back(std::move(nd));
  auto& pushed = children_.back();
  pushed.SetParent(this);
  return pushed;
}

Ast& Ast::PushFront(Ast&& nd) {
  children_.push_front(std::move(nd));
  auto& pushed = children_.front();
  pushed.SetParent(this);
  return pushed;
}

Ast Ast::PopBack() {
  if (!children_.empty()) {
    Ast popped = children_.back();
    children_.pop_back();
    popped.PopParent();
    return popped;
  } else
    throw std::out_of_range(
        "Ast node pop_back() called on node with no children.");
}

Ast Ast::PopFront() {
  if (!children_.empty()) {
    Ast popped = children_.front();
    children_.pop_front();
    popped.PopParent();
    return popped;
  } else
    throw std::out_of_range(
        "Ast node pop_front() called on node with no children.");
}

Ast& Ast::Front() {
  if (!children_.empty())
    return children_.front();
  else
    throw std::out_of_range(
        "Ast node front() called on node with no children.");
}

Ast& Ast::Back() {
  if (!children_.empty())
    return children_.back();
  else
    throw std::out_of_range(
        "Ast node front() called on node with no children.");
}

Ast& Ast::At(std::size_t index) {
  if (index >= children_.size())
    throw std::out_of_range("Ast node at() called with index out of range.");
  if (index == 0) return children_.front();

  return [this, &index]() -> Ast& {
    auto it = children_.begin();
    for (std::size_t i = 0; i < index; i++) {
      it++;
    }
    return *it;
  }();
}

const std::list<Ast>& Ast::Children() const { return children_; }

const Ast& Ast::operator[](std::size_t index) const {
  if (index >= children_.size())
    throw std::out_of_range(
        "ast node [] index operator called with index out of range.");
  if (index == 0) return children_.front();

  return [this, &index]() -> const Ast& {
    auto it = children_.begin();
    for (std::size_t i = 0; i < index; i++) {
      it++;
    }
    return *it;
  }();
}

Ast& Ast::operator[](std::size_t index) {
  if (index >= children_.size())
    throw std::out_of_range(
        "ast node [] index operator called with index out of range.");
  if (index == 0) return children_.front();

  return [this, &index]() -> Ast& {
    auto it = children_.begin();
    for (std::size_t i = 0; i < index; i++) {
      it++;
    }
    return *it;
  }();
}

bool Ast::TypeIs(eAst type) const noexcept { return type_ == type; }

bool Ast::TypeIsnt(eAst type) const noexcept { return type_ != type; }
