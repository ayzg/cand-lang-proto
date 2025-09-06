//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: common
// File: cand_syntax.h
//---------------------------------------------------------------------------//
// Brief: {description}
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_COMMON_CAND_SYNTAX_H
#define HEADER_GUARD_CAOCO_COMMON_CAND_SYNTAX_H
// Includes:
#include "import_stl.h"
#include "string_constant.h"
#include "compiler_enum.h"
#include "cand_grammar.h"
#include "tk_traits.h"
#include "token.h"
#include "ast.h"
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

using TkVector = std::vector<Tk>;
using TkVectorIter = TkVector::iterator;
using TkVectorConstIter = TkVector::const_iterator;

constexpr std::string ToStr(eAssoc a) {
  switch (a) {
    case eAssoc::kNone:
      return "none";
    case eAssoc::kLeft:
      return "left";
    case eAssoc::kRight:
      return "right";
    default:
      return "eAssoc invalid enum value";
  }
}

constexpr std::string ToStr(eOperation o) {
  switch (o) {
    case eOperation::kNone:
      return "none";
    case eOperation::kBinary:
      return "binary";
    case eOperation::kPrefix:
      return "prefix";
    case eOperation::kPostfix:
      return "postfix";
    default:
      return "eOperation invalid enum value";
  }
}

constexpr std::string ToStr(ePriority p) {
  switch (p) {
    case ePriority::kNone:
      return "none";
    case ePriority::kAssignment:
      return "assignment";
    case ePriority::kLogicalOr:
      return "logical_or";
    case ePriority::kLogicalAnd:
      return "logical_and";
    case ePriority::kBitwiseOr:
      return "bitwise_or";
    case ePriority::kBitwiseXor:
      return "bitwise_xor";
    case ePriority::kBitwiseAnd:
      return "bitwise_and";
    case ePriority::kEquality:
      return "equality";
    case ePriority::kThreeWayEquality:
      return "three_way_equality";
    case ePriority::kComparison:
      return "comparison";
    case ePriority::kBitshift:
      return "bitshift";
    case ePriority::kTerm:
      return "term";
    case ePriority::kFactor:
      return "factor";
    case ePriority::kPrefix:
      return "prefix";
    case ePriority::kPostfix:
      return "postfix";
    case ePriority::kFunctional:
      return "functional";
    case ePriority::kAccess:
      return "access";
    case ePriority::kMax:
      return "max";
    default:
      return "ePriority::Enum invalid enum value";
  }
}

constexpr std::string ToStr(eTk t) {
  switch (t) {
    case eTk::kNone:
      return "none";
    case eTk::kInvalid:
      return "invalid";
    case eTk::kEof:
      return "eof";
    case eTk::kLineComment:
      return "line_comment";
    case eTk::kBlockComment:
      return "block_comment";
    case eTk::kNewline:
      return "newline";
    case eTk::kWhitespace:
      return "whitespace";
    case eTk::kStringLiteral:
      return "string_literal";
    case eTk::kNumberLiteral:
      return "number_literal";
    case eTk::kDoubleLiteral:
      return "real_literal";
    case eTk::kByteLiteral:
      return "byte_literal";
    case eTk::kBoolLiteral:
      return "bit_literal";
    case eTk::kUnsignedLiteral:
      return "unsigned_literal";
    case eTk::kIdentifier:
      return "alnumus";
    case eTk::kSimpleAssignment:
      return "simple_assignment";
    case eTk::kAdditionAssignment:
      return "addition_assignment";
    case eTk::kSubtractionAssignment:
      return "subtraction_assignment";
    case eTk::kMultiplicationAssignment:
      return "multiplication_assignment";
    case eTk::kDivisionAssignment:
      return "division_assignment";
    case eTk::kRemainderAssignment:
      return "remainder_assignment";
    case eTk::kBitwiseAndAssignment:
      return "bitwise_and_assignment";
    case eTk::kBitwiseOrAssignment:
      return "bitwise_or_assignment";
    case eTk::kBitwiseXorAssignment:
      return "bitwise_xor_assignment";
    case eTk::kLeftShiftAssignment:
      return "left_shift_assignment";
    case eTk::kRightShiftAssignment:
      return "right_shift_assignment";
    case eTk::kIncrement:
      return "increment";
    case eTk::kDecrement:
      return "decrement";
    case eTk::kAddition:
      return "addition";
    case eTk::kSubtraction:
      return "subtraction";
    case eTk::kMultiplication:
      return "multiplication";
    case eTk::kDivision:
      return "division";
    case eTk::kRemainder:
      return "remainder";
    case eTk::kBitwiseNot:
      return "bitwise_not";
    case eTk::kBitwiseAnd:
      return "bitwise_and";
    case eTk::kBitwiseOr:
      return "bitwise_or";
    case eTk::kBitwiseXor:
      return "bitwise_xor";
    case eTk::kBitwiseLeftShift:
      return "bitwise_left_shift";
    case eTk::kBitwiseRightShift:
      return "bitwise_right_shift";
    case eTk::kNegation:
      return "negation";
    case eTk::kLogicalAnd:
      return "logical_and";
    case eTk::kLogicalOr:
      return "logical_or";
    case eTk::kEqual:
      return "equal";
    case eTk::kNotEqual:
      return "not_equal";
    case eTk::kLessThan:
      return "less_than";
    case eTk::kGreaterThan:
      return "greater_than";
    case eTk::kLessThanOrEqual:
      return "less_than_or_equal";
    case eTk::kGreaterThanOrEqual:
      return "greater_than_or_equal";
    case eTk::kThreeWayComparison:
      return "three_way_comparison";
    case eTk::kOpenParen:
      return "(";
    case eTk::kCloseParen:
      return ")";
    case eTk::kOpenBrace:
      return "open_brace";
    case eTk::kCloseBrace:
      return "close_brace";
    case eTk::kOpenBracket:
      return "open_bracket";
    case eTk::kCloseBracket:
      return "close_bracket";
    case eTk::kSemicolon:
      return "semicolon";
    case eTk::kColon:
      return "colon";
    case eTk::kComma:
      return "comma";
    case eTk::kPeriod:
      return "period";
    case eTk::kEllipsis:
      return "ellipsis";
    case eTk::kCommercialAt:
      return "commerical_at";
    case eTk::kNoneLiteral:
      return "none_literal";
    case eTk::kTrueLiteral:
      return "true_literal";
    case eTk::kFalseLiteral:
      return "false_literal";
    case eTk::kInt:
      return "int";
    case eTk::kUint:
      return "uint";
    case eTk::kReal:
      return "real";
    case eTk::kByte:
      return "byte";
    case eTk::kBit:
      return "bit";
    case eTk::kStr:
      return "str";
    case eTk::kArray:
      return "array";
    case eTk::kDirInclude:
      return "include";
    case eTk::kDirMacro:
      return "macro";
    case eTk::kDirEndmacro:
      return "endmacro";
    case eTk::kUse:
      return "use";
    case eTk::kClass:
      return "class";
    case eTk::kPrivate:
      return "private";
    case eTk::kPublic:
      return "public";
    case eTk::kConst:
      return "const";
    case eTk::kStatic:
      return "";
    case eTk::kRef:
      return "ref";
    case eTk::kIf:
      return "if";
    case eTk::kElse:
      return "else";
    case eTk::kElif:
      return "elif";
    case eTk::kWhile:
      return "while";
    case eTk::kFor:
      return "for";
    case eTk::kSwitch:
      return "switch";
    case eTk::kCase:
      return "case";
    case eTk::kDefault:
      return "default";
    case eTk::kBreak:
      return "break";
    case eTk::kContinue:
      return "continue";
    case eTk::kReturn:
      return "return";
    case eTk::kAuto:
      return "auto";
    case eTk::kAny:
      return "any";
    case eTk::kDef:
      return "def";
    case eTk::kFn:
      return "fn";
    case eTk::kLib:
      return "lib";
    case eTk::kMain:
      return "main";
    case eTk::kImport:
      return "import";
    case eTk::kList:
      return "list";
    case eTk::kNamespace:
      return "namespace";
    case eTk::kDoubleColon:
      return "double_colon";
    default:
      return "[!eTk invalid enum value: Implement in ToStr(eTk) in "
             "cand_syntax.h]";
  }
}

