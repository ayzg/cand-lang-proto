#include <algorithm>
#include <any>
#include <concepts>
#include <cstdarg>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <vector>
#include <optional>

// Utilities
constexpr auto one_of(char c, std::initializer_list<char> list) {
  for (auto elem : list) {
    if (elem == c) return true;
  }
  return false;
}
void log(const std::string& message) { std::cout << message << std::endl; }

// Tokenizer Objects
struct Token;  // a fwd decl for typedefs

// Constants
namespace CandilConstants {
const auto keywords =
    std::set<std::string>{"def", "if", "else", "while", "for"};
}

// Token type enumerations
enum class eTokenType {
  invalid_,
  number_,
  alnumus_,
  symbol_,
  whitespace_,
  newline_,
  eof_,  // End of file token.

  // Phase 2
  integer_literal_,  // Integer literal value. cpp_type = unsigned long long.
  float_literal_,    // Floating point literal value. cpp_type = float.
  string_literal_,   // String literal value. cpp_type = std::string.
  directive_,  // '#' converts the following token into a compiler directive.
  operator_,   // Any symbol that is not a number, alnumus, whitespace, newline,
              // or eof, directive, scope start or scope end. Note: A symbol can
              // be a combination of symbols. eg. "+=". Symbols may be seperated
              // by whitespace.
  start_scope_,  // '('
  end_scope_,    // ')'
  separator_     // ',' used to seperate arguments.
};

// Typdefs
using token_uptr = std::unique_ptr<Token>;
using token_array_type = std::vector<token_uptr>;
using token_array_iterator = token_array_type::iterator;
using const_token_array_iterator = token_array_type::const_iterator;
template <typename TokenT>
  requires std::derived_from<TokenT, Token>
using optional_token_location_type =
    std::optional<std::tuple<TokenT, std::tuple<const_token_array_iterator,
                                                const_token_array_iterator>>>;


// Base Class Token
struct Token {
  using string_type = std::string;
  using value_type = std::any;
  using size_type = std::size_t;

  value_type value_;
  string_type literal_;
  size_type size_ = 1;  // Size of the token, combination tokens will have a
                        // size greater than 1.
  const_token_array_iterator end_;  // The end of the token in the Phase 1 token
                                    // array. End - Size = Begin.
  eTokenType type_ = eTokenType::invalid_;

  virtual ~Token(){};

  bool has_value() const { return value_.has_value(); }
  value_type value() const { return value_; }
  string_type literal() const { return literal_; };
  eTokenType type() const { return type_; }
  size_type length() const { return literal_.length(); }
  size_type size() const { return size_; }
  const_token_array_iterator begin() const { return end_ - size_; }
  const_token_array_iterator end() const { return end_; }
};

void log_token(const Token& token) {
  static std::map<eTokenType, std::string> token_type_map = {
      {eTokenType::invalid_, "invalid_"},
      {eTokenType::number_, "number_"},
      {eTokenType::alnumus_, "alnumus_"},
      {eTokenType::symbol_, "symbol_"},
      {eTokenType::whitespace_, "whitespace_"},
      {eTokenType::newline_, "newline_"},
      {eTokenType::eof_, "eof_"},
      {eTokenType::integer_literal_, "integer_literal_"},
      {eTokenType::float_literal_, "float_literal_"},
      {eTokenType::string_literal_, "string_literal_"},
      {eTokenType::directive_, "directive_"},
      {eTokenType::operator_, "operator_"},
      {eTokenType::start_scope_, "start_scope_"},
      {eTokenType::end_scope_, "end_scope_"},
      {eTokenType::separator_, "separator_"}};
  // print formatted token data
  std::cout << "<Tk: " << token_type_map[token.type()]
            << " | Lit: " << token.literal()
            << " | Val: " << token.value().type().name() << " >";
}

// Wrapper for a pointer to a token.
struct OptionalToken {
  token_uptr underlying_object_;

  bool valid() const {
    return underlying_object_.get()->type() != eTokenType::invalid_;
  }
  const token_uptr& get() const { return underlying_object_; }

  bool has_value() const { return underlying_object_.get()->has_value(); }
  eTokenType type() const { return underlying_object_.get()->type(); }
  Token::string_type literal() const {
    return underlying_object_.get()->literal();
  }
  Token::value_type value() const { return underlying_object_.get()->value(); }
  Token::size_type length() const { return underlying_object_.get()->length(); }
  Token::size_type size() const { return underlying_object_.get()->size(); }
  const_token_array_iterator begin() const {
    return underlying_object_.get()->begin();
  }
  const_token_array_iterator end() const {
    return underlying_object_.get()->end();
  }

  static token_uptr convert_to_token(OptionalToken& optional_token) {
    return std::move(optional_token.underlying_object_);
  }
};

bool assert_tokens(const token_array_type& expected,
                   const token_array_type& input) {
  if (expected.size() != input.size()) {
    log("ASSERT_TOKEN->expected token array size: " +
        std::to_string(expected.size()) +
        " Got: " + std::to_string(input.size()));
    return false;
  }

  for (auto i = 0; i < expected.size(); i++) {
    log_token(*expected[i].get());
    if (expected[i]->literal() != input[i]->literal()) {
      log("ASSERT_TOKEN->Expected Token Literal: " + expected[i]->literal() +
          " Got: " + input[i]->literal() + " At index: " + std::to_string(i));
      return false;
    }
    if (expected[i]->type() != input[i]->type()) {
      log("ASSERT_TOKEN->Expected Token Type: " +
          std::to_string(static_cast<int>(expected[i]->type())) +
          " Got: " + std::to_string(static_cast<int>(input[i]->type())) +
          " At index: " + std::to_string(i));
      return false;
    }
  }
  log("\nASSERT_TOKEN->Passed.");
  return true;
}

template <typename TokenT>
  requires std::derived_from<TokenT, Token>
inline token_uptr make_token_uptr(TokenT&& token) {
  return std::make_unique<TokenT>(std::move(token));
}

template <typename TokenT>
  requires std::derived_from<TokenT, Token>
inline token_uptr make_token_uptr(const TokenT& token) {
  return std::make_unique<TokenT>(token);
}

