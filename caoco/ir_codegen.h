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
#ifndef HEADER_GUARD_CAOCO_COMPILER_IR_CODEGEN_H
#define HEADER_GUARD_CAOCO_COMPILER_IR_CODEGEN_H

#include "cand_lang.h"
#include "cand_syntax.h"
#include "import_stl.h"

static constexpr CandNone kCandNull = CandNone{};
static constexpr CandUndefined kCandUndefined = CandUndefined{};

// Error codes
static constexpr std::string_view kIrErrorNoProgramDefinition =
    "[C&][ERROR][CRITICAL] No program definition found.";

static constexpr std::string_view kIrErrorDeclarationCannotAppearInContext =
    "[C&][ERROR][CRITICAL] Declaration cannot appear in this context.";

static constexpr std::string_view kIrErrorInvalidNumberLiteral =
    "[C&][ERROR][CRITICAL] Invalid number literal.";

static constexpr std::string_view kIrErrorInvalidLiteralAstType =
    "[C&][ERROR][CRITICAL] Invalid literal AST type.";

static constexpr std::string_view kIrErrorInvalidStringLiteral =
    "[C&][ERROR][CRITICAL] Invalid string literal.";

static constexpr std::string_view kIrErrorInvalidPrimaryExpression =
    "[C&][ERROR][CRITICAL] Invalid primary expression.";

enum class eIrOp {
  // Program
  ENTER_PROGRAM_DEFINITION,
  ABORT_AND_ERROR,
  ALLOCATE_LITERAL,
  ALLOCATE_STACK_VALUE,

  // Variables
  DECLARE_VARIABLE,
  DEFINE_VARIABLE,

  // Methods
  DECLARE_METHOD,
  DEFINE_METHOD,

  // Object
  DECLARE_OBJECT,
  DEFINE_OBJECT,
  ADD_OBJECT_STATIC_MEMBER,
  ADD_OBJECT_STATIC_METHOD,
  ADD_OBJECT_MEMBER,
  ADD_OBJECT_METHOD,
  ADD_OBJECT_CONSTRUCTOR,
  ADD_OBJECT_DESTRUCTOR,

  // Control flow


  // Operators
  BINARY_ADD,
  BINARY_SUB,
  BINARY_MUL,
  BINARY_DIV,
  BINARY_MOD,
};

using IrInt = int;
using IrDouble = double;
using IrString = std::string_view;
using IrVariant = std::variant<int, double, std::string_view>;
static const std::vector<IrVariant> kIrOpNullArguments = {};

struct IrLine {
  std::size_t index;
  eIrOp op;
  std::vector<IrVariant> args;
};

struct IrCode {
  std::list<IrLine> lines;

  std::list<IrLine>::iterator getLine(int index) {
    return std::next(lines.begin(), index);
  }
  IrLine& AddLine(std::size_t line_index, eIrOp op,
                  std::vector<IrVariant> args) {
    lines.push_back(IrLine{line_index, op, args});
    return lines.back();
  }
  IrLine& AddLine(IrLine line) {
    lines.push_back(line);
    return lines.back();
  }
  void AddLines(std::list<IrLine> lines_vec) {
    for (const auto& line : lines_vec) {
      lines.push_back(line);
    }
  }
  std::list<IrLine>& GetLines() { return lines; }

  bool isAborted() {
    if (lines.back().op == eIrOp::ABORT_AND_ERROR) {
      return true;
    } else {
      return false;
    }
  }

  void PrintDisassembly() {
    for (const auto& line : lines) {
      std::cout << "Line " << line.index << ": ";
      switch (line.op) {
        case eIrOp::ENTER_PROGRAM_DEFINITION:
          std::cout << "ENTER_PROGRAM_DEFINITION";
          break;
        case eIrOp::ABORT_AND_ERROR:
          std::cout << "ABORT_AND_ERROR";
          break;
        case eIrOp::ALLOCATE_LITERAL:
          std::cout << "ALLOCATE_LITERAL";
          break;
        case eIrOp::ALLOCATE_STACK_VALUE:
          std::cout << "ALLOCATE_STACK_VALUE";
          break;
        case eIrOp::DECLARE_VARIABLE:
          std::cout << "DECLARE_VARIABLE";
          break;
        case eIrOp::DEFINE_VARIABLE:
          std::cout << "DEFINE_VARIABLE";
          break;
        case eIrOp::BINARY_ADD:
          std::cout << "BINARY_ADD";
          break;
        case eIrOp::BINARY_SUB:
          std::cout << "BINARY_SUB";
          break;
        case eIrOp::BINARY_MUL:
          std::cout << "BINARY_MUL";
          break;
        case eIrOp::BINARY_DIV:
          std::cout << "BINARY_DIV";
          break;
        case eIrOp::BINARY_MOD:
          std::cout << "BINARY_MOD";
          break;
      }
      std::cout << " Args: ";
      for (const auto& arg : line.args) {
        std::visit(
            [](auto&& arg) {
              using T = std::decay_t<decltype(arg)>;
              if constexpr (std::is_same_v<T, int>) {
                std::cout << arg << " ";
              } else if constexpr (std::is_same_v<T, double>) {
                std::cout << arg << " ";
              } else if constexpr (std::is_same_v<T, std::string_view>) {
                std::cout << arg << " ";
              }
            },
            arg);
      }
      std::cout << std::endl;
    }
  }
};

