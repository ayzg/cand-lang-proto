//#include "cand_syntax.h"
//
//#include "import_stl.h"
//
// constexpr std::string ToStr(eAssoc a) {
//  switch (a) {
//    case eAssoc::kNone:
//      return "none";
//    case eAssoc::kLeft:
//      return "left";
//    case eAssoc::kRight:
//      return "right";
//    default:
//      return "eAssoc invalid enum value";
//  }
//}
//
// constexpr std::string ToStr(eOperation o) {
//  switch (o) {
//    case eOperation::kNone:
//      return "none";
//    case eOperation::kBinary:
//      return "binary";
//    case eOperation::kBinaryOrPrefix:
//      return "binary_or_prefix";
//    case eOperation::kPrefix:
//      return "prefix";
//    case eOperation::kPostfix:
//      return "postfix";
//    case eOperation::kVariadic:
//      return "variadic";
//    case eOperation::kScopeEntry:
//      return "scope_entry";
//    case eOperation::kScopeExit:
//      return "scope_exit";
//    default:
//      return "eOperation invalid enum value";
//  }
//}
//
// constexpr std::string ToStr(ePriority p) {
//  switch (p) {
//    case ePriority::kNone:
//      return "none";
//    case ePriority::kAssignment:
//      return "assignment";
//    case ePriority::kLogicalOr:
//      return "logical_or";
//    case ePriority::kLogicalAnd:
//      return "logical_and";
//    case ePriority::kBitwiseOr:
//      return "bitwise_or";
//    case ePriority::kBitwiseXor:
//      return "bitwise_xor";
//    case ePriority::kBitwiseAnd:
//      return "bitwise_and";
//    case ePriority::kEquality:
//      return "equality";
//    case ePriority::kThreeWayEquality:
//      return "three_way_equality";
//    case ePriority::kComparison:
//      return "comparison";
//    case ePriority::kBitshift:
//      return "bitshift";
//    case ePriority::kTerm:
//      return "term";
//    case ePriority::kFactor:
//      return "factor";
//    case ePriority::kPrefix:
//      return "prefix";
//    case ePriority::kPostfix:
//      return "postfix";
//    case ePriority::kFunctional:
//      return "functional";
//    case ePriority::kAccess:
//      return "access";
//    case ePriority::kMax:
//      return "max";
//    default:
//      return "ePriority::Enum invalid enum value";
//  }
//}
//
// constexpr std::string ToStr(eTk t) {
//  switch (t) {
//    case eTk::kNone:
//      return "none";
//    case eTk::kInvalid:
//      return "invalid";
//    case eTk::kEof:
//      return "eof";
//    case eTk::kLineComment:
//      return "line_comment";
//    case eTk::kBlockComment:
//      return "block_comment";
//    case eTk::kNewline:
//      return "newline";
//    case eTk::kWhitespace:
//      return "whitespace";
//    case eTk::kStringLiteral:
//      return "string_literal";
//    case eTk::kNumberLiteral:
//      return "number_literal";
//    case eTk::kDoubleLiteral:
//      return "real_literal";
//    case eTk::kByteLiteral:
//      return "byte_literal";
//    case eTk::kBoolLiteral:
//      return "bit_literal";
//    case eTk::kUnsignedLiteral:
//      return "unsigned_literal";
//    case eTk::kIdentifier:
//      return "alnumus";
//    case eTk::kSimpleAssignment:
//      return "simple_assignment";
//    case eTk::kAdditionAssignment:
//      return "addition_assignment";
//    case eTk::kSubtractionAssignment:
//      return "subtraction_assignment";
//    case eTk::kMultiplicationAssignment:
//      return "multiplication_assignment";
//    case eTk::kDivisionAssignment:
//      return "division_assignment";
//    case eTk::kRemainderAssignment:
//      return "remainder_assignment";
//    case eTk::kBitwiseAndAssignment:
//      return "bitwise_and_assignment";
//    case eTk::kBitwiseOrAssignment:
//      return "bitwise_or_assignment";
//    case eTk::kBitwiseXorAssignment:
//      return "bitwise_xor_assignment";
//    case eTk::kLeftShiftAssignment:
//      return "left_shift_assignment";
//    case eTk::kRightShiftAssignment:
//      return "right_shift_assignment";
//    case eTk::kIncrement:
//      return "increment";
//    case eTk::kDecrement:
//      return "decrement";
//    case eTk::kAddition:
//      return "addition";
//    case eTk::kSubtraction:
//      return "subtraction";
//    case eTk::kMultiplication:
//      return "multiplication";
//    case eTk::kDivision:
//      return "division";
//    case eTk::kRemainder:
//      return "remainder";
//    case eTk::kBitwiseNot:
//      return "bitwise_not";
//    case eTk::kBitwiseAnd:
//      return "bitwise_and";
//    case eTk::kBitwiseOr:
//      return "bitwise_or";
//    case eTk::kBitwiseXor:
//      return "bitwise_xor";
//    case eTk::kBitwiseLeftShift:
//      return "bitwise_left_shift";
//    case eTk::kBitwiseRightShift:
//      return "bitwise_right_shift";
//    case eTk::kNegation:
//      return "negation";
//    case eTk::kLogicalAnd:
//      return "logical_and";
//    case eTk::kLogicalOr:
//      return "logical_or";
//    case eTk::kEqual:
//      return "equal";
//    case eTk::kNotEqual:
//      return "not_equal";
//    case eTk::kLessThan:
//      return "less_than";
//    case eTk::kGreaterThan:
//      return "greater_than";
//    case eTk::kLessThanOrEqual:
//      return "less_than_or_equal";
//    case eTk::kGreaterThanOrEqual:
//      return "greater_than_or_equal";
//    case eTk::kThreeWayComparison:
//      return "three_way_comparison";
//    case eTk::kOpenParen:
//      return "(";
//    case eTk::kCloseParen:
//      return ")";
//    case eTk::kOpenBrace:
//      return "open_brace";
//    case eTk::kCloseBrace:
//      return "close_brace";
//    case eTk::kOpenBracket:
//      return "open_bracket";
//    case eTk::kCloseBracket:
//      return "close_bracket";
//    case eTk::kSemicolon:
//      return "semicolon";
//    case eTk::kColon:
//      return "colon";
//    case eTk::kComma:
//      return "comma";
//    case eTk::kPeriod:
//      return "period";
//    case eTk::kEllipsis:
//      return "ellipsis";
//    case eTk::kCommercialAt:
//      return "commerical_at";
//    case eTk::kNoneLiteral:
//      return "none_literal";
//    case eTk::kTrueLiteral:
//      return "true_literal";
//    case eTk::kFalseLiteral:
//      return "false_literal";
//    case eTk::kType:
//      return "type";
//    case eTk::kIdentity:
//      return "identity";
//    case eTk::kValue:
//      return "value";
//    case eTk::kInt:
//      return "int";
//    case eTk::kUint:
//      return "uint";
//    case eTk::kReal:
//      return "real";
//    case eTk::kByte:
//      return "byte";
//    case eTk::kBit:
//      return "bit";
//    case eTk::kStr:
//      return "str";
//    case eTk::kArray:
//      return "array";
//    case eTk::kPointer:
//      return "pointer";
//    case eTk::kMemory:
//      return "memory";
//    case eTk::kFunction:
//      return "function";
//    case eTk::kInclude:
//      return "include";
//    case eTk::kMacro:
//      return "macro";
//    case eTk::kEndmacro:
//      return "endmacro";
//    case eTk::kEnter:
//      return "enter";
//    case eTk::kStart:
//      return "start";
//    case eTk::kUse:
//      return "use";
//    case eTk::kClass:
//      return "class";
//    case eTk::kObj:
//      return "obj";
//    case eTk::kPrint:
//      return "print";
//    case eTk::kPrivate:
//      return "private";
//    case eTk::kPublic:
//      return "public";
//    case eTk::kConst:
//      return "const";
//    case eTk::kStatic:
//      return "";
//    case eTk::kRef:
//      return "ref";
//    case eTk::kIf:
//      return "if";
//    case eTk::kElse:
//      return "else";
//    case eTk::kElif:
//      return "elif";
//    case eTk::kWhile:
//      return "while";
//    case eTk::kFor:
//      return "for";
//    case eTk::kSwitch:
//      return "switch";
//    case eTk::kCase:
//      return "case";
//    case eTk::kDefault:
//      return "default";
//    case eTk::kBreak:
//      return "break";
//    case eTk::kContinue:
//      return "continue";
//    case eTk::kReturn:
//      return "return";
//    default:
//      return "eTk invalid enum value";
//  }
//}
//
// constexpr std::string ToStr(eAst t) {
//  switch (t) {
//    case eAst::kNone:
//      return "none";
//    case eAst::kInvalid:
//      return "invalid";
//    case eAst::kEof:
//      return "eof";
//    case eAst::kLineComment:
//      return "line_comment";
//    case eAst::kBlockComment:
//      return "block_comment";
//    case eAst::kNewline:
//      return "newline";
//    case eAst::kWhitespace:
//      return "whitespace";
//    case eAst::kStringLiteral:
//      return "string_literal";
//    case eAst::kNumberLiteral:
//      return "number_literal";
//    case eAst::kDoubleLiteral:
//      return "real_literal";
//    case eAst::kByteLiteral:
//      return "byte_literal";
//    case eAst::kBoolLiteral:
//      return "bit_literal";
//    case eAst::kUnsignedLiteral:
//      return "unsigned_literal";
//    case eAst::kIdentifier:
//      return "alnumus";
//    case eAst::kSimpleAssignment:
//      return "simple_assignment";
//    case eAst::kAdditionAssignment:
//      return "addition_assignment";
//    case eAst::kSubtractionAssignment:
//      return "subtraction_assignment";
//    case eAst::kMultiplicationAssignment:
//      return "multiplication_assignment";
//    case eAst::kDivisionAssignment:
//      return "division_assignment";
//    case eAst::kRemainderAssignment:
//      return "remainder_assignment";
//    case eAst::kBitwiseAndAssignment:
//      return "bitwise_and_assignment";
//    case eAst::kBitwiseOrAssignment:
//      return "bitwise_or_assignment";
//    case eAst::kBitwiseXorAssignment:
//      return "bitwise_xor_assignment";
//    case eAst::kLeftShiftAssignment:
//      return "left_shift_assignment";
//    case eAst::kRightShiftAssignment:
//      return "right_shift_assignment";
//    case eAst::kIncrement:
//      return "increment";
//    case eAst::kDecrement:
//      return "decrement";
//    case eAst::kAddition:
//      return "addition";
//    case eAst::kSubtraction:
//      return "subtraction";
//    case eAst::kMultiplication:
//      return "multiplication";
//    case eAst::kDivision:
//      return "division";
//    case eAst::kRemainder:
//      return "remainder";
//    case eAst::kBitwiseNot:
//      return "bitwise_not";
//    case eAst::kBitwiseAnd:
//      return "bitwise_and";
//    case eAst::kBitwiseOr:
//      return "bitwise_or";
//    case eAst::kBitwiseXor:
//      return "bitwise_xor";
//    case eAst::kBitwiseLeftShift:
//      return "bitwise_left_shift";
//    case eAst::kBitwiseRightShift:
//      return "bitwise_right_shift";
//    case eAst::kNegation:
//      return "negation";
//    case eAst::kLogicalAnd:
//      return "logical_and";
//    case eAst::kLogicalOr:
//      return "logical_or";
//    case eAst::kEqual:
//      return "equal";
//    case eAst::kNotEqual:
//      return "not_equal";
//    case eAst::kLessThan:
//      return "less_than";
//    case eAst::kGreaterThan:
//      return "greater_than";
//    case eAst::kLessThanOrEqual:
//      return "less_than_or_equal";
//    case eAst::kGreaterThanOrEqual:
//      return "greater_than_or_equal";
//    case eAst::kThreeWayComparison:
//      return "three_way_comparison";
//    case eAst::kOpenParen:
//      return "open_paren";
//    case eAst::kCloseParen:
//      return "close_paren";
//    case eAst::kOpenBrace:
//      return "open_brace";
//    case eAst::kCloseBrace:
//      return "close_brace";
//    case eAst::kOpenBracket:
//      return "open_bracket";
//    case eAst::kCloseBracket:
//      return "close_bracket";
//    case eAst::kSemicolon:
//      return "semicolon";
//    case eAst::kColon:
//      return "colon";
//    case eAst::kComma:
//      return "comma";
//    case eAst::kPeriod:
//      return "period";
//    case eAst::kEllipsis:
//      return "ellipsis";
//    case eAst::kCommericalAt:
//      return "commerical_at";
//    case eAst::kNoneLiteral:
//      return "none_literal";
//    case eAst::kTrueLiteral:
//      return "true_literal";
//    case eAst::kFalseLiteral:
//      return "false_literal";
//    case eAst::kType:
//      return "type";
//    case eAst::kIdentity:
//      return "identity";
//    case eAst::kValue:
//      return "value";
//    case eAst::kInt:
//      return "int";
//    case eAst::kUint:
//      return "uint";
//    case eAst::kReal:
//      return "real";
//    case eAst::kByte:
//      return "byte";
//    case eAst::kBit:
//      return "bit";
//    case eAst::kStr:
//      return "str";
//    case eAst::kArray:
//      return "array";
//    case eAst::kPointer:
//      return "pointer";
//    case eAst::kMemory:
//      return "memory";
//    case eAst::kFunction:
//      return "function";
//    case eAst::kInclude:
//      return "include";
//    case eAst::kMacro:
//      return "macro";
//    case eAst::kEndmacro:
//      return "endmacro";
//    case eAst::kEnter:
//      return "enter";
//    case eAst::kStart:
//      return "start";
//    case eAst::kUse:
//      return "use";
//    case eAst::kClass:
//      return "class";
//    case eAst::kObj:
//      return "obj";
//    case eAst::kPrint:
//      return "print";
//    case eAst::kPrivate:
//      return "private";
//    case eAst::kPublic:
//      return "public";
//    case eAst::kConst:
//      return "const";
//    case eAst::kStatic:
//      return "";
//    case eAst::kRef:
//      return "ref";
//    case eAst::kIf:
//      return "if";
//    case eAst::kElse:
//      return "else";
//    case eAst::kElif:
//      return "elif";
//    case eAst::kWhile:
//      return "while";
//    case eAst::kFor:
//      return "for";
//    case eAst::kSwitch:
//      return "switch";
//    case eAst::kCase:
//      return "case";
//    case eAst::kDefault:
//      return "default";
//    case eAst::kBreak:
//      return "break";
//    case eAst::kContinue:
//      return "continue";
//    case eAst::kReturn:
//      return "return";
//    case eAst::kStatement:
//      return "statement";
//    case eAst::kExpression:
//      return "expression";
//    case eAst::kProgram:
//      return "program";
//    case eAst::kPragmaticBlock:
//      return "pragmatic_block";
//    case eAst::kFunctionalBlock:
//      return "functional_block";
//    case eAst::kConditionalBlock:
//      return "conditional_block";
//    case eAst::kIterativeBlock:
//      return "iterative_block";
//    case eAst::kArguments:
//      return "arguments";
//    case eAst::kParameter:
//      return "parameter";
//    case eAst::kParameterList:
//      return "parameter_list";
//    case eAst::kTypeConstraints:
//      return "type_constraints";
//    case eAst::kCaptureList:
//      return "capture_list";
//    case eAst::kSubexpression:
//      return "subexpression";
//    case eAst::kTypeList:
//      return "type_list";
//    case eAst::kGenericList:
//      return "generic_list";
//    case eAst::kFunctionCall:
//      return "function_call";
//    case eAst::kTypeCall:
//      return "type_call";
//    case eAst::kTypeArguments:
//      return "type_arguments";
//    case eAst::kIndexOperator:
//      return "index_operator";
//    case eAst::kIndexArguments:
//      return "index_arguments";
//    case eAst::kUnaryMinus:
//      return "unary_minus";
//    case eAst::kDereference:
//      return "dereference";
//    case eAst::kAddressOf:
//      return "address_of";
//    default:
//      return "eAst invalid enum value";
//  }
//}
//
// constexpr eAst TkTypeToAstNodeType(eTk t) {
//  switch (t) {
//    case eTk::kNone:
//      return eAst::kNone;
//    case eTk::kInvalid:
//      return eAst::kInvalid;
//    case eTk::kEof:
//      return eAst::kEof;
//    case eTk::kLineComment:
//      return eAst::kLineComment;
//    case eTk::kBlockComment:
//      return eAst::kBlockComment;
//    case eTk::kNewline:
//      return eAst::kNewline;
//    case eTk::kWhitespace:
//      return eAst::kWhitespace;
//    case eTk::kStringLiteral:
//      return eAst::kStringLiteral;
//    case eTk::kNumberLiteral:
//      return eAst::kNumberLiteral;
//    case eTk::kDoubleLiteral:
//      return eAst::kDoubleLiteral;
//    case eTk::kByteLiteral:
//      return eAst::kByteLiteral;
//    case eTk::kBoolLiteral:
//      return eAst::kBoolLiteral;
//    case eTk::kUnsignedLiteral:
//      return eAst::kUnsignedLiteral;
//    case eTk::kIdentifier:
//      return eAst::kIdentifier;
//    case eTk::kSimpleAssignment:
//      return eAst::kSimpleAssignment;
//    case eTk::kAdditionAssignment:
//      return eAst::kAdditionAssignment;
//    case eTk::kSubtractionAssignment:
//      return eAst::kSubtractionAssignment;
//    case eTk::kMultiplicationAssignment:
//      return eAst::kMultiplicationAssignment;
//    case eTk::kDivisionAssignment:
//      return eAst::kDivisionAssignment;
//    case eTk::kRemainderAssignment:
//      return eAst::kRemainderAssignment;
//    case eTk::kBitwiseAndAssignment:
//      return eAst::kBitwiseAndAssignment;
//    case eTk::kBitwiseOrAssignment:
//      return eAst::kBitwiseOrAssignment;
//    case eTk::kBitwiseXorAssignment:
//      return eAst::kBitwiseXorAssignment;
//    case eTk::kLeftShiftAssignment:
//      return eAst::kLeftShiftAssignment;
//    case eTk::kRightShiftAssignment:
//      return eAst::kRightShiftAssignment;
//    case eTk::kIncrement:
//      return eAst::kIncrement;
//    case eTk::kDecrement:
//      return eAst::kDecrement;
//    case eTk::kAddition:
//      return eAst::kAddition;
//    case eTk::kSubtraction:
//      return eAst::kSubtraction;
//    case eTk::kMultiplication:
//      return eAst::kMultiplication;
//    case eTk::kDivision:
//      return eAst::kDivision;
//    case eTk::kRemainder:
//      return eAst::kRemainder;
//    case eTk::kBitwiseNot:
//      return eAst::kBitwiseNot;
//    case eTk::kBitwiseAnd:
//      return eAst::kBitwiseAnd;
//    case eTk::kBitwiseOr:
//      return eAst::kBitwiseOr;
//    case eTk::kBitwiseXor:
//      return eAst::kBitwiseXor;
//    case eTk::kBitwiseLeftShift:
//      return eAst::kBitwiseLeftShift;
//    case eTk::kBitwiseRightShift:
//      return eAst::kBitwiseRightShift;
//    case eTk::kNegation:
//      return eAst::kNegation;
//    case eTk::kLogicalAnd:
//      return eAst::kLogicalAnd;
//    case eTk::kLogicalOr:
//      return eAst::kLogicalOr;
//    case eTk::kEqual:
//      return eAst::kEqual;
//    case eTk::kNotEqual:
//      return eAst::kNotEqual;
//    case eTk::kLessThan:
//      return eAst::kLessThan;
//    case eTk::kGreaterThan:
//      return eAst::kGreaterThan;
//    case eTk::kLessThanOrEqual:
//      return eAst::kLessThanOrEqual;
//    case eTk::kGreaterThanOrEqual:
//      return eAst::kGreaterThanOrEqual;
//    case eTk::kThreeWayComparison:
//      return eAst::kThreeWayComparison;
//    case eTk::kOpenParen:
//      return eAst::kOpenParen;
//    case eTk::kCloseParen:
//      return eAst::kCloseParen;
//    case eTk::kOpenBrace:
//      return eAst::kOpenBrace;
//    case eTk::kCloseBrace:
//      return eAst::kCloseBrace;
//    case eTk::kOpenBracket:
//      return eAst::kOpenBracket;
//    case eTk::kCloseBracket:
//      return eAst::kCloseBracket;
//    case eTk::kSemicolon:
//      return eAst::kSemicolon;
//    case eTk::kColon:
//      return eAst::kColon;
//    case eTk::kComma:
//      return eAst::kComma;
//    case eTk::kPeriod:
//      return eAst::kPeriod;
//    case eTk::kEllipsis:
//      return eAst::kEllipsis;
//    case eTk::kCommercialAt:
//      return eAst::kCommericalAt;
//    case eTk::kNoneLiteral:
//      return eAst::kNoneLiteral;
//    case eTk::kTrueLiteral:
//      return eAst::kTrueLiteral;
//    case eTk::kFalseLiteral:
//      return eAst::kFalseLiteral;
//    case eTk::kType:
//      return eAst::kType;
//    case eTk::kIdentity:
//      return eAst::kIdentity;
//    case eTk::kValue:
//      return eAst::kValue;
//    case eTk::kInt:
//      return eAst::kInt;
//    case eTk::kUint:
//      return eAst::kUint;
//    case eTk::kReal:
//      return eAst::kReal;
//    case eTk::kByte:
//      return eAst::kByte;
//    case eTk::kBit:
//      return eAst::kBit;
//    case eTk::kStr:
//      return eAst::kStr;
//    case eTk::kArray:
//      return eAst::kArray;
//    case eTk::kPointer:
//      return eAst::kPointer;
//    case eTk::kMemory:
//      return eAst::kMemory;
//    case eTk::kFunction:
//      return eAst::kFunction;
//    case eTk::kInclude:
//      return eAst::kInclude;
//    case eTk::kMacro:
//      return eAst::kMacro;
//    case eTk::kEndmacro:
//      return eAst::kEndmacro;
//    case eTk::kEnter:
//      return eAst::kEnter;
//    case eTk::kStart:
//      return eAst::kStart;
//    case eTk::kUse:
//      return eAst::kUse;
//    case eTk::kClass:
//      return eAst::kClass;
//    case eTk::kObj:
//      return eAst::kObj;
//    case eTk::kPrint:
//      return eAst::kPrint;
//    case eTk::kPrivate:
//      return eAst::kPrivate;
//    case eTk::kPublic:
//      return eAst::kPublic;
//    case eTk::kConst:
//      return eAst::kConst;
//    case eTk::kStatic:
//      return eAst::kStatic;
//    case eTk::kRef:
//      return eAst::kRef;
//    case eTk::kIf:
//      return eAst::kIf;
//    case eTk::kElse:
//      return eAst::kElse;
//    case eTk::kElif:
//      return eAst::kElif;
//    case eTk::kWhile:
//      return eAst::kWhile;
//    case eTk::kFor:
//      return eAst::kFor;
//    case eTk::kSwitch:
//      return eAst::kSwitch;
//    case eTk::kCase:
//      return eAst::kCase;
//    case eTk::kDefault:
//      return eAst::kDefault;
//    case eTk::kBreak:
//      return eAst::kBreak;
//    case eTk::kContinue:
//      return eAst::kContinue;
//    case eTk::kReturn:
//      return eAst::kReturn;
//    default:
//      return eAst::kNone;
//  }
//}
//
// constexpr eAssoc TkTypeAssoc(eTk t) {
//  switch (t) {
//    case eTk::kNone:
//    case eTk::kInvalid:
//    case eTk::kEof:
//    case eTk::kLineComment:
//    case eTk::kBlockComment:
//    case eTk::kNewline:
//    case eTk::kWhitespace:
//    case eTk::kStringLiteral:
//    case eTk::kNumberLiteral:
//    case eTk::kDoubleLiteral:
//    case eTk::kByteLiteral:
//    case eTk::kBoolLiteral:
//    case eTk::kUnsignedLiteral:
//    case eTk::kIdentifier:
//    case eTk::kNoneLiteral:
//    case eTk::kTrueLiteral:
//    case eTk::kFalseLiteral:
//    case eTk::kType:
//    case eTk::kIdentity:
//    case eTk::kValue:
//    case eTk::kInt:
//    case eTk::kUint:
//    case eTk::kReal:
//    case eTk::kByte:
//    case eTk::kBit:
//    case eTk::kStr:
//    case eTk::kArray:
//    case eTk::kPointer:
//    case eTk::kMemory:
//    case eTk::kFunction:
//    case eTk::kInclude:
//    case eTk::kMacro:
//    case eTk::kEndmacro:
//    case eTk::kEnter:
//    case eTk::kStart:
//    case eTk::kUse:
//    case eTk::kClass:
//    case eTk::kObj:
//    case eTk::kPrint:
//    case eTk::kPrivate:
//    case eTk::kPublic:
//    case eTk::kConst:
//    case eTk::kStatic:
//    case eTk::kRef:
//    case eTk::kIf:
//    case eTk::kElse:
//    case eTk::kElif:
//    case eTk::kWhile:
//    case eTk::kFor:
//    case eTk::kSwitch:
//    case eTk::kCase:
//    case eTk::kDefault:
//    case eTk::kBreak:
//    case eTk::kContinue:
//    case eTk::kReturn:
//      return eAssoc::kNone;
//    case eTk::kSimpleAssignment:
//    case eTk::kAdditionAssignment:
//    case eTk::kSubtractionAssignment:
//    case eTk::kMultiplicationAssignment:
//    case eTk::kDivisionAssignment:
//    case eTk::kRemainderAssignment:
//    case eTk::kBitwiseAndAssignment:
//    case eTk::kBitwiseOrAssignment:
//    case eTk::kBitwiseXorAssignment:
//    case eTk::kLeftShiftAssignment:
//    case eTk::kRightShiftAssignment:
//    case eTk::kNegation:
//    case eTk::kBitwiseNot:
//      return eAssoc::kRight;
//    case eTk::kAddition:
//    case eTk::kSubtraction:
//    case eTk::kMultiplication:
//    case eTk::kDivision:
//    case eTk::kRemainder:
//    case eTk::kBitwiseAnd:
//    case eTk::kBitwiseOr:
//    case eTk::kBitwiseXor:
//    case eTk::kBitwiseLeftShift:
//    case eTk::kBitwiseRightShift:
//    case eTk::kLogicalAnd:
//    case eTk::kLogicalOr:
//    case eTk::kEqual:
//    case eTk::kNotEqual:
//    case eTk::kLessThan:
//    case eTk::kGreaterThan:
//    case eTk::kLessThanOrEqual:
//    case eTk::kGreaterThanOrEqual:
//    case eTk::kThreeWayComparison:
//    case eTk::kIncrement:
//    case eTk::kDecrement:
//    case eTk::kPeriod:
//      return eAssoc::kLeft;
//    case eTk::kOpenParen:
//    case eTk::kCloseParen:
//    case eTk::kOpenBrace:
//    case eTk::kCloseBrace:
//    case eTk::kOpenBracket:
//    case eTk::kCloseBracket:
//    case eTk::kSemicolon:
//    case eTk::kColon:
//    case eTk::kComma:
//      return eAssoc::kNone;
//    case eTk::kEllipsis:
//    case eTk::kCommercialAt:
//      return eAssoc::kRight;
//    default:
//      return eAssoc::kNone;
//  }
//}
//
// constexpr eOperation TkTypeOperation(eTk t) {
//  switch (t) {
//    case eTk::kNone:
//    case eTk::kInvalid:
//    case eTk::kEof:
//    case eTk::kLineComment:
//    case eTk::kBlockComment:
//    case eTk::kNewline:
//    case eTk::kWhitespace:
//    case eTk::kStringLiteral:
//    case eTk::kNumberLiteral:
//    case eTk::kDoubleLiteral:
//    case eTk::kByteLiteral:
//    case eTk::kBoolLiteral:
//    case eTk::kUnsignedLiteral:
//    case eTk::kIdentifier:
//      return eOperation::kNone;
//      break;
//    case eTk::kSimpleAssignment:
//    case eTk::kAdditionAssignment:
//    case eTk::kSubtractionAssignment:
//    case eTk::kMultiplicationAssignment:
//    case eTk::kDivisionAssignment:
//    case eTk::kRemainderAssignment:
//    case eTk::kBitwiseAndAssignment:
//    case eTk::kBitwiseOrAssignment:
//    case eTk::kBitwiseXorAssignment:
//    case eTk::kLeftShiftAssignment:
//    case eTk::kRightShiftAssignment:
//      return eOperation::kBinary;
//      break;
//    case eTk::kIncrement:
//    case eTk::kDecrement:
//      return eOperation::kPostfix;
//      break;
//    case eTk::kAddition:
//    case eTk::kSubtraction:
//    case eTk::kMultiplication:
//    case eTk::kDivision:
//    case eTk::kRemainder:
//    case eTk::kBitwiseAnd:
//    case eTk::kBitwiseOr:
//    case eTk::kBitwiseXor:
//    case eTk::kBitwiseLeftShift:
//    case eTk::kBitwiseRightShift:
//      return eOperation::kBinary;
//      break;
//    case eTk::kNegation:
//    case eTk::kBitwiseNot:
//      return eOperation::kPrefix;
//      break;
//    case eTk::kLogicalAnd:
//    case eTk::kLogicalOr:
//    case eTk::kEqual:
//    case eTk::kNotEqual:
//    case eTk::kLessThan:
//    case eTk::kGreaterThan:
//    case eTk::kLessThanOrEqual:
//    case eTk::kGreaterThanOrEqual:
//    case eTk::kThreeWayComparison:
//      return eOperation::kBinary;
//      break;
//    case eTk::kOpenParen:
//    case eTk::kCloseParen:
//    case eTk::kOpenBrace:
//    case eTk::kCloseBrace:
//    case eTk::kOpenBracket:
//    case eTk::kCloseBracket:
//      return eOperation::kPostfix;
//      break;
//    case eTk::kSemicolon:
//    case eTk::kColon:
//    case eTk::kComma:
//      return eOperation::kNone;
//      break;
//    case eTk::kPeriod:
//      return eOperation::kBinary;
//      break;
//    case eTk::kEllipsis:
//      return eOperation::kBinary;
//      break;
//    case eTk::kCommercialAt:
//      return eOperation::kPrefix;
//      break;
//    case eTk::kNoneLiteral:
//    case eTk::kTrueLiteral:
//    case eTk::kFalseLiteral:
//    case eTk::kType:
//    case eTk::kIdentity:
//    case eTk::kValue:
//    case eTk::kInt:
//    case eTk::kUint:
//    case eTk::kReal:
//    case eTk::kByte:
//    case eTk::kBit:
//    case eTk::kStr:
//    case eTk::kArray:
//    case eTk::kPointer:
//    case eTk::kMemory:
//    case eTk::kFunction:
//    case eTk::kInclude:
//    case eTk::kMacro:
//    case eTk::kEndmacro:
//    case eTk::kEnter:
//    case eTk::kStart:
//    case eTk::kUse:
//    case eTk::kClass:
//    case eTk::kObj:
//    case eTk::kPrint:
//    case eTk::kPrivate:
//    case eTk::kPublic:
//    case eTk::kConst:
//    case eTk::kStatic:
//    case eTk::kRef:
//    case eTk::kIf:
//    case eTk::kElse:
//    case eTk::kElif:
//    case eTk::kWhile:
//    case eTk::kFor:
//    case eTk::kSwitch:
//    case eTk::kCase:
//    case eTk::kDefault:
//    case eTk::kBreak:
//    case eTk::kContinue:
//    case eTk::kReturn:
//      return eOperation::kNone;
//      break;
//    default:
//      return eOperation::kNone;
//      break;
//  }
//}
//
// constexpr ePriority TkTypePriority(eTk t) {
//  switch (t) {
//    case eTk::kNone:
//    case eTk::kInvalid:
//    case eTk::kEof:
//    case eTk::kLineComment:
//    case eTk::kBlockComment:
//    case eTk::kNewline:
//    case eTk::kWhitespace:
//      return ePriority::kNone;
//      break;
//    case eTk::kStringLiteral:
//    case eTk::kNumberLiteral:
//    case eTk::kDoubleLiteral:
//    case eTk::kByteLiteral:
//    case eTk::kBoolLiteral:
//    case eTk::kUnsignedLiteral:
//    case eTk::kIdentifier:
//      return ePriority::kMax;
//      break;
//    case eTk::kSimpleAssignment:
//    case eTk::kAdditionAssignment:
//    case eTk::kSubtractionAssignment:
//    case eTk::kMultiplicationAssignment:
//    case eTk::kDivisionAssignment:
//    case eTk::kRemainderAssignment:
//    case eTk::kBitwiseAndAssignment:
//    case eTk::kBitwiseOrAssignment:
//    case eTk::kBitwiseXorAssignment:
//    case eTk::kLeftShiftAssignment:
//    case eTk::kRightShiftAssignment:
//      return ePriority::kAssignment;
//      break;
//    case eTk::kIncrement:
//    case eTk::kDecrement:
//      return ePriority::kPostfix;
//      break;
//    case eTk::kAddition:
//    case eTk::kSubtraction:
//      return ePriority::kTerm;
//      break;
//    case eTk::kMultiplication:
//    case eTk::kDivision:
//    case eTk::kRemainder:
//      return ePriority::kFactor;
//      break;
//    case eTk::kNegation:
//    case eTk::kBitwiseNot:
//      return ePriority::kPrefix;
//      break;
//    case eTk::kBitwiseAnd:
//    case eTk::kBitwiseOr:
//    case eTk::kBitwiseXor:
//    case eTk::kBitwiseLeftShift:
//    case eTk::kBitwiseRightShift:
//    case eTk::kLogicalAnd:
//    case eTk::kLogicalOr:
//    case eTk::kEqual:
//    case eTk::kNotEqual:
//    case eTk::kLessThan:
//    case eTk::kGreaterThan:
//    case eTk::kLessThanOrEqual:
//    case eTk::kGreaterThanOrEqual:
//    case eTk::kThreeWayComparison:
//      return ePriority::kComparison;
//      break;
//    case eTk::kOpenParen:
//    case eTk::kCloseParen:
//    case eTk::kOpenBrace:
//    case eTk::kCloseBrace:
//    case eTk::kOpenBracket:
//    case eTk::kCloseBracket:
//      return ePriority::kPostfix;
//      break;
//    case eTk::kSemicolon:
//    case eTk::kColon:
//    case eTk::kComma:
//    case eTk::kEllipsis:
//    case eTk::kCommercialAt:
//      return ePriority::kMax;
//      break;
//    case eTk::kPeriod:
//      return ePriority::kAccess;
//      break;
//    case eTk::kNoneLiteral:
//    case eTk::kTrueLiteral:
//    case eTk::kFalseLiteral:
//    case eTk::kType:
//    case eTk::kIdentity:
//    case eTk::kValue:
//    case eTk::kInt:
//    case eTk::kUint:
//    case eTk::kReal:
//    case eTk::kByte:
//    case eTk::kBit:
//    case eTk::kStr:
//    case eTk::kArray:
//    case eTk::kPointer:
//    case eTk::kMemory:
//    case eTk::kFunction:
//    case eTk::kInclude:
//    case eTk::kMacro:
//    case eTk::kEndmacro:
//    case eTk::kEnter:
//    case eTk::kStart:
//    case eTk::kUse:
//    case eTk::kClass:
//    case eTk::kObj:
//    case eTk::kPrint:
//    case eTk::kPrivate:
//    case eTk::kPublic:
//    case eTk::kConst:
//    case eTk::kStatic:
//    case eTk::kRef:
//    case eTk::kIf:
//    case eTk::kElse:
//    case eTk::kElif:
//    case eTk::kWhile:
//    case eTk::kFor:
//    case eTk::kSwitch:
//    case eTk::kCase:
//    case eTk::kDefault:
//    case eTk::kBreak:
//    case eTk::kContinue:
//    case eTk::kReturn:
//      return ePriority::kMax;
//      break;
//    default:
//      return ePriority::kNone;
//      break;
//  }
//}
//
// constexpr bool TkTypeIsKeyword(eTk t) {
//  switch (t) {
//    case eTk::kInclude:
//    case eTk::kMacro:
//    case eTk::kEndmacro:
//    case eTk::kEnter:
//    case eTk::kStart:
//    case eTk::kUse:
//    case eTk::kClass:
//    case eTk::kObj:
//    case eTk::kPrivate:
//    case eTk::kPublic:
//    case eTk::kConst:
//    case eTk::kStatic:
//    case eTk::kIf:
//    case eTk::kElse:
//    case eTk::kElif:
//    case eTk::kWhile:
//    case eTk::kFor:
//    case eTk::kSwitch:
//    case eTk::kCase:
//    case eTk::kDefault:
//    case eTk::kBreak:
//    case eTk::kContinue:
//    case eTk::kReturn:
//    case eTk::kPrint:
//    case eTk::kNoneLiteral:
//    case eTk::kTrueLiteral:
//    case eTk::kFalseLiteral:
//    case eTk::kType:
//    case eTk::kIdentity:
//    case eTk::kValue:
//    case eTk::kInt:
//    case eTk::kUint:
//    case eTk::kReal:
//    case eTk::kByte:
//    case eTk::kBit:
//    case eTk::kStr:
//      return true;
//    default:
//      return false;
//  }
//}
//
// constexpr bool TkTypeIsOpeningScope(eTk t) {
//  switch (t) {
//    case eTk::kOpenParen:
//    case eTk::kOpenBrace:
//    case eTk::kOpenBracket:
//      return true;
//    default:
//      return false;
//  }
//}
//
// constexpr bool TkTypeIsClosingScope(eTk t) {
//  switch (t) {
//    case eTk::kCloseParen:
//    case eTk::kCloseBrace:
//    case eTk::kCloseBracket:
//      return true;
//    default:
//      return false;
//  }
//}
//
// constexpr bool TkTypeIsClosingScopeOf(eTk topen, eTk tclose) {
//  return (topen == eTk::kOpenParen && tclose == eTk::kCloseParen) ||
//         (topen == eTk::kOpenBrace && tclose == eTk::kCloseBrace) ||
//         (topen == eTk::kOpenBracket && tclose == eTk::kCloseBracket);
//}
//
//constexpr void Tk::SetLine(std::size_t line) { line_ = line; }
//
//constexpr void Tk::SetCol(std::size_t col) { col_ = col; }
//
//constexpr eTk Tk::Type() const noexcept { return type_; }
//
//constexpr std::size_t Tk::Size() const noexcept { return literal_.size(); }
//
//constexpr std::size_t Tk::Line() const noexcept { return line_; }
//
//constexpr std::size_t Tk::Col() const noexcept { return col_; }
//
//constexpr const std::string& Tk::Literal() const noexcept { return literal_; }
//
//constexpr ePriority Tk::Priority() const noexcept {
//  return TkTypePriority(type_);
//};
//
//constexpr eAssoc Tk::Assoc() const noexcept { return TkTypeAssoc(type_); }
//
//constexpr eOperation Tk::Operation() const noexcept {
//  return TkTypeOperation(type_);
//}
//
//constexpr bool Tk::TypeIs(eTk type) const noexcept { return type_ == type; }
//
//constexpr bool Tk::TypeAndLitIs(eTk kind,
//                                const std::string& literal) const noexcept {
//  return type_ == kind && literal_ == literal;
//}
//
//constexpr bool Tk::IsKeyword() const noexcept { return TkTypeIsKeyword(type_); }
//
//constexpr bool Tk::IsOpeningScope() const noexcept {
//  return TkTypeIsOpeningScope(type_);
//}
//
//constexpr bool Tk::IsClosingScope() const noexcept {
//  return TkTypeIsClosingScope(type_);
//}
//
//constexpr bool Tk::IsClosingScopeOf(eTk topen) const noexcept {
//  return TkTypeIsClosingScopeOf(topen, type_);
//}
//
//constexpr eAst Tk::NodeType() const noexcept {
//  return TkTypeToAstNodeType(type_);
//}
//
//Ast::Ast(const Tk& t)
//    : type_(TkTypeToAstNodeType(t.Type())), literal_(t.Literal()) {}
//
//Ast::Ast(eAst type, std::vector<Tk>::iterator beg,
//         std::vector<Tk>::iterator end)
//    : type_(type) {
//  literal_ = "";
//  for (std::vector<Tk>::iterator it = beg; it != end; it++) {
//    literal_ += it->Literal();
//  }
//}
//
//constexpr eAst Ast::Type() const noexcept { return type_; }
//
//constexpr const std::string& Ast::Literal() const noexcept { return literal_; }
//
//bool Ast::Leaf() const noexcept { return children_.empty(); }
//
//constexpr bool Ast::Root() const noexcept { return parent_ == nullptr; }
//
//bool Ast::Branch() const noexcept { return !children_.empty(); }
//
//Ast& Ast::Parent() {
//  if (parent_ != nullptr)
//    return *parent_;
//  else
//    throw std::out_of_range("ast node parent() called on node with no parent.");
//}
//
//constexpr void Ast::SetParent(Ast* parent) { parent_ = parent; }
//
//constexpr void Ast::PopParent() {
//  if (parent_ != nullptr)
//    parent_ = nullptr;
//  else
//    throw std::out_of_range(
//        "Ast node PopParent() called on node with no parent.");
//}
//
//std::size_t Ast::Size() const { return children_.size(); }
//
//Ast& Ast::PushBack(const Ast& nd) {
//  children_.push_back(nd);
//  auto& pushed = children_.back();
//  pushed.SetParent(this);
//  return pushed;
//}
//
//Ast& Ast::PushFront(const Ast& nd) {
//  children_.push_front(nd);
//  auto& pushed = children_.front();
//  pushed.SetParent(this);
//  return pushed;
//}
//
//Ast& Ast::PushBack(Ast&& nd) {
//  children_.push_back(std::move(nd));
//  auto& pushed = children_.back();
//  pushed.SetParent(this);
//  return pushed;
//}
//
//Ast& Ast::PushFront(Ast&& nd) {
//  children_.push_front(std::move(nd));
//  auto& pushed = children_.front();
//  pushed.SetParent(this);
//  return pushed;
//}
//
//Ast Ast::PopBack() {
//  if (!children_.empty()) {
//    Ast popped = children_.back();
//    children_.pop_back();
//    popped.PopParent();
//    return popped;
//  } else
//    throw std::out_of_range(
//        "Ast node pop_back() called on node with no children.");
//}
//
//Ast Ast::PopFront() {
//  if (!children_.empty()) {
//    Ast popped = children_.front();
//    children_.pop_front();
//    popped.PopParent();
//    return popped;
//  } else
//    throw std::out_of_range(
//        "Ast node pop_front() called on node with no children.");
//}
//
//Ast& Ast::Front() {
//  if (!children_.empty())
//    return children_.front();
//  else
//    throw std::out_of_range(
//        "Ast node front() called on node with no children.");
//}
//
//Ast& Ast::Back() {
//  if (!children_.empty())
//    return children_.back();
//  else
//    throw std::out_of_range(
//        "Ast node front() called on node with no children.");
//}
//
//Ast& Ast::At(std::size_t index) {
//  if (index >= children_.size())
//    throw std::out_of_range("Ast node at() called with index out of range.");
//  if (index == 0) return children_.front();
//
//  return [this, &index]() -> Ast& {
//    auto it = children_.begin();
//    for (std::size_t i = 0; i < index; i++) {
//      it++;
//    }
//    return *it;
//  }();
//}
//
//const Ast& Ast::operator[](std::size_t index) const {
//  if (index >= children_.size())
//    throw std::out_of_range(
//        "ast node [] index operator called with index out of range.");
//  if (index == 0) return children_.front();
//
//  return [this, &index]() -> const Ast& {
//    auto it = children_.begin();
//    for (std::size_t i = 0; i < index; i++) {
//      it++;
//    }
//    return *it;
//  }();
//}
//
//Ast& Ast::operator[](std::size_t index) {
//  if (index >= children_.size())
//    throw std::out_of_range(
//        "ast node [] index operator called with index out of range.");
//  if (index == 0) return children_.front();
//
//  return [this, &index]() -> Ast& {
//    auto it = children_.begin();
//    for (std::size_t i = 0; i < index; i++) {
//      it++;
//    }
//    return *it;
//  }();
//}
//
//bool Ast::TypeIs(eAst type) const noexcept { return type_ == type; }