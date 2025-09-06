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
#ifndef HEADER_GUARD_CAOCO_COMMON_COMPILER_ERROR_H
#define HEADER_GUARD_CAOCO_COMMON_COMPILER_ERROR_H
// Includes:
#include "cand_syntax.h"
#include "import_stl.h"

namespace compiler_error {
namespace tokenizer {

static constexpr lambda xInvalidChar = [](std::size_t line, std::size_t col,
                                          char c, std::string error = "") {
  std::stringstream ss;
  ss << "\n[User Syntax Error]: ";
  ss << "\nInvalid character at line: " << line << " column: " << col
     << " \nerror detail: " << error;
  ss << "\n Offending character: " << static_cast<char>(c);
  return ss.str();
};

static constexpr lambda xLexerSyntaxError =
    [](std::size_t line, std::size_t col, char8_t c, std::string error = "") {
      std::stringstream ss;
      ss << "\n[User Syntax Error]: ";
      ss << "\nLexer syntax error at line: " << line << " column: " << col
         << "\n Offending character: " << static_cast<char>(c)
         << "\nerror detail: " << error;
      return ss.str();
    };

static constexpr lambda xProgrammerLogicError =
    [](std::size_t line, std::size_t col, char8_t c, std::string error = "") {
      std::stringstream ss;
      ss << "\n[Compiler programmer logic error]: ";
      ss << "\nFailed to tokenize at line: " << line << " column: " << col
         << "\n Offending character: " << static_cast<char>(c)
         << " \nerror detail: " << error;
      return ss.str();
    };
}  // namespace tokenizer
namespace parser {
static constexpr lambda xPrettyPrintToken = [](const Tk& token) {
  std::stringstream ss;
  ss << "\nToken: " << ToStr(token.Type()) << "\nline: " << token.Line()
     << "\ncolumn: " << token.Col() << "\nliteral: " << token.Literal();
  return ss.str();
};

static lambda xProgrammerLogicError =
    [](eAst attempted_astnode_type, TkVectorConstIter error_location,
       std::string error_message = "",
       std::source_location err_loc = std::source_location::current()) {
      std::stringstream ss;
      ss << "\n[Compiler programmer logic error]: ";
      ss << "\nFailed to parse ast of type " << ToStr(attempted_astnode_type)
         << " at: " << xPrettyPrintToken(*error_location);
      if (!error_message.empty()) {
        ss << "\n[Detail]: " << error_message;
      }
      ss << "\n[Compiler Error Location]: " << err_loc.file_name()
         << " line: " << err_loc.line() << " column: " << err_loc.column();
      ss << "\n[Compiler Method]: " << err_loc.function_name();
      return ss.str();
    };

static constexpr lambda xOperationMissingOperand =
    [](eAst attempted_operator_type, TkVectorConstIter error_location,
       std::string error_message = "") {
      std::stringstream ss;
      ss << "\n[User Syntax Error]: ";
      ss << "\nOperation" << ToStr(attempted_operator_type)
           << " missing operand at : "
         << " at: " << xPrettyPrintToken(*error_location)
         << "\nerror detail: " << error_message;
      return ss.str();
    };

static lambda xMismatchedParentheses =
    [](TkVectorConstIter error_location, std::string error_message = "",
       std::source_location err_loc = std::source_location::current()) {
      std::stringstream ss;
      ss << "\n[C&][ERROR][xMismatchedParentheses]: ";
      ss << "\nMismatched parenthesis at: "
         << " at: " << xPrettyPrintToken(*error_location);
      if (!error_message.empty()) {
        ss << "\n[Detail]: " << error_message;
      }
      ss << "\n[Compiler Error Location]: " << err_loc.file_name()
         << " line: " << err_loc.line() << " column: " << err_loc.column();
      ss << "\n[Compiler Method]: " << err_loc.function_name();
      return ss.str();
    };

static lambda xInvalidForLoopConditionSyntax =
    [](TkVectorConstIter error_location, std::string error_message = "",
       std::source_location err_loc = std::source_location::current()) {
      std::stringstream ss;
      ss << "\n[C&][ERROR][xInvalidForLoopConditionSyntax]: ";
      ss << "\nInvalid for loop syntax at: "
         << " at: " << xPrettyPrintToken(*error_location);
      if (!error_message.empty()) {
        ss << "\n[Detail]: " << error_message;
      }
      ss << "\n[Compiler Error Location]: " << err_loc.file_name()
         << " line: " << err_loc.line() << " column: " << err_loc.column();
      ss << "\n[Compiler Method]: " << err_loc.function_name();
      return ss.str();
    };

static constexpr lambda xInvalidExpression =
    [](TkVectorConstIter error_location, std::string error_message = "") {
      std::stringstream ss;
      ss << "\n[User Syntax Error]: ";
      ss << "\nInvalid expression at: "
         << " at: " << xPrettyPrintToken(*error_location)
         << "\nerror detail: " << error_message;
      return ss.str();
    };

static lambda xExpectedToken =
    [](const std::string& expected, const std::string& found,
       const std::string& error_message = "",
       std::source_location err_loc = std::source_location::current()) {
      std::stringstream ss;
      ss << "\n[C&][ERROR][xExpectedToken] ";
      ss << "Expected: " << expected << " found: " << found;
      if (!error_message.empty()) {
        ss << "\n[Detail]: " << error_message;
      }
      ss << "\n[Compiler Error Location]: " << err_loc.file_name()
         << " line: " << err_loc.line() << " column: " << err_loc.column();
      ss << "\n[Compiler Method]: " << err_loc.function_name();
      return ss.str();
    };

static constexpr lambda xUserSyntaxError = [](TkVectorConstIter error_location,
                                              std::string error_message = "") {
  std::stringstream ss;
  ss << "\n[C&][ERROR][User Syntax Error]: "
     << " at: " << xPrettyPrintToken(*error_location)
     << "\nerror detail: " << error_message;
  return ss.str();
};
}  // namespace parser
}  // namespace compiler_error

//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

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
#endif HEADER_GUARD_CAOCO_COMMON_COMPILER_ERROR_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