// Phase 1 Tokenization
namespace Tk1 {
struct Invalid : public Token {
  Invalid(std::string token_string = "") : Token() {
    this->literal_ = token_string;
    this->type_ = eTokenType::invalid_;
  }
};

struct Number : public Token {
  Number(std::string token_string) : Token() {
    this->literal_ = token_string;
    // TODO: For now we will use stoull and assume all numbers are unsigned long
    // long at this stage.
    this->value_ = token_string;
    this->type_ = eTokenType::number_;
  }
};

struct Alnumus : public Token {  // Alphanumeric and Underscore
  Alnumus(std::string alnumus) : Token() {
    this->type_ = eTokenType::alnumus_;
    this->literal_ = alnumus;
  }
};

struct Symbol : public Token {
  /* All other accepted symbol characters, will combine until next other token.
        eg. "1 += 1" -> "1" "+=" "1"
        eg. "1 + = 1" -> "1" "+" "=" "1"
        eg. "1 +_= 1" -> "1" "+" "_" "=" "1" // An underscore is considered an
     alnumus.*/
  Symbol(std::string symbol) : Token() {
    this->literal_ = symbol;
    this->type_ = eTokenType::symbol_;
  }
};

struct Whitespace : public Token {
  Whitespace(std::string whitespace) : Token() {
    this->literal_ = whitespace;
    this->type_ = eTokenType::whitespace_;
  }
};

struct Newline : public Token {
  Newline(std::string newline) : Token() {
    this->literal_ = newline;
    this->type_ = eTokenType::newline_;
  }
};

struct Eof : public Token {
  Eof() : Token() {
    this->literal_ = "";
    this->type_ = eTokenType::eof_;
  }
};

OptionalToken make_token(const std::string& source, std::size_t cursor_idx) {
  if (source[cursor_idx] == ' ') {
    std::stringstream current_token_stream;
    current_token_stream << source[cursor_idx];
    cursor_idx++;

    while (cursor_idx <= source.length() && source[cursor_idx] == ' ') {
      current_token_stream << source[cursor_idx];
      cursor_idx++;
    }
    return OptionalToken(
        make_token_uptr(Whitespace(current_token_stream.str())));
  } else if (source[cursor_idx] == '\n') {
    std::stringstream current_token_stream;
    current_token_stream << source[cursor_idx];
    cursor_idx++;

    while (cursor_idx <= source.length() && source[cursor_idx] == '\n') {
      current_token_stream << source[cursor_idx];
      cursor_idx++;
    }

    return OptionalToken(make_token_uptr(Newline(current_token_stream.str())));
  } else if (one_of(source[cursor_idx],
                    {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'})) {
    std::stringstream current_token_stream;
    current_token_stream << source[cursor_idx];
    cursor_idx++;

    while (cursor_idx <= source.length() &&
           one_of(source[cursor_idx],
                  {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'})) {
      current_token_stream << source[cursor_idx];
      cursor_idx++;
    }

    return OptionalToken(make_token_uptr(Number(current_token_stream.str())));
  } else if (one_of(source[cursor_idx],
                    {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
                     'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                     'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                     'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                     'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_'})) {
    std::stringstream current_token_stream;
    current_token_stream << source[cursor_idx];
    cursor_idx++;

    while (
        cursor_idx <= source.length() &&
        one_of(source[cursor_idx],
               {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                '_', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'})) {
      current_token_stream << source[cursor_idx];
      cursor_idx++;
    }

    return OptionalToken(make_token_uptr(Alnumus(current_token_stream.str())));
  } else if (one_of(source[cursor_idx],
                    {'+', '-', '*', '/', '%', '^', '&',  '|', '~',  '!',
                     '<', '>', '?', '.', ',', ';', ':',  '(', ')',  '[',
                     ']', '{', '}', '#', '@', '$', '\\', '"', '\'', '`'})) {
    std::stringstream current_token_stream;
    current_token_stream << source[cursor_idx];
    cursor_idx++;

    while (cursor_idx <= source.length() &&
           one_of(source[cursor_idx],
                  {'+', '-', '*', '/', '%', '^', '&',  '|', '~',  '!',
                   '<', '>', '?', '.', ',', ';', ':',  '(', ')',  '[',
                   ']', '{', '}', '#', '@', '$', '\\', '"', '\'', '`'})) {
      current_token_stream << source[cursor_idx];
      cursor_idx++;
    }

    return OptionalToken(make_token_uptr(Symbol(current_token_stream.str())));
  } else {
    return OptionalToken(
        make_token_uptr(Invalid(std::to_string(source[cursor_idx]))));
  }
}

token_array_type tokenize(const std::string& source) {
  if (source.empty()) throw std::runtime_error("Empty source.");

  auto cursor_idx = 0;
  auto tokens = token_array_type();

  while (cursor_idx < source.length()) {
    auto current_char = source[cursor_idx];
    auto optional_token = make_token(source, cursor_idx);

    if (optional_token.valid()) {
      cursor_idx += optional_token.length();
      tokens.push_back(std::move(OptionalToken::convert_to_token(
          optional_token)));  // Move the token uptr as it is no longer needed.
    } else {
      throw std::runtime_error("Invalid character or token in source file: " +
                               optional_token.literal());
    }
  }
  tokens.push_back(std::move(make_token_uptr(Eof())));
  return tokens;
}
}  // namespace Tk1

// Phase 2 Token Combination
namespace Tk2 {
struct IntegralLiteral : public Token {
  IntegralLiteral(std::string literal) : Token() {
    this->literal_ = literal;
    this->value_ = std::stoull(literal);
    this->type_ = eTokenType::integer_literal_;
  }

  IntegralLiteral(std::string literal, Token::size_type size,
                  const_token_array_iterator end)
      : Token() {
    this->literal_ = literal;
    this->value_ = std::stoull(literal);
    this->type_ = eTokenType::integer_literal_;
    this->size_ = size;
    this->end_ = end;
  }
};

struct FloatingLiteral : public Token {
  FloatingLiteral(std::string literal, Token::size_type size,
                  const_token_array_iterator end)
      : Token() {
    this->literal_ = literal;

    if (literal.back() == '.')
      this->value_ = std::stof(literal + "0");
    else
      this->value_ = std::stof(literal);

    this->type_ = eTokenType::integer_literal_;
    this->size_ = size;
    this->end_ = end;
  }

  // DO NOT USE THIS CONSTRUCTOR. IT IS ONLY FOR TESTING PURPOSES(initializing
  // random tokens).
  FloatingLiteral(std::string literal) : Token() {
    /* DO NOT USE THIS CONSTRUCTOR. IT IS ONLY FOR TESTING PURPOSES(initializing
     * random tokens).*/
    this->literal_ = literal;

    if (literal.back() == '.')
      this->value_ = std::stof(literal + "0");
    else
      this->value_ = std::stof(literal);

    this->type_ = eTokenType::integer_literal_;
    // this->size_ = size;
    // this->end_ = end;
  }
};

struct StringLiteral : public Token {
  StringLiteral(std::string literal) : Token() {
    this->literal_ = literal;
    this->value_ = literal.substr(
        1,
        literal.length() -
            1);  // Remove the first and last '\'' character from the literal.
    this->type_ = eTokenType::string_literal_;
  }
};

struct Directive : public Token {
  Directive(std::string keyword_literal, const_token_array_iterator end)
      : Token() {
    this->literal_ = "#" + keyword_literal;
    this->type_ = eTokenType::directive_;
    this->size_ = 2;
    this->end_ = end;
  }

  Directive(std::string keyword_literal) : Token() {
    this->literal_ = "#" + keyword_literal;
    this->type_ = eTokenType::directive_;
  }
};

struct ScopeStart : public Token {
  ScopeStart(std::string literal) : Token() {
    this->literal_ = literal;
    this->type_ = eTokenType::start_scope_;
  }
};

struct ScopeEnd : public Token {
  ScopeEnd(std::string literal) : Token() {
    this->literal_ = literal;
    this->type_ = eTokenType::end_scope_;
  }
};

struct Operator : public Token {
  Operator(std::string literal) : Token() {
    this->literal_ = literal;
    this->type_ = eTokenType::operator_;
  }
};

struct Separator : public Token {
  Separator(std::string literal) : Token() {
    this->literal_ = literal;
    this->type_ = eTokenType::separator_;
  }
};

OptionalToken construct_token(const_token_array_iterator token_it,
                              const_token_array_iterator p2_token_end_it) {
  // 1. Check if the current token is a number then find an integer literal or
  // floating point literal.
  if (token_it->get()->type() == eTokenType::number_) {
    auto next_token = token_it + 1;
    // Check if the next token is a period symbol.
    if (next_token->get()->type() == eTokenType::symbol_ &&
        next_token->get()->literal() == ".") {
      auto next_next_token = next_token + 1;
      // Check if the next next token is a number if so then we have a floating
      // literal.
      if (next_next_token->get()->type() == eTokenType::number_) {
        return OptionalToken(make_token_uptr(
            FloatingLiteral(token_it->get()->literal() + "." +
                                next_next_token->get()->literal(),
                            3, next_next_token)));
      }
      // Else the fractional part is equal to 0. End of token.
      else {
        return OptionalToken(make_token_uptr(
            FloatingLiteral(token_it->get()->literal() + ".", 2, next_token)));
      }
    }
    // Else the token is an integer literal.
    else {
      return OptionalToken(
          make_token_uptr(IntegralLiteral(token_it->get()->literal())));
    }
  }
  // 2. Check if the current token is an alnumus then pass it forward.
  else if (token_it->get()->type() == eTokenType::alnumus_) {
    return OptionalToken(
        make_token_uptr(Tk1::Alnumus(token_it->get()->literal())));
  }
  // 3. Check if the current token is a symbol then find one of the Phase 2
  // literal operators.
  else if (token_it->get()->type() == eTokenType::symbol_) {
    // 3.1 Check if the symbol is a string literal operator.
    if (token_it->get()->literal() == "\'") {
      auto beg = token_it + 1;
      auto end = beg;

      std::string literal = beg->get()->literal();
      while (end->get()->type() != eTokenType::symbol_ &&
             end->get()->literal() != "\'") {
        end++;
        literal += end->get()->literal();  // Consume all tokens until the next
                                           // string literal operator.
      }

      return OptionalToken(make_token_uptr(StringLiteral(literal)));
    }
    // 3.2 Check if the symbol is a compiler_directive. '#'
    else if (token_it->get()->literal() == "#") {
      // 3.2.2 Check if the next token is an identifier and  the literal is a
      // candil keyword.
      auto next_token = token_it + 1;
      if (next_token->get()->type() == eTokenType::alnumus_ &&
          CandilConstants::keywords.contains(next_token->get()->literal())) {
        return OptionalToken(make_token_uptr(
            Directive(next_token->get()->literal(), next_token)));
      }
      // 3.2.1 Else the token is invalid.
      else {
        return OptionalToken(
            make_token_uptr(Tk1::Invalid(token_it->get()->literal())));
      }
    }
    // 3.3 Check if the symbol is a opening parentheses, return a ScopeStart
    // operator.
    else if (token_it->get()->literal() == "(") {
      return OptionalToken(
          make_token_uptr(ScopeStart(token_it->get()->literal())));
    }
    // 3.4 Check if the symbol is a closing parentheses, return a ScopeStart
    // operator.
    else if (token_it->get()->literal() == ")") {
      return OptionalToken(
          make_token_uptr(ScopeEnd(token_it->get()->literal())));
    } else if (token_it->get()->literal() == ",") {
      return OptionalToken(
          make_token_uptr(Separator(token_it->get()->literal())));
    }
    // 3.5 Else the symbol is an operator.
    else {
      return OptionalToken(
          make_token_uptr(Operator(token_it->get()->literal())));
    }

  }
  // 4. Newline tokens will remain the same and pass through.
  else if (token_it->get()->type() == eTokenType::whitespace_ ||
           token_it->get()->type() == eTokenType::newline_) {
    return OptionalToken(std::make_unique<Token>(**token_it));
  }
  // Unrecognized token.
  else {
    return OptionalToken(
        make_token_uptr(Tk1::Invalid(token_it->get()->literal())));
  }
}

auto construct(const std::vector<std::unique_ptr<Token>>& tokens) {
  token_array_type output_tokens;
  auto token_it = tokens.cbegin();

  while (token_it->get()->type() != eTokenType::eof_) {
    auto optional_token = construct_token(token_it, output_tokens.cend());

    if (optional_token.valid()) {
      // Set the iterator to the next token.
      if (optional_token.size() == 1)
        token_it += optional_token.size();
      else
        token_it = optional_token.end() + 1;

      // Add the current token to the output.
      output_tokens.push_back(std::move(OptionalToken::convert_to_token(
          optional_token)));  // Move the token uptr as it is no longer needed.
    } else {
      throw std::runtime_error("Invalid character or token in source file: " +
                               optional_token.literal());
    }
  }
  output_tokens.push_back(std::move(make_token_uptr(Tk1::Eof())));
  return output_tokens;
}

}  // namespace Tk2

// Phase 3 Token Evaluation
namespace Tk3 {

struct TokenEvaluationResult {
  std::size_t popped_count = 0;
  std::size_t cursor_offset = 0;
  token_array_type output_tokens;
};

struct ExpressionEvaluationResult {
  token_array_type tokens;  // The tokens that make up the resulting expression.
  bool solved = false;      // If the expression was solved.
};

bool solve_arithmetic_expression(token_array_type& expression_tokens) {
  // If we have an expression then we attempt to solve it.
  if (expression_tokens.size() > 1) {
    // Solve all binary arithmetic expressions.
    // Solve all multiplicative expressions.
    for (auto i = 0; i < expression_tokens.size(); i++) {
      if (expression_tokens[i]->type() == eTokenType::operator_) {
        if (expression_tokens[i]->literal() == "*") {
          auto lhs = std::any_cast<unsigned long long>(
              expression_tokens[i - 1]->value());
          auto rhs = std::any_cast<unsigned long long>(
              expression_tokens[i + 1]->value());
          auto result = lhs * rhs;
          expression_tokens[i - 1] = std::move(
              make_token_uptr(Tk2::IntegralLiteral(std::to_string(result))));
          expression_tokens.erase(expression_tokens.begin() + i,
                                  expression_tokens.begin() + i + 2);
          i--;
        } else if (expression_tokens[i]->literal() == "/") {
          auto lhs = std::any_cast<unsigned long long>(
              expression_tokens[i - 1]->value());
          auto rhs = std::any_cast<unsigned long long>(
              expression_tokens[i + 1]->value());
          auto result = lhs / rhs;
          expression_tokens[i - 1] = std::move(
              make_token_uptr(Tk2::IntegralLiteral(std::to_string(result))));
          expression_tokens.erase(expression_tokens.begin() + i,
                                  expression_tokens.begin() + i + 2);
          i--;
        } else if (expression_tokens[i]->literal() == "%") {
          auto lhs = std::any_cast<unsigned long long>(
              expression_tokens[i - 1]->value());
          auto rhs = std::any_cast<unsigned long long>(
              expression_tokens[i + 1]->value());
          auto result = lhs % rhs;
          expression_tokens[i - 1] = std::move(
              make_token_uptr(Tk2::IntegralLiteral(std::to_string(result))));
          expression_tokens.erase(expression_tokens.begin() + i,
                                  expression_tokens.begin() + i + 2);
          i--;
        }
      }
    }

    // Solve all additive expressions.
    for (auto i = 0; i < expression_tokens.size(); i++) {
      if (expression_tokens[i]->type() == eTokenType::operator_) {
        if (expression_tokens[i]->literal() == "+") {
          auto lhs = std::any_cast<unsigned long long>(
              expression_tokens[i - 1]->value());
          auto rhs = std::any_cast<unsigned long long>(
              expression_tokens[i + 1]->value());
          auto result = lhs + rhs;
          expression_tokens[i - 1] = std::move(
              make_token_uptr(Tk2::IntegralLiteral(std::to_string(result))));
          expression_tokens.erase(expression_tokens.begin() + i,
                                  expression_tokens.begin() + i + 2);
          i--;
        } else if (expression_tokens[i]->literal() == "-") {
          auto lhs = std::any_cast<unsigned long long>(
              expression_tokens[i - 1]->value());
          auto rhs = std::any_cast<unsigned long long>(
              expression_tokens[i + 1]->value());
          auto result = lhs - rhs;
          expression_tokens[i - 1] = std::move(
              make_token_uptr(Tk2::IntegralLiteral(std::to_string(result))));
          expression_tokens.erase(expression_tokens.begin() + i,
                                  expression_tokens.begin() + i + 2);
          i--;
        }
      }
    }
    return true;
  }  // end if (expression_tokens.size() > 1)
  else {
    return true;
  }
}

TokenEvaluationResult solve_bin_arithmetic(
    const token_array_type& tokens, std::size_t cursor_idx,
    const token_array_type& current_output_tokens) {
  TokenEvaluationResult result_tokens;
  const auto& curr_token = tokens[cursor_idx];

  // If we have an integerl literal then we attempt to scan in an arithmetic
  // expression.
  token_array_type expression_tokens;
  if (curr_token->type() == eTokenType::integer_literal_) {
    auto expression_end = cursor_idx;

    while (tokens[expression_end]->type() != eTokenType::newline_ &&
           tokens[expression_end]->type() != eTokenType::end_scope_ &&
           tokens[expression_end]->type() != eTokenType::eof_) {
      expression_tokens.push_back(
          std::move(make_token_uptr(*tokens[expression_end])));
      expression_end++;
    }
    auto expression_result = solve_arithmetic_expression(expression_tokens);
    result_tokens.cursor_offset = expression_end - cursor_idx;
    // result_tokens.popped_count = 1;
    result_tokens.output_tokens = std::move(expression_tokens);
  } else if (curr_token->type() == eTokenType::operator_) {
    // If the current token is an operator then we attempt to create an
    // expression with the last output token as the rhs. Check if the last token
    // in the output is an integer literal. Else we cannot solve the expression.
    if (current_output_tokens.back()->type() == eTokenType::integer_literal_) {
      auto expression_end = cursor_idx + 1;

      while (tokens[expression_end]->type() != eTokenType::newline_ &&
             tokens[expression_end]->type() != eTokenType::end_scope_ &&
             tokens[expression_end]->type() != eTokenType::eof_) {
        expression_tokens.push_back(
            std::move(make_token_uptr(*tokens[expression_end])));
        expression_end++;
      }
      auto expression_result = solve_arithmetic_expression(expression_tokens);
      result_tokens.cursor_offset = expression_end - cursor_idx;
      result_tokens.popped_count = 1;
      result_tokens.output_tokens = std::move(expression_tokens);
    } else {
      result_tokens.cursor_offset = 1;
      result_tokens.output_tokens.push_back(
          std::move(make_token_uptr(*curr_token)));
    }

  } else if (curr_token->type() == eTokenType::start_scope_ ||
             curr_token->type() == eTokenType::end_scope_ ||
             curr_token->type() == eTokenType::newline_ ||
             curr_token->type() == eTokenType::eof_) {
    result_tokens.cursor_offset = 1;
    result_tokens.output_tokens.push_back(
        std::move(make_token_uptr(*curr_token)));
  }
  return result_tokens;
}

auto evaluate(const token_array_type& tokens) {
  token_array_type output_tokens;
  auto token_it = tokens.cbegin();

  // Reconstruct tokens filtering redundant tokens.
  // Filter whitespace
  token_array_type filtered_tokens;
  for (const auto& token : tokens) {
    if (token->type() != eTokenType::whitespace_) {
      filtered_tokens.push_back(std::move(make_token_uptr(*token)));
    }
  }

  // Evaluate filtered tokens.
  std::size_t cursor_idx = 0;
  while (filtered_tokens[cursor_idx]->type() != eTokenType::eof_) {
    const auto& curr_token = filtered_tokens[cursor_idx];

    auto result =
        solve_bin_arithmetic(filtered_tokens, cursor_idx, output_tokens);

    cursor_idx += result.cursor_offset;

    for (auto i = 0; i < result.popped_count; i++) {
      output_tokens.pop_back();
    }

    output_tokens.insert(output_tokens.end(),
                         std::make_move_iterator(result.output_tokens.begin()),
                         std::make_move_iterator(result.output_tokens.end()));

    // Remove redundant scopes from the output. Any scopes with a single token
    // inside are redundant.
    for (auto out_it = output_tokens.begin(); out_it != output_tokens.end();) {
      if (out_it->get()->type() == eTokenType::start_scope_) {
        if (output_tokens.size() > 2) {  // Check if we have enough tokens to
                                         // check for a redundant scope.
          if ((out_it + 2)->get()->type() == eTokenType::end_scope_) {
            out_it = output_tokens.erase(out_it);
            if ((out_it + 1) <= output_tokens.end())
              out_it = output_tokens.erase(out_it + 1);
          } else {
            ++out_it;
          }
        } else if (output_tokens.size() > 1) {
          if ((out_it + 1)->get()->type() == eTokenType::end_scope_) {
            out_it = output_tokens.erase(out_it);
            output_tokens.erase(out_it);

          } else {
            ++out_it;
          }
        } else {
          ++out_it;
        }
      } else {
        ++out_it;
      }
    }

  }  // end evaluate filtered tokens.

  output_tokens.push_back(std::move(make_token_uptr(Tk1::Eof())));
  return output_tokens;

}  // end auto evaluate(const token_array_type& tokens)
}  // end namespace Tk3

TEST(TestCase_EmptySourceError, Test_TokenizerBasic) {
  EXPECT_ANY_THROW(Tk1::tokenize(""));
}

TEST(TestCase_InvalidCharacterError, Test_TokenizerBasic) {
  EXPECT_ANY_THROW(
      Tk1::tokenize("ñ"));  // The ninha character is not a valid character.
}

TEST(TestCase_TokenizeWhitespace_Single, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize(" ");
  EXPECT_TRUE(assert_tokens(expected, tokens));
}

TEST(TestCase_TokenizeWhitespace_Double, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Whitespace("  ")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("  ");
  EXPECT_TRUE(assert_tokens(expected, tokens));
}

TEST(TestCase_TokenizeWhitespace_Many, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Whitespace("          ")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("          ");
  EXPECT_TRUE(assert_tokens(expected, tokens));
}

TEST(TestCase_TokenizeNewline_Single, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Newline("\n")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("\n");
  EXPECT_TRUE(assert_tokens(expected, tokens));
}

TEST(TestCase_TokenizeNewline_Double, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Newline("\n\n")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("\n\n");
  EXPECT_TRUE(assert_tokens(expected, tokens));
}

TEST(TestCase_TokenizeNewline_Many, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Newline("\n\n\n\n\n\n\n\n\n")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("\n\n\n\n\n\n\n\n\n");
  EXPECT_TRUE(assert_tokens(expected, tokens));
}

TEST(TestCase_TokenizeNumber_Single, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Number("1")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("1");
  EXPECT_TRUE(assert_tokens(expected, tokens));

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk1::Number("1")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens2 = Tk1::tokenize(" 1");
  EXPECT_TRUE(assert_tokens(expected2, tokens2));

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens3 = Tk1::tokenize(" 1 ");
  EXPECT_TRUE(assert_tokens(expected3, tokens3));
}

TEST(TestCase_TokenizeNumber_Double, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Number("16")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("16");
  EXPECT_TRUE(assert_tokens(expected, tokens));

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk1::Number("16")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens2 = Tk1::tokenize(" 16");
  EXPECT_TRUE(assert_tokens(expected2, tokens2));

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens3 = Tk1::tokenize(" 16 ");
  EXPECT_TRUE(assert_tokens(expected3, tokens3));
}

TEST(TestCase_TokenizeNumber_Many, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Number("1234567890")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("1234567890");
  EXPECT_TRUE(assert_tokens(expected, tokens));

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk1::Number("1234567890")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens2 = Tk1::tokenize(" 1234567890");
  EXPECT_TRUE(assert_tokens(expected2, tokens2));

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens3 = Tk1::tokenize(" 1234567890 ");
  EXPECT_TRUE(assert_tokens(expected3, tokens3));
}

TEST(TestCase_TokenizeAlnumus_Single, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Alnumus("a")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("a");
  EXPECT_TRUE(assert_tokens(expected, tokens));

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk1::Alnumus("a")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens2 = Tk1::tokenize(" a");
  EXPECT_TRUE(assert_tokens(expected2, tokens2));

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens3 = Tk1::tokenize(" a ");
  EXPECT_TRUE(assert_tokens(expected3, tokens3));
}

TEST(TestCase_TokenizeAlnumus_Double, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Alnumus("a_")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("a_");
  EXPECT_TRUE(assert_tokens(expected, tokens));

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk1::Alnumus("a_")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens2 = Tk1::tokenize(" a_");
  EXPECT_TRUE(assert_tokens(expected2, tokens2));

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens3 = Tk1::tokenize(" a_ ");
  EXPECT_TRUE(assert_tokens(expected3, tokens3));
}

TEST(TestCase_TokenizeAlnumus_Many, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Alnumus(
      "_abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize(
      "_abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");
  EXPECT_TRUE(assert_tokens(expected, tokens));

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk1::Alnumus(
      "_abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens2 = Tk1::tokenize(
      " _abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");
  EXPECT_TRUE(assert_tokens(expected2, tokens2));

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens3 = Tk1::tokenize(
      " _abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_ ");
  EXPECT_TRUE(assert_tokens(expected3, tokens3));
}

TEST(TestCase_TokenizeSymbol_Single, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Symbol("+")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("+");
  EXPECT_TRUE(assert_tokens(expected, tokens));

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk1::Symbol("+")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens2 = Tk1::tokenize(" +");
  EXPECT_TRUE(assert_tokens(expected2, tokens2));

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens3 = Tk1::tokenize(" + ");
  EXPECT_TRUE(assert_tokens(expected3, tokens3));
}

TEST(TestCase_TokenizeSymbol_Double, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Symbol("+-")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("+-");
  EXPECT_TRUE(assert_tokens(expected, tokens));

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk1::Symbol("+-")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens2 = Tk1::tokenize(" +-");
  EXPECT_TRUE(assert_tokens(expected2, tokens2));

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens3 = Tk1::tokenize(" +- ");
  EXPECT_TRUE(assert_tokens(expected3, tokens3));
}

TEST(TestCase_TokenizeSymbol_Many, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Symbol("+-*/%")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("+-*/%");
  EXPECT_TRUE(assert_tokens(expected, tokens));

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk1::Symbol("+-*/%")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens2 = Tk1::tokenize(" +-*/%");
  EXPECT_TRUE(assert_tokens(expected2, tokens2));

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens3 = Tk1::tokenize(" +-*/% ");
  EXPECT_TRUE(assert_tokens(expected3, tokens3));
}

TEST(TestCase_TokenizeMultipleTypes, Test_TokenizerBasic) {
  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk1::Number("1")));
  expected.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected.push_back(make_token_uptr(Tk1::Alnumus("a")));
  expected.push_back(make_token_uptr(Tk1::Symbol("+")));
  expected.push_back(make_token_uptr(Tk1::Newline("\n")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens = Tk1::tokenize("1 a+\n");
  EXPECT_TRUE(assert_tokens(expected, tokens));

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Number("1")));
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk1::Alnumus("a")));
  expected2.push_back(make_token_uptr(Tk1::Symbol("+")));
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk1::Newline("\n")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens2 = Tk1::tokenize("1 a+ \n");
  EXPECT_TRUE(assert_tokens(expected2, tokens2));

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  auto tokens3 = Tk1::tokenize("1 a+ \n ");
  EXPECT_TRUE(assert_tokens(expected3, tokens3));
}

TEST(TestCase_TokenizeSampleSource, Test_TokenizerBasic) {
  auto source = std::string(
      "#def(my_var,string,\"125854\") \n "
      "#def(my_var2,string,\"125854\") \n "
      "#call(#add,my_var,my_var2) \n ");

  EXPECT_NO_THROW(Tk1::tokenize(source));
}

TEST(TestCase_TokenizeSampleBadSource, Test_TokenizerBasic) {
  auto source = std::string(
      "#def(my_var,string,\"125854\") \n "
      "#def(my_var2,string,\"125854\") \n "
      "#call(#add,my_var,my_var2) ñ\n ");

  EXPECT_ANY_THROW(Tk1::tokenize(source));
}

TEST(TestCase_EvaluateIntegralLiteral_Single, Test_TokenEvaluatorBasic) {
  auto tokens = Tk1::tokenize("1");

  auto constructed_tokens = Tk2::construct(tokens);

  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk2::IntegralLiteral("1")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected, constructed_tokens));

  auto tokens2 = Tk1::tokenize(" 1");

  auto constructed_tokens2 = Tk2::construct(tokens2);

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk2::IntegralLiteral("1")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected2, constructed_tokens2));

  auto tokens3 = Tk1::tokenize(" 1 ");

  auto constructed_tokens3 = Tk2::construct(tokens3);

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected3, constructed_tokens3));
}

TEST(TestCase_EvaluateIntegralLiteral_Double, Test_TokenEvaluatorBasic) {
  auto tokens = Tk1::tokenize("16");

  auto constructed_tokens = Tk2::construct(tokens);

  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk2::IntegralLiteral("16")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected, constructed_tokens));

  auto tokens2 = Tk1::tokenize(" 16");

  auto constructed_tokens2 = Tk2::construct(tokens2);

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk2::IntegralLiteral("16")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected2, constructed_tokens2));

  auto tokens3 = Tk1::tokenize(" 16 ");

  auto constructed_tokens3 = Tk2::construct(tokens3);

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected3, constructed_tokens3));
}

TEST(TestCase_EvaluateIntegralLiteral_Many, Test_TokenEvaluatorBasic) {
  auto tokens = Tk1::tokenize("1645345 1");

  auto constructed_tokens = Tk2::construct(tokens);

  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk2::IntegralLiteral("1645345")));
  expected.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected.push_back(make_token_uptr(Tk2::IntegralLiteral("1")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected, constructed_tokens));

  auto tokens2 = Tk1::tokenize(" 1645345");

  auto constructed_tokens2 = Tk2::construct(tokens2);

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk2::IntegralLiteral("1645345")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected2, constructed_tokens2));

  auto tokens3 = Tk1::tokenize(" 1645345 ");

  auto constructed_tokens3 = Tk2::construct(tokens3);

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected3, constructed_tokens3));
}

TEST(TestCase_EvaluateFractionalLiteral_Single, Test_TokenEvaluatorBasic) {
  auto tokens = Tk1::tokenize("1.0");

  auto constructed_tokens = Tk2::construct(tokens);

  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk2::FloatingLiteral("1.0")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected, constructed_tokens));

  auto tokens2 = Tk1::tokenize(" 1.0");

  auto constructed_tokens2 = Tk2::construct(tokens2);

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk2::FloatingLiteral("1.0")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected2, constructed_tokens2));

  auto tokens3 = Tk1::tokenize(" 1.0 ");

  auto constructed_tokens3 = Tk2::construct(tokens3);

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected3, constructed_tokens3));
}

TEST(TestCase_EvaluateFractionalLiteral_SingleNoFraction,
     Test_TokenEvaluatorBasic) {
  auto tokens = Tk1::tokenize("1.");

  auto constructed_tokens = Tk2::construct(tokens);

  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk2::FloatingLiteral("1.")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected, constructed_tokens));

  auto tokens2 = Tk1::tokenize(" 1.");

  auto constructed_tokens2 = Tk2::construct(tokens2);

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk2::FloatingLiteral("1.")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected2, constructed_tokens2));

  auto tokens3 = Tk1::tokenize(" 1. ");

  auto constructed_tokens3 = Tk2::construct(tokens3);

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected3, constructed_tokens3));
}

TEST(TestCase_EvaluateFractionalLiteral_Double, Test_TokenEvaluatorBasic) {
  auto tokens = Tk1::tokenize("16.06");

  auto constructed_tokens = Tk2::construct(tokens);

  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk2::FloatingLiteral("16.06")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected, constructed_tokens));

  auto tokens2 = Tk1::tokenize(" 16.06");

  auto constructed_tokens2 = Tk2::construct(tokens2);

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(make_token_uptr(Tk2::FloatingLiteral("16.06")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected2, constructed_tokens2));

  auto tokens3 = Tk1::tokenize(" 16.06 ");

  auto constructed_tokens3 = Tk2::construct(tokens3);

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected3, constructed_tokens3));
}

TEST(TestCase_EvaluateFractionalLiteral_Many, Test_TokenEvaluatorBasic) {
  auto tokens = Tk1::tokenize("111111.1111111111");

  auto constructed_tokens = Tk2::construct(tokens);

  auto expected = token_array_type();
  expected.push_back(
      make_token_uptr(Tk2::FloatingLiteral("111111.1111111111")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected, constructed_tokens));

  auto tokens2 = Tk1::tokenize(" 111111.1111111111");

  auto constructed_tokens2 = Tk2::construct(tokens2);

  auto expected2 = token_array_type();
  expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected2.push_back(
      make_token_uptr(Tk2::FloatingLiteral("111111.1111111111")));
  expected2.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected2, constructed_tokens2));

  auto tokens3 = Tk1::tokenize(" 111111.1111111111 ");

  auto constructed_tokens3 = Tk2::construct(tokens3);

  auto expected3 = std::move(expected2);
  expected3.pop_back();
  expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
  expected3.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected3, constructed_tokens3));
}

TEST(TestCase_EvaluateFractionalLiteral_OverflowError,
     Test_TokenEvaluatorBasic) {
  EXPECT_THROW(Tk2::FloatingLiteral(
                   "339999999999999999996123846586046231866666666666671489.0"),
               std::out_of_range);
}

TEST(TestCase_EvaluateIntegralLiteral_OverflowError, Test_TokenEvaluatorBasic) {
  EXPECT_THROW(Tk2::IntegralLiteral("18446744073709551616"), std::out_of_range);
  EXPECT_THROW(Tk2::IntegralLiteral("184467440737095516150"),
               std::out_of_range);
  EXPECT_NO_THROW(Tk2::IntegralLiteral("18446744073709551615"));
}

TEST(TestCase_EvaluateDirective, Test_TokenEvaluatorBasic) {
  auto tokens = Tk1::tokenize("#def");

  auto constructed_tokens = Tk2::construct(tokens);

  auto expected = token_array_type();
  expected.push_back(make_token_uptr(Tk2::Directive("def")));
  expected.push_back(make_token_uptr(Tk1::Eof()));

  EXPECT_TRUE(assert_tokens(expected, constructed_tokens));
}

TEST(TestCase_ReplFromFile, Test_CandilLanguage) {
  std::string input;

  std::cout << "Compiling \"main.candi\" "
               "..............................................................."
               "..........: ";
  // std::getline(std::cin, input);
  std::ifstream file("main.candi");

  file.open(input);

  // Convert file to string
  std::string source((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());

  auto tokens = Tk1::tokenize(source);
  auto constructed_tokens = Tk2::construct(tokens);
  auto evaluated_tokens = Tk3::evaluate(constructed_tokens);

  std::cout << "Constructed Tokens: " << std::endl;
  for (auto& token : constructed_tokens) {
    log_token(*token.get());
  }
  std::cout << std::endl;
  std::cout << "Evaluated Tokens: " << std::endl;
  for (auto& token : evaluated_tokens) {
    log_token(*token.get());
  }
  std::cout << std::endl;
}

// TEST(TestCase8_TokenizeInteger, Test_TokenizerBasic) {
//     auto expected = token_array_type();
//     expected.push_back(make_token_uptr(Tk1::Number("1")));
//     expected.push_back(make_token_uptr(Tk1::Eof()));
//
//     auto tokens = Tk1::tokenize("1");
//     EXPECT_TRUE(assert_tokens(expected, tokens));
//
//     auto expected2 = token_array_type();
//     expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
//	expected2.push_back(make_token_uptr(Tk1::Number("1")));
//     expected2.push_back(make_token_uptr(Tk1::Eof()));
//
//     auto tokens2 = Tk1::tokenize(" 1");
//     EXPECT_TRUE(assert_tokens(expected2, tokens2));
//
//     auto expected3 = std::move(expected2);
//     expected3.pop_back();
//     expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
//     expected3.push_back(make_token_uptr(Tk1::Eof()));
//
//     auto tokens3 = Tk1::tokenize(" 1 ");
//     EXPECT_TRUE(assert_tokens(expected3, tokens3));
// }
//
// TEST(TestCase_TokenizeAlnumus, Test_TokenizerBasic) {
//     auto expected = token_array_type();
//	expected.push_back(make_token_uptr(Tk1::Alnumus("a")));
//	expected.push_back(make_token_uptr(Tk1::Eof()));
//
//	auto tokens = Tk1::tokenize("a");
//	EXPECT_TRUE(assert_tokens(expected, tokens));
//
//	auto expected2 = token_array_type();
//	expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
//	expected2.push_back(make_token_uptr(Tk1::Alnumus("a")));
//	expected2.push_back(make_token_uptr(Tk1::Eof()));
//
//	auto tokens2 = Tk1::tokenize(" a");
//	EXPECT_TRUE(assert_tokens(expected2, tokens2));
//
//	auto expected3 = std::move(expected2);
//	expected3.pop_back();
//	expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
//	expected3.push_back(make_token_uptr(Tk1::Eof()));
//
//	auto tokens3 = Tk1::tokenize(" a ");
//	EXPECT_TRUE(assert_tokens(expected3, tokens3));
// }
//
// TEST(TestCase_TokenizeSymbol, Test_TokenizerBasic) {
//	auto expected = token_array_type();
//	expected.push_back(make_token_uptr(Tk1::Symbol("+~!")));
//	expected.push_back(make_token_uptr(Tk1::Eof()));
//
//	auto tokens = Tk1::tokenize("+~!");
//	EXPECT_TRUE(assert_tokens(expected, tokens));
//
//	auto expected2 = token_array_type();
//	expected2.push_back(make_token_uptr(Tk1::Whitespace(" ")));
//	expected2.push_back(make_token_uptr(Tk1::Symbol("+~!")));
//	expected2.push_back(make_token_uptr(Tk1::Eof()));
//
//	auto tokens2 = Tk1::tokenize(" +~!");
//	EXPECT_TRUE(assert_tokens(expected2, tokens2));
//
//	auto expected3 = std::move(expected2);
//	expected3.pop_back();
//	expected3.push_back(make_token_uptr(Tk1::Whitespace(" ")));
//	expected3.push_back(make_token_uptr(Tk1::Eof()));
//
//	auto tokens3 = Tk1::tokenize(" +~! ");
//	EXPECT_TRUE(assert_tokens(expected3, tokens3));
// }

// struct P2OperatorAddition : public Token {
//     /*
//     * Phase 2 Addition operator is a binary operator that adds two operands.
//     * The operands must be a P2OperandIntegerLiteral or
//     P2OperandFloatLiteral, or P2OperandStringLiteral.
//     * The operands must be of the same token type.
//     * The result is the same token type as the operands.
//     *
//     * The operand cannot be an identifier(P2OperandIdentifier).
//     *
//     */
//
//     std::vector<std::unique_ptr<Token>>::const_iterator lhs_;
//     std::vector<std::unique_ptr<Token>>::const_iterator rhs_;
//
//     P2OperatorAddition(std::vector<std::unique_ptr<Token>>::const_iterator
//     lhs, std::vector<std::unique_ptr<Token>>::const_iterator rhs) : Token() {
//         this->literal_ = lhs->get()->literal() + '.' + rhs->get()->literal();
//         this->lhs_ = lhs;
//         this->rhs_ = rhs;
//     }
//
//     optional_token_location<P2OperandIntegerLiteral> solve() {
//		//return std::make_tuple(P2OperandIntegerLiteral(this->literal()),
//std::make_tuple(lhs_, rhs_));
//	}
//
//     static bool begin_lex(std::vector<std::unique_ptr<Token>>::const_iterator
//     token_it) {
//         return token_it->get()->type() == eTokenType::operator_ &&
//         token_it->get()->literal() == "+";
//     }
//
//     static std::optional<P2OperatorMakeFractional> try_lex(const
//     std::vector<std::unique_ptr<Token>>& token_list,
//     std::vector<std::unique_ptr<Token>>::const_iterator token_it) {
//         if (P2OperatorMakeFractional::begin_lex(token_it)) {
//             // Assert binary arguments exist.
//             if (token_it == token_list.cend() || token_it ==
//             token_list.cbegin()) {
//                 throw std::runtime_error("Operator '.' lacking lhs or rhs
//                 arguments.");
//             }
//
//             auto lhs = (token_it - 1);
//             auto rhs = (token_it + 1);
//
//             // Assert lhs is a integer literal or float literal.
//             if (lhs->get()->type() != eTokenType::integer_literal_ ||
//             lhs->get()->type() != eTokenType::float_literal_) {
//                 throw std::runtime_error("Invalid token:
//                 P2OperatorMakeFractional expected number_ as lhs argument.");
//             }
//
//             // Assert rhs is a number or whitespace. If whitespace check for
//             following number. if (rhs->get()->type() != eTokenType::number_
//             || rhs->get()->type() != eTokenType::witespace_) {
//
//                 throw std::runtime_error("Invalid token:
//                 P2OperatorMakeFractional expected number_ or whitespace_ as
//                 rhs argument.");
//             }
//
//             if (rhs->get()->type() == eTokenType::witespace_) {
//                     auto next_token = rhs + 1;
//                     if (next_token->get()->type() != eTokenType::number_) {
//					    throw std::runtime_error("Invalid token:
//P2OperatorMakeFractional expected number_ as lhs and rhs arguments.");
//				    }
//             }
//
//             if (rhs->get()->type() == eTokenType::witespace_) {
//                 rhs++;
//             }
//
//             // Check if the rhs number is a float literal by checking for a
//             following '.' operator. const_token_it next_token = rhs + 1; if
//             (P2OperatorMakeFractional::begin_lex(next_token)) {
//                 auto make_fractional =
//                 P2OperatorMakeFractional::try_lex(token_list, next_token); if
//                 (make_fractional.has_value()) {
//					auto float_litreral_loc =
//make_fractional.value().solve();
//                     if (float_litreral_loc.has_value()) {
//						auto float_literal =
//std::get<0>(float_litreral_loc.value()); 						auto rhs =
//std::get<1>(std::get<1>(float_litreral_loc.value()));
//					}
//                     else {
//						throw std::runtime_error("Invalid token:
//BinaryOperatorMakeFractional expected number_ or whitespace_ as rhs
//argument.");
//					}
//				}
//             }
//
//
//             if (rhs->get()->type() == eTokenType::number_) {
//                 return P2OperatorMakeFractional(token_it - 1, token_it + 1);
//             }
//             else if (rhs->get()->type() == eTokenType::witespace_) {
//                 return P2OperatorMakeFractional(token_it - 1, token_it + 1);
//             }
//             else {
//                 throw std::runtime_error("Invalid token:
//                 P2OperatorMakeFractional expected number_ as lhs and rhs
//                 arguments.");
//             }
//         }
//         else {
//             return std::nullopt;
//         }
//     }
//
// };
//
// auto extract_token(const std::string& source, std::size_t source_idx) {
//     auto extracted_token = std::make_unique<Token>();
//
//     // Whitespace
//     extracted_token = std::move(TokenWhitespace::try_lex(source,
//     source_idx)); if (extracted_token->type() != eTokenType::invalid_) {
//		return extracted_token;
//	}
//
//     // Number
//     extracted_token = std::move(TokenOperandInteger::try_lex( source,
//     source_idx)); if (extracted_token->type() != eTokenType::invalid_) {
//         return extracted_token;
//     }
//
//     // Identifier
//     extracted_token = std::move(TokenOperandIdentifier::try_lex(source,
//     source_idx)); if (extracted_token->type() != eTokenType::invalid_) {
//		return extracted_token;
//	}
//
//     // Symbol
//     extracted_token = std::move(TokenOperator::try_lex(source, source_idx));
//     if (extracted_token->type() != eTokenType::invalid_) {
//         return extracted_token;
//     }
//
//     // Invalid
//     extracted_token->literal_ = source[source_idx]; // Set the literal to the
//     invalid character. return extracted_token;
// }
//
// auto tokenize(std::string source) {
//     if (source.empty())
//         throw std::runtime_error("Empty source.");
//
//     auto cursor_idx = 0;
//     std::vector<std::unique_ptr<Token>> tokens;
//
//     while (cursor_idx < source.length()) {
//         auto current_char = source[cursor_idx];
//         auto token = extract_token(source, cursor_idx);
//
//             if (token->type() != eTokenType::invalid_) {
//                 cursor_idx += token->literal().length();
//                 tokens.push_back(std::move(token)); // Move the token uptr as
//                 it is no longer needed.
//			}
//             else {
//				throw std::runtime_error("Invalid character or token
//in source file: "+token->literal());
//			}
//     }
//     return tokens;
// }
//
// auto construct_tokens(std::vector<std::unique_ptr<Token>>& tokens) {
//     auto output_tokens = std::vector<std::unique_ptr<Token>>();
//     auto token_it = tokens.cbegin();
//
//     while (token_it != tokens.cend()) {
//         auto token = token_it->get();
//         eTokenType p1_type = token->type();
//
//         if (p1_type == eTokenType::number_) { // If the token is a number
//         then we check if the next token is a period incase of a
//         MakeFractional operator.
//             auto next_token = (token_it + 1)->get();
//             auto next_token_it = token_it + 1;
//
//             if (P2OperatorMakeFractional::begin_lex(next_token_it)) {
//                 auto make_fractional =
//                 P2OperatorMakeFractional::try_lex(tokens, next_token_it); if
//                 (make_fractional.has_value()) {
//                     auto float_litreral_loc =
//                     make_fractional.value().solve(); if
//                     (float_litreral_loc.has_value()) {
//						auto float_literal =
//std::get<0>(float_litreral_loc.value()); 						auto rhs =
//std::get<1>(std::get<1>(float_litreral_loc.value()));
//						output_tokens.push_back(std::make_unique<Token>(float_literal));
//						token_it = rhs + 1;
//					}
//                     else {
//                         throw std::runtime_error("Invalid token:
//                         BinaryOperatorMakeFractional expected number_ or
//                         whitespace_ as rhs argument.");
//                     }
//                 }
//             }
//             else if (next_token->type() == eTokenType::witespace_ ||
//                 next_token->type() == eTokenType::operator_) { // If the next
//                 token is a whitespace or operator then we construct an
//                 integer literal. auto integer_literal =
//                 P2OperandIntegerLiteral(*token);
//                 output_tokens.push_back(std::make_unique<Token>(integer_literal));
//
//                 token_it += 1; // We increment the token iterator by 1 as we
//                 have consumed 1 token.
//             }
//             else {
//				throw std::runtime_error("Invalid token:
//P2OperandIntegerLiteral expected whitespace_ or operator_ as next token.");
//			}
//         }
//         else if (p1_type == eTokenType::operator_) {
//
//             // Check for strigify operator
//             if (P2OperatorStringify::begin_lex(token_it)) {
//                 auto string_literal = P2OperatorStringify::try_lex(tokens,
//                 token_it); if (string_literal.has_value()) {
//                     auto string_literal_loc = string_literal.value().solve();
//                     if (string_literal_loc.has_value()) {
//                         auto string_literal =
//                         std::get<0>(string_literal_loc.value()); auto rhs =
//                         std::get<1>(std::get<1>(string_literal_loc.value()));
//                         output_tokens.push_back(std::make_unique<Token>(string_literal));
//                         token_it = rhs + 1;
//                     }
//                     else {
//                         throw std::runtime_error("Invalid token:
//                         P2OperatorStringify expected variadic arguments.");
//                     }
//                 }
//                 else {
//                     throw std::runtime_error("Invalid token:
//                     P2OperatorStringify expected variadic arguments.");
//                 }
//             }
//             else {
//                 std::cout << "WARNING: Discarding unrecognized operator token
//                 in token stream: " << token->literal() << std::endl;
//                 token_it++; // TEMPORARY: unrecognized tokens are ignored.
//                 continue;
//             }
//         }
//         else if (p1_type == eTokenType::identifier_) {
//             // for now we just push the identifier to the output stream.
//             output_tokens.push_back(std::make_unique<Token>(P2OperandObjectIdentifier(token->literal())));
//             token_it++;
//         }
//         else {
//             //throw std::runtime_error("Invalid token:
//             P2OperandIntegerLiteral expected number_ as first token.");
//             std::cout << "WARNING: Discarding unrecognized token in token
//             stream: " << token->literal() << std::endl; token_it++; //
//             TEMPORARY: unrecognized tokens are ignored. continue;
//         }
//     }
//	return output_tokens;
// }
//
// struct Statement {
//     virtual ~Statement() {};
// };
// struct Program : public Statement {
//	std::vector<std::unique_ptr<Statement>> body_;
// };
// struct Expression : public Statement {
//     virtual ~Expression() {};
//     virtual std::any solve() { return nullptr; }
// };
//
// struct Literal : Expression {
//	std::any value_;
//     std::any solve() override { return value_; }
// };
// struct IntegralLiteral : Literal {
//	std::int64_t value_;
//     std::any solve() override { return value_; }
// };
// struct FractionalLiteral : Literal {
//	std::float_t value_;
//	std::any solve() override { return value_; }
// };
//
// struct Symbol : Expression {
//	std::string symbol_;
//
//     virtual std::string symbol() const { return symbol_; }
//     virtual ~Symbol() {}
// };
// struct IdentifierSymbol : Symbol {
// };
// struct OperatorSymbol : Symbol {
// };
//
// struct BinaryExpression : Expression {
//     std::unique_ptr<Expression> lhs_;
//     std::unique_ptr<Expression> rhs_;
//     std::unique_ptr<OperatorSymbol> operator_;
// };
// struct BinaryArithmeticExpression : BinaryExpression {
//     std::any solve() {
//         auto lhs = lhs_->solve();
//         auto rhs = rhs_->solve();
//         auto op = operator_->symbol();
//
//         if (lhs.type() == typeid(std::int64_t) && rhs.type() ==
//         typeid(std::int64_t)) {
//             auto lhs_int = std::any_cast<std::int64_t>(lhs);
//             auto rhs_int = std::any_cast<std::int64_t>(rhs);
//             auto op_str = op;
//
//             if (op_str == "+") {
//                 return lhs_int + rhs_int;
//             }
//             else if (op_str == "-") {
//                 return lhs_int - rhs_int;
//             }
//             else if (op_str == "*") {
//                 return lhs_int * rhs_int;
//             }
//             else if (op_str == "/") {
//                 return lhs_int / rhs_int;
//             }
//             else if (op_str == "%") {
//                 return lhs_int % rhs_int;
//             }
//         }
//     }
// };
//
// struct VariadicExpression : Expression {
//	std::vector<std::unique_ptr<Expression>> arguments_;
//	std::unique_ptr<OperatorSymbol> operator_;
// };
//
////std::unique_ptr<Statement> parse_statement(std::vector
///<std::unique_ptr<Token>>::const_iterator token_it) { /	return
///parse_expression(token_it);
////}
////
////std::unique_ptr<Expression> parse_expression(std::vector
///<std::unique_ptr<Token>>::const_iterator token_it) { /	return
///parse_primary_expression(token_it);
////}
////
////std::unique_ptr<Expression> parse_additive_expression(std::vector
///<std::unique_ptr<Token>>::const_iterator token_it) { /    auto lhs =
///parse_primary_expression(token_it); /    auto op =
///std::make_unique<OperatorSymbol>(std::dynamic_pointer_cast<OperatorSymbol>(std::make_shared<Expression>(parse_primary_expression(token_it+1).get())));
////    auto rhs = parse_primary_expression(token_it+2);
////
////    BinaryArithmeticExpression binary_arithmetic_expression;
////    binary_arithmetic_expression.lhs_ = std::move(lhs);
////    binary_arithmetic_expression.rhs_ = std::move(rhs);
////    binary_arithmetic_expression.operator_ = std::move(op);
////
////
////    return
///std::make_unique<BinaryArithmeticExpression>(binary_arithmetic_expression);
////}
////
////std::unique_ptr<Expression> parse_primary_expression(std::vector
///<std::unique_ptr<Token>>::const_iterator token_it) { /    Token* token =
///token_it->get();
////
////    if (token->type() == eTokenType::number_) {
////		return std::make_unique<IntegralLiteral>(IntegralLiteral());
////	}
////    else if (token->type() == eTokenType::identifier_) {
////		return std::make_unique<IdentifierSymbol>(IdentifierSymbol());
////	}
////    else {
////		throw std::runtime_error("Invalid token type for
///parse_primary_expression."); /	}
////
////
////
////	return std::make_unique<Expression>(Expression());
////}
////
////Program make_ast(const std::vector <std::unique_ptr<Token>> & tokens) {
////    Program program;
////
////    for (auto token_it = tokens.cbegin(); token_it < tokens.cend();
///token_it++) { /        program.body_.push_back(parse_statement(token_it)); /
///}
////
////    return program;
////}
//// 3.3 Check for additive arithmetic operators.
// else if (token_it->get()->literal() == "+") {
//     // 3.3.1 Check if the preceding token is a NumericLiteral or
//     FloatingLiteral to resolve the lhs argument. auto last_token =
//     p2_token_end_it - 1; std::any lhs; std::any rhs; auto beg = token_it;
//     auto end = beg + 1;
//     // 3.3.1.0 Read whilespaces and newlines in reverse until a
//     non-whitespace or newline token is found. while
//     (last_token->get()->type() == eTokenType::whitespace_ ||
//     last_token->get()->type() == eTokenType::newline_) {
//         beg -= last_token->get()->size();
//         last_token--;
//     }
//     // 3.3.1.1 If the token is a NumericLiteral or FloatingLiteral then
//     resolve the lhs argument. if (last_token->get()->type() ==
//     eTokenType::integer_literal_ || last_token->get()->type() ==
//     eTokenType::float_literal_) {
//         lhs = last_token->get()->value();
//     }
//     else {
//         // Else the token is unresolvable at this phase, return the input
//         symbol token. return
//         OptionalToken(make_token_uptr(Tk1::Symbol(token_it->get()->literal())));
//     }

//    // 3.3.2 Check if the next token is a NumericLiteral or FloatingLiteral to
//    resolve the rhs argument. auto next_token = token_it + 1;
//    // 3.3.2.0 Continue reading tokens until the next token is not a
//    whitespace or newline. Stop if token is a number. while
//    (next_token->get()->type() == eTokenType::whitespace_ ||
//    next_token->get()->type() == eTokenType::newline_) {
//        next_token++;
//    }
//    // 3.3.2 Check if the next token is a number.
//    if (next_token->get()->type() == eTokenType::number_) {
//        // 3.3.2.0 Recursively call construct_token to resolve the rhs
//        argument. auto optional_token = construct_token(next_token,
//        p2_token_end_it);
//        // 3.3.2.0. If the constructed token is a floating literal or numeric
//        literal , assign the rhs argument. if (optional_token.valid() &&
//        optional_token.type() == eTokenType::float_literal_ ||
//        optional_token.type() == eTokenType::integer_literal_) {
//            rhs = optional_token.value();
//            end++;
//        }
//        else { // Else the token is unresolvable at this phase, return the
//        input symbol token.
//            return
//            OptionalToken(make_token_uptr(Tk1::Symbol(token_it->get()->literal())));
//        }
//    }
//    else { // Else the token is unresolvable at this phase, return the input
//    symbol token.
//        return
//        OptionalToken(make_token_uptr(Tk1::Symbol(token_it->get()->literal())));
//    }

//    // 3.3.3 Check if the lhs and rhs arguments are both numeric literals or
//    both floating literals. If so, resolve the token. if (lhs.type() ==
//    typeid(unsigned long long) && rhs.type() == typeid(unsigned long long)) {
//        // perform integer addition, return the result as a NumericLiteral.
//        return OptionalToken(make_token_uptr(IntegralLiteral(
//            std::to_string(std::any_cast<unsigned long long>(lhs) +
//            std::any_cast<unsigned long long>(rhs)), end - beg, end)));
//    }
//    else if (lhs.type() == typeid(float) && rhs.type() == typeid(float)) {
//        // perform floating point addition, return the result as a
//        FloatingLiteral. return
//        OptionalToken(make_token_uptr(FloatingLiteral(std::to_string(std::any_cast<float>(lhs)
//        + std::any_cast<float>(rhs)),
//            end - beg,
//            end)));
//    }
//    else { // Else the token is unresolvable at this phase, return the input
//    symbol token.
//        return
//        OptionalToken(make_token_uptr(Tk1::Symbol(token_it->get()->literal())));
//    }
//}