class IrGen {
  using IrLineList = std::list<IrLine>;
  using LineIndex = std::size_t;
  IrCode ir;
  LineIndex line_index = 0;

  static IrLine LineGenNumberLiteral(LineIndex line_index,
                                     std::string literal) {
    int value{};
    try {
      value = std::stoi(literal);
    } catch (const std::exception&) {
      return IrLine(line_index, eIrOp::ABORT_AND_ERROR,
                    {kIrErrorInvalidNumberLiteral});
    }
    return IrLine(line_index, eIrOp::ALLOCATE_LITERAL, {value});
  }

  static IrLine LineGenDoubleLiteral(LineIndex line_index,
                                     std::string literal) {
    float value{};
    try {
      value = std::stof(literal);
    } catch (const std::exception&) {
      return IrLine(line_index, eIrOp::ABORT_AND_ERROR,
                    {kIrErrorInvalidNumberLiteral});
    }
    return IrLine(line_index, eIrOp::ALLOCATE_LITERAL, {value});
  }

  static IrLine LineGenStringLiteral(LineIndex line_index,
                                     std::string literal) {
    // Process escape sequences
    std::string value = literal;

    // Remove the quotes
    value = value.substr(1, value.size() - 2);
    std::string escaped_value = "";
    for (auto c = value.begin(); c != value.end(); c++) {
      if (*c == '\\') {
        c++;
        // Process escape sequence
        if (c == value.end()) {
          // Invalid escape sequence
          return IrLine(line_index, eIrOp::ABORT_AND_ERROR,
                        {kIrErrorInvalidStringLiteral});
        }
        switch (*c) {
          case 'n':
            escaped_value += '\n';
            break;
          case 't':
            escaped_value += '\t';
            break;
          case 'r':
            escaped_value += '\r';
            break;
          case '0':
            escaped_value += '\0';
            break;
          case '\\':
            escaped_value += '\\';
            break;
          case '\'':
            escaped_value += '\'';
            break;
          case '\"':
            escaped_value += '\"';
            break;
          default:
            // Invalid escape sequence
            return IrLine(line_index, eIrOp::ABORT_AND_ERROR,
                          {kIrErrorInvalidNumberLiteral});
        }
      } else {
        // Process regular character
        escaped_value += *c;
      }
    }

    return IrLine(line_index, eIrOp::ALLOCATE_LITERAL, {escaped_value});
  }

  void GenLiteral(const Ast& ast) {
    lambda xLineGenLiteral = [&](std::string literal, auto&& visitor) -> void {
      ir.AddLine(visitor(line_index, ast.Literal()));
      if (!(ir.lines.back().op == eIrOp::ABORT_AND_ERROR)) {
        line_index++;
      }
    };

    switch (ast.Type()) {
      case eAst::kNumberLiteral:
        xLineGenLiteral(ast.Literal(), &LineGenNumberLiteral);
        break;
      case eAst::kDoubleLiteral:
        xLineGenLiteral(ast.Literal(), &LineGenDoubleLiteral);
        break;
      case eAst::kStringLiteral:
        xLineGenLiteral(ast.Literal(), &LineGenStringLiteral);
        break;
      default:
        ir.AddLine(IrLine(line_index, eIrOp::ABORT_AND_ERROR,
                          {kIrErrorInvalidLiteralAstType}));
    }
  }

  void GenPrimaryExpr(const Ast& ast) {
    // Handle literals
    if (ast.IsLiteral()) {
      GenLiteral(ast);
    } 
    // Handle binary operations
    else if (ast.IsArithmeticBinaryOp()) {
      GenBinaryExpr(ast);
    } 
    else
        ir.AddLine(line_index, eIrOp::ABORT_AND_ERROR,
                   {kIrErrorInvalidPrimaryExpression});
  }

  void GenBinaryExpr(const Ast& ast) {
    auto binop_type = ast.Type();
    std::vector<IrVariant> args;
    // Generate the binary operation
    IrLine* binop_line = nullptr;
    switch (binop_type) {
      case eAst::kAddition:
        binop_line =
            &ir.AddLine(line_index, eIrOp::BINARY_ADD, kIrOpNullArguments);
        break;
      case eAst::kSubtraction:
        binop_line =
            &ir.AddLine(line_index, eIrOp::BINARY_SUB, kIrOpNullArguments);
        break;
      case eAst::kMultiplication:
        binop_line =
            &ir.AddLine(line_index, eIrOp::BINARY_MUL, kIrOpNullArguments);
        break;
      case eAst::kDivision:
        binop_line =
            &ir.AddLine(line_index, eIrOp::BINARY_DIV, kIrOpNullArguments);
        break;
      case eAst::kRemainder:
        binop_line =
            &ir.AddLine(line_index, eIrOp::BINARY_MOD, kIrOpNullArguments);
        break;
      default:
        binop_line = &ir.AddLine(line_index, eIrOp::ABORT_AND_ERROR,
                                 {kIrErrorInvalidLiteralAstType});
    }

    // Return early if binary operation type does not exist.
    if (ir.isAborted()) {
      return;
    }

    // Advance the line index
    line_index++;

    // First child is the left operand
    const auto& left_operand_ast = ast[0];
    auto left_operand_start = line_index;
    args.push_back((int)line_index);
    GenPrimaryExpr(left_operand_ast);
    args.push_back((int)line_index-1);
    if (ir.isAborted()) {
      return;
    }

    // Second child is the right operand
    const auto& right_operand_ast = ast[1];
    args.push_back((int)line_index);
    GenPrimaryExpr(right_operand_ast);
    args.push_back((int)line_index-1);
    if (ir.isAborted()) {
      return;
    }

    // Set the lines for the binop args.
    binop_line->args = args;
  }

  void GenVariableDeclaration(const Ast& ast) {
    // First child is modifiers, ignore for now
    auto& var_decl_line = ir.AddLine(line_index, eIrOp::DECLARE_VARIABLE,
                                     kIrOpNullArguments);
    line_index++;

    // Second child is the type constraint. TEMP ignore for now
    //const auto& type_constraint_ast = ast[1];
    // For now always push 'any' type index
    var_decl_line.args.push_back((int)RtVal::Any::idx);

    // Third child is the identifier
    const auto& identifier_ast = ast[2];
    var_decl_line.args.push_back(identifier_ast.Literal());

    // Fourth child is the initializer
    // Format: [VariableDefinition] -> [Expr]
    if (ast.Size() == 4) {
      var_decl_line.args.push_back((int)line_index);
      const auto& initializer_ast = ast[3][0]; // Get first child of initializer
      // Ast has children. Must be an expression.
      GenPrimaryExpr(initializer_ast);
      var_decl_line.args.push_back((int)line_index);
    }
  }

 public:
  IrCode GenerateIr(const Ast& ast) {
    // Create the entry initial block
    ir.AddLine(line_index, eIrOp::ENTER_PROGRAM_DEFINITION, kIrOpNullArguments);
    line_index++;

    // First AST node must always be a Program
    if (ast.Empty() || ast.TypeIsnt(eAst::kProgram)) {
      ir.AddLine(line_index, eIrOp::ABORT_AND_ERROR,
                 {kIrErrorNoProgramDefinition});
      return ir;
    }

    // Read all declarations in the program from top to bottom.
    for (const auto& decl_ast : ast.Children()) {
      switch (decl_ast.Type()) {
        case eAst::kVariableDeclaration:
          GenVariableDeclaration(decl_ast);
          break;
        // Default case, invalid declaration in this context.
        default:
          ir.AddLine(line_index, eIrOp::ABORT_AND_ERROR,
                     {kIrErrorDeclarationCannotAppearInContext});
          return ir;
      }
    }

    return ir;
  }
};

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
#endif HEADER_GUARD_CAOCO_COMPILER_IR_CODEGEN_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//