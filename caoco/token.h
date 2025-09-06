#pragma once
#include "compiler_enum.h"
#include "import_stl.h"
#include "tk_traits.h"

class Tk {
 private:
  eTk type_{eTk::kInvalid};
  std::string literal_{""};
  std::size_t line_{0};
  std::size_t col_{0};

 public:
  // Modifiers
  constexpr void SetLine(std::size_t line) { line_ = line; }

  constexpr void SetCol(std::size_t col) { col_ = col; }
  // Properties
  constexpr eTk Type() const noexcept { return type_; }

  constexpr std::size_t Size() const noexcept { return literal_.size(); }

  constexpr std::size_t Line() const noexcept { return line_; }

  constexpr std::size_t Col() const noexcept { return col_; }

  constexpr const std::string& Literal() const noexcept { return literal_; }
  constexpr std::string& LiteralMutable() { return literal_; }
  // Parsing Utilities
  constexpr ePriority Priority() const noexcept {
    return tk_traits::kTkTypePriority(type_);
  };

  constexpr eAssoc Assoc() const noexcept {
    return tk_traits::kTkTypeAssoc(type_);
  }

  constexpr eOperation Operation() const noexcept {
    return tk_traits::kTkTypeOperation(type_);
  }
  // Fast type queries.
  constexpr bool TypeIs(eTk type) const noexcept { return type_ == type; }

  constexpr bool TypeAndLitIs(eTk kind,
                              const std::string& literal) const noexcept {
    return type_ == kind && literal_ == literal;
  }

  constexpr bool IsKeyword() const noexcept {
    return tk_traits::kTkTypeIsKeyword(type_);
  }

  inline constexpr bool IsModifierKeyword() const noexcept {
    return tk_traits::kTkTypeIsModifierKeyword(type_);
  }

  constexpr bool IsDeclarativeKeyword() const noexcept {
    return tk_traits::kTkTypeIsDeclarativeKeyword(type_);
  };
  constexpr bool IsSingularOperand() const noexcept {
    return tk_traits::kTkTypeIsSingularOperand(type_);
  };
  constexpr bool IsSingularPrefixOperator() const noexcept {
    return tk_traits::kTkTypeIsSingularPrefixOperator(type_);
  };
  constexpr bool IsOpeningScope() const noexcept {
    return tk_traits::kTkTypeIsOpeningScope(type_);
  };
  constexpr bool IsClosingScope() const noexcept {
    return tk_traits::kTkTypeIsClosingScope(type_);
  };
  constexpr bool IsClosingScopeOf(eTk topen) const noexcept {
    return tk_traits::kTkTypeIsClosingScopeOf(topen, type_);
  };
  constexpr bool IsPrimaryExpressionOpening() const noexcept {
    return tk_traits::kTkTypeIsPrimaryExpressionOpening(type_);
  };

  constexpr eAst NodeType() const noexcept {
    return tk_traits::kTkTypeToAstNodeType(type_);
  };

 public:
  constexpr Tk() noexcept : type_(eTk::kNone), line_(0), col_(0), literal_() {}
  constexpr Tk(eTk type) noexcept : type_(type), line_(0), col_(0) {}
  constexpr Tk(eTk type, std::vector<char>::const_iterator beg,
               std::vector<char>::const_iterator end) noexcept
      : type_(type), line_(0), col_(0) {
    literal_ = std::string(beg, end);
  }
  constexpr Tk(eTk type, std::vector<char>::const_iterator beg,
               std::vector<char>::const_iterator end, std::size_t line,
               std::size_t col) noexcept
      : type_(type), line_(line), col_(col) {
    literal_ = std::string(beg, end);
  }
  constexpr Tk(eTk type, std::string literal)
      : type_(type), line_(0), col_(0), literal_(literal) {}
  constexpr Tk(eTk type, std::string literal, std::size_t line, std::size_t col)
      : type_(type), line_(line), col_(col), literal_(literal) {}

  constexpr Tk(const Tk& other) noexcept
      : type_(other.type_),
        line_(other.line_),
        col_(other.col_),
        literal_(other.literal_) {}
  constexpr Tk(Tk&& other) noexcept
      : type_(other.type_),
        line_(other.line_),
        col_(other.col_),
        literal_(std::move(other.literal_)) {}
  constexpr auto operator=(const Tk& other) noexcept {
    type_ = other.type_;
    line_ = other.line_;
    col_ = other.col_;
    literal_ = other.literal_;
    return *this;
  }
  constexpr auto operator=(Tk&& other) noexcept {
    type_ = other.type_;
    line_ = other.line_;
    col_ = other.col_;
    literal_ = std::move(other.literal_);
    return *this;
  }
  constexpr bool operator==(const Tk& rhs) const {
    return type_ == rhs.type_ && literal_ == rhs.literal_;
  };
  constexpr bool operator!=(const Tk& rhs) const { return !(*this == rhs); };
};