constexpr std::string ToStr(eAst t) {
  switch (t) {
    case eAst::kNone:
      return "none";
    case eAst::kInvalid:
      return "invalid";
    case eAst::kEof:
      return "eof";
    case eAst::kLineComment:
      return "line_comment";
    case eAst::kBlockComment:
      return "block_comment";
    case eAst::kNewline:
      return "newline";
    case eAst::kWhitespace:
      return "whitespace";
    case eAst::kStringLiteral:
      return "string_literal";
    case eAst::kNumberLiteral:
      return "number_literal";
    case eAst::kDoubleLiteral:
      return "real_literal";
    case eAst::kByteLiteral:
      return "byte_literal";
    case eAst::kBoolLiteral:
      return "bit_literal";
    case eAst::kUnsignedLiteral:
      return "unsigned_literal";
    case eAst::kIdentifier:
      return "alnumus";
    case eAst::kSimpleAssignment:
      return "simple_assignment";
    case eAst::kAdditionAssignment:
      return "addition_assignment";
    case eAst::kSubtractionAssignment:
      return "subtraction_assignment";
    case eAst::kMultiplicationAssignment:
      return "multiplication_assignment";
    case eAst::kDivisionAssignment:
      return "division_assignment";
    case eAst::kRemainderAssignment:
      return "remainder_assignment";
    case eAst::kBitwiseAndAssignment:
      return "bitwise_and_assignment";
    case eAst::kBitwiseOrAssignment:
      return "bitwise_or_assignment";
    case eAst::kBitwiseXorAssignment:
      return "bitwise_xor_assignment";
    case eAst::kLeftShiftAssignment:
      return "left_shift_assignment";
    case eAst::kRightShiftAssignment:
      return "right_shift_assignment";
    case eAst::kIncrement:
      return "increment";
    case eAst::kDecrement:
      return "decrement";
    case eAst::kAddition:
      return "addition";
    case eAst::kSubtraction:
      return "subtraction";
    case eAst::kMultiplication:
      return "multiplication";
    case eAst::kDivision:
      return "division";
    case eAst::kRemainder:
      return "remainder";
    case eAst::kBitwiseNot:
      return "bitwise_not";
    case eAst::kBitwiseAnd:
      return "bitwise_and";
    case eAst::kBitwiseOr:
      return "bitwise_or";
    case eAst::kBitwiseXor:
      return "bitwise_xor";
    case eAst::kBitwiseLeftShift:
      return "bitwise_left_shift";
    case eAst::kBitwiseRightShift:
      return "bitwise_right_shift";
    case eAst::kNegation:
      return "negation";
    case eAst::kLogicalAnd:
      return "logical_and";
    case eAst::kLogicalOr:
      return "logical_or";
    case eAst::kEqual:
      return "equal";
    case eAst::kNotEqual:
      return "not_equal";
    case eAst::kLessThan:
      return "less_than";
    case eAst::kGreaterThan:
      return "greater_than";
    case eAst::kLessThanOrEqual:
      return "less_than_or_equal";
    case eAst::kGreaterThanOrEqual:
      return "greater_than_or_equal";
    case eAst::kThreeWayComparison:
      return "three_way_comparison";
    case eAst::kOpenParen:
      return "open_paren";
    case eAst::kCloseParen:
      return "close_paren";
    case eAst::kOpenBrace:
      return "open_brace";
    case eAst::kCloseBrace:
      return "close_brace";
    case eAst::kOpenBracket:
      return "open_bracket";
    case eAst::kCloseBracket:
      return "close_bracket";
    case eAst::kSemicolon:
      return "semicolon";
    case eAst::kColon:
      return "colon";
    case eAst::kComma:
      return "comma";
    case eAst::kPeriod:
      return "period";
    case eAst::kEllipsis:
      return "ellipsis";
    case eAst::kCommericalAt:
      return "commerical_at";
    case eAst::kNoneLiteral:
      return "none_literal";
    case eAst::kTrueLiteral:
      return "true_literal";
    case eAst::kFalseLiteral:
      return "false_literal";
    case eAst::kType:
      return "type";
    case eAst::kIdentity:
      return "identity";
    case eAst::kValue:
      return "value";
    case eAst::kInt:
      return "int";
    case eAst::kUint:
      return "uint";
    case eAst::kReal:
      return "real";
    case eAst::kByte:
      return "byte";
    case eAst::kBit:
      return "bit";
    case eAst::kStr:
      return "str";
    case eAst::kArray:
      return "array";
    case eAst::kPointer:
      return "pointer";
    case eAst::kMemory:
      return "memory";
    case eAst::kFunction:
      return "function";
    case eAst::kInclude:
      return "include";
    case eAst::kMacro:
      return "macro";
    case eAst::kEndmacro:
      return "endmacro";
    case eAst::kEnter:
      return "enter";
    case eAst::kStart:
      return "start";
    case eAst::kUse:
      return "use";
    case eAst::kClass:
      return "class";
    case eAst::kObj:
      return "obj";
    case eAst::kDef:
      return "def";
    case eAst::kFn:
      return "fn";
    case eAst::kAuto:
      return "auto";
    case eAst::kAny:
      return "any";
    case eAst::kPrint:
      return "print";
    case eAst::kPrivate:
      return "private";
    case eAst::kPublic:
      return "public";
    case eAst::kConst:
      return "const";
    case eAst::kStatic:
      return "";
    case eAst::kRef:
      return "ref";
    case eAst::kIf:
      return "if";
    case eAst::kElse:
      return "else";
    case eAst::kElif:
      return "elif";
    case eAst::kWhile:
      return "while";
    case eAst::kFor:
      return "for";
    case eAst::kSwitch:
      return "switch";
    case eAst::kCase:
      return "case";
    case eAst::kDefault:
      return "default";
    case eAst::kBreak:
      return "break";
    case eAst::kContinue:
      return "continue";
    case eAst::kReturn:
      return "return";
    case eAst::kStatement:
      return "statement";
    case eAst::kExpression:
      return "expression";
    case eAst::kProgram:
      return "program";
    case eAst::kPragmaticBlock:
      return "pragmatic_block";
    case eAst::kFunctionalBlock:
      return "functional_block";
    case eAst::kConditionalBlock:
      return "conditional_block";
    case eAst::kIterativeBlock:
      return "iterative_block";
    case eAst::kArguments:
      return "arguments";
    case eAst::kMethodParameter:
      return "parameter";
    case eAst::kMethodParameterList:
      return "parameter_list";
    case eAst::kMethodReturnType:
      return "return_type";
    case eAst::kTypeConstraints:
      return "type_constraints";
    case eAst::kCaptureList:
      return "capture_list";
    case eAst::kSubexpression:
      return "subexpression";
    case eAst::kTypeList:
      return "type_list";
    case eAst::kGenericList:
      return "generic_list";
    case eAst::kFunctionCall:
      return "function_call";
    case eAst::kTypeCall:
      return "type_call";
    case eAst::kTypeArguments:
      return "type_arguments";
    case eAst::kIndexOperator:
      return "index_operator";
    case eAst::kIndexArguments:
      return "index_arguments";
    case eAst::kUnaryMinus:
      return "unary_minus";
    case eAst::kDereference:
      return "dereference";
    case eAst::kAddressOf:
      return "address_of";
    case eAst::kListingOperator:
      return "listing_operator";
    case eAst::kModifiers:
      return "modifiers";
    case eAst::kLib:
      return "lib";
    case eAst::kMain:
      return "main";
    case eAst::kImport:
      return "import";
    case eAst::kList:
      return "list";
    case eAst::kNamespace:
      return "namespace";

      // Declarations and Defintions
      // Generated by a 'use' declaration.
      // kTypeAlias, kLibraryTypeAlias, kLibraryNamespaceInclusion,
      // kNamespaceInclusion,
    case eAst::kTypeAlias:
      return "type_alias";
    case eAst::kLibraryTypeAlias:
      return "library_type_alias";
    case eAst::kLibraryNamespaceInclusion:
      return "library_namespace_inclusion";
    case eAst::kNamespaceInclusion:
      return "namespace_inclusion";
    case eAst::kNamespaceObjectInclusion:
      return "namespace_object_inclusion";
    case eAst::kLibraryObjectInclusion:
      return "library_object_inclusion";
      // Generated by a 'def' declaration.
      // kVariableDeclaration, kVariableDefinition,
    case eAst::kVariableDeclaration:
      return "variable_declaration";
    case eAst::kVariableDefinition:
      return "variable_definition";
      // Generated by a 'fn' declaration.
      // kMethodDeclaration, kMethodDefinition,
    case eAst::kMethodDeclaration:
      return "method_declaration";
    case eAst::kMethodDefinition:
      return "method_definition";
    case eAst::kMethodSignature:
      return "method_signature";
    case eAst::kMethodVoid:
      return "method_void";
      // Genenrated a 'class' declaration.
      // kClassDefinition, kClassDeclaration,
    case eAst::kClassDefinition:
      return "class_definition";
    case eAst::kClassDeclaration:
      return "class_declaration";
      // Generated by import declaration.
      // kImportDeclaration,
    case eAst::kImportDeclaration:
      return "import_declaration";
      // Generated by the lib declaration.
      // kLibraryDeclaration, kLibraryDefinition,
    case eAst::kLibraryDeclaration:
      return "library_declaration";
    case eAst::kLibraryDefinition:
      return "library_definition";
      // Generated by a main declaration.
      // kMainDeclaration, kMainDefinition,
    case eAst::kMainDeclaration:
      return "main_declaration";
    case eAst::kMainDefinition:
      return "main_definition";

      // Generated by if keyword.

    case eAst::kIfStatement:
      return "if_statement";
    case eAst::kDoubleColon:
      return "double_colon";
    default:
      return "[!eAst invalid enum value: Implement in ToStr(eAst) in "
             "cand_syntax.h]";
  }
}

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: common
// File: cand_syntax.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_COMMON_CAND_SYNTAX_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
