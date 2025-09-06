#pragma once
#include "compiler_enum.h"
#include "import_stl.h"
namespace tk_traits {

constexpr eAst kTkTypeToAstNodeType(eTk t);

constexpr eAssoc kTkTypeAssoc(eTk t);

constexpr eOperation kTkTypeOperation(eTk t);

constexpr ePriority kTkTypePriority(eTk t);

constexpr std::string_view kTkTypeLiteral(eTk t);

constexpr bool kTkTypeIsKeyword(eTk t);

constexpr bool kTkTypeIsOpeningScope(eTk t);

constexpr bool kTkTypeIsClosingScope(eTk t);

constexpr bool kTkTypeIsClosingScopeOf(eTk topen, eTk tclose);

constexpr bool kTkTypeIsModifierKeyword(eTk t);

constexpr bool kTkTypeIsDeclarativeKeyword(eTk t);

constexpr bool kTkTypeIsSingularOperand(eTk t);

constexpr bool kTkTypeIsSingularPrefixOperator(eTk t);

constexpr bool kTkTypeIsPrimaryExpressionOpening(eTk t) {
  return kTkTypeIsSingularOperand(t) || kTkTypeIsSingularPrefixOperator(t) ||
         t == eTk::kOpenParen;
}
template <eTk TkType>
struct TkTrait {
  static constexpr eTk type = TkType;
  static constexpr std::string_view literal = kTkTypeLiteral(TkType);
  static constexpr eAst ast_type = kTkTypeToAstNodeType(TkType);
  static constexpr ePriority priority = kTkTypePriority(TkType);
  static constexpr eOperation operation = kTkTypeOperation(TkType);
  static constexpr eAssoc assoc = kTkTypeAssoc(TkType);

  // Detailed Properties
  // Keyword Properties
  static constexpr bool IsKeyword = kTkTypeIsKeyword(TkType);
  static constexpr bool IsModifierKeyword = kTkTypeIsModifierKeyword(TkType);
  static constexpr bool IsDeclarativeKeyword =
      kTkTypeIsDeclarativeKeyword(TkType);

  // Scope Properties
  static constexpr bool IsOpeningScope = kTkTypeIsOpeningScope(TkType);
  static constexpr bool IsClosingScope = kTkTypeIsClosingScope(TkType);

  // Primary Expression Properties
  static constexpr bool IsSingularOperand = kTkTypeIsSingularOperand(TkType);
  static constexpr bool IsSingularPrefixOperator =
      kTkTypeIsSingularPrefixOperator(TkType);
  static constexpr bool IsPrimaryExpressionOpening =
      kTkTypeIsPrimaryExpressionOpening(TkType);

  template <eTk OpenScopeTkType>
  static constexpr bool IsClosingScopeOf =
      kTkTypeIsClosingScopeOf(OpenScopeTkType, TkType);
};

constexpr eAst kTkTypeToAstNodeType(eTk t) {
  switch (t) {
    case eTk::kNone:
      return eAst::kNone;
    case eTk::kInvalid:
      return eAst::kInvalid;
    case eTk::kEof:
      return eAst::kEof;
    case eTk::kLineComment:
      return eAst::kLineComment;
    case eTk::kBlockComment:
      return eAst::kBlockComment;
    case eTk::kNewline:
      return eAst::kNewline;
    case eTk::kWhitespace:
      return eAst::kWhitespace;
    case eTk::kStringLiteral:
      return eAst::kStringLiteral;
    case eTk::kNumberLiteral:
      return eAst::kNumberLiteral;
    case eTk::kDoubleLiteral:
      return eAst::kDoubleLiteral;
    case eTk::kByteLiteral:
      return eAst::kByteLiteral;
    case eTk::kBoolLiteral:
      return eAst::kBoolLiteral;
    case eTk::kUnsignedLiteral:
      return eAst::kUnsignedLiteral;
    case eTk::kIdentifier:
      return eAst::kIdentifier;
    case eTk::kSimpleAssignment:
      return eAst::kSimpleAssignment;
    case eTk::kAdditionAssignment:
      return eAst::kAdditionAssignment;
    case eTk::kSubtractionAssignment:
      return eAst::kSubtractionAssignment;
    case eTk::kMultiplicationAssignment:
      return eAst::kMultiplicationAssignment;
    case eTk::kDivisionAssignment:
      return eAst::kDivisionAssignment;
    case eTk::kRemainderAssignment:
      return eAst::kRemainderAssignment;
    case eTk::kBitwiseAndAssignment:
      return eAst::kBitwiseAndAssignment;
    case eTk::kBitwiseOrAssignment:
      return eAst::kBitwiseOrAssignment;
    case eTk::kBitwiseXorAssignment:
      return eAst::kBitwiseXorAssignment;
    case eTk::kLeftShiftAssignment:
      return eAst::kLeftShiftAssignment;
    case eTk::kRightShiftAssignment:
      return eAst::kRightShiftAssignment;
    case eTk::kIncrement:
      return eAst::kIncrement;
    case eTk::kDecrement:
      return eAst::kDecrement;
    case eTk::kAddition:
      return eAst::kAddition;
    case eTk::kSubtraction:
      return eAst::kSubtraction;
    case eTk::kMultiplication:
      return eAst::kMultiplication;
    case eTk::kDivision:
      return eAst::kDivision;
    case eTk::kRemainder:
      return eAst::kRemainder;
    case eTk::kBitwiseNot:
      return eAst::kBitwiseNot;
    case eTk::kBitwiseAnd:
      return eAst::kBitwiseAnd;
    case eTk::kBitwiseOr:
      return eAst::kBitwiseOr;
    case eTk::kBitwiseXor:
      return eAst::kBitwiseXor;
    case eTk::kBitwiseLeftShift:
      return eAst::kBitwiseLeftShift;
    case eTk::kBitwiseRightShift:
      return eAst::kBitwiseRightShift;
    case eTk::kNegation:
      return eAst::kNegation;
    case eTk::kLogicalAnd:
      return eAst::kLogicalAnd;
    case eTk::kLogicalOr:
      return eAst::kLogicalOr;
    case eTk::kEqual:
      return eAst::kEqual;
    case eTk::kNotEqual:
      return eAst::kNotEqual;
    case eTk::kLessThan:
      return eAst::kLessThan;
    case eTk::kGreaterThan:
      return eAst::kGreaterThan;
    case eTk::kLessThanOrEqual:
      return eAst::kLessThanOrEqual;
    case eTk::kGreaterThanOrEqual:
      return eAst::kGreaterThanOrEqual;
    case eTk::kThreeWayComparison:
      return eAst::kThreeWayComparison;
    case eTk::kOpenParen:
      return eAst::kOpenParen;
    case eTk::kCloseParen:
      return eAst::kCloseParen;
    case eTk::kOpenBrace:
      return eAst::kOpenBrace;
    case eTk::kCloseBrace:
      return eAst::kCloseBrace;
    case eTk::kOpenBracket:
      return eAst::kOpenBracket;
    case eTk::kCloseBracket:
      return eAst::kCloseBracket;
    case eTk::kSemicolon:
      return eAst::kSemicolon;
    case eTk::kColon:
      return eAst::kColon;
    case eTk::kComma:
      return eAst::kComma;
    case eTk::kPeriod:
      return eAst::kPeriod;
    case eTk::kEllipsis:
      return eAst::kEllipsis;
    case eTk::kCommercialAt:
      return eAst::kCommericalAt;
    case eTk::kNoneLiteral:
      return eAst::kNoneLiteral;
    case eTk::kTrueLiteral:
      return eAst::kTrueLiteral;
    case eTk::kFalseLiteral:
      return eAst::kFalseLiteral;
    case eTk::kInt:
      return eAst::kInt;
    case eTk::kUint:
      return eAst::kUint;
    case eTk::kReal:
      return eAst::kReal;
    case eTk::kByte:
      return eAst::kByte;
    case eTk::kBit:
      return eAst::kBit;
    case eTk::kStr:
      return eAst::kStr;
    case eTk::kArray:
      return eAst::kArray;
    case eTk::kDirInclude:
      return eAst::kInclude;
    case eTk::kDirMacro:
      return eAst::kMacro;
    case eTk::kDirEndmacro:
      return eAst::kEndmacro;
    case eTk::kDirIf:
      return eAst::kIf;
    case eTk::kDirElse:
      return eAst::kElse;
    case eTk::kDirElif:
      return eAst::kElif;
    case eTk::kUse:
      return eAst::kUse;
    case eTk::kClass:
      return eAst::kClass;
    // case eTk::kObj:
    //   return eAst::kObj;
    // case eTk::kPrint:
    //   return eAst::kPrint;
    case eTk::kPrivate:
      return eAst::kPrivate;
    case eTk::kPublic:
      return eAst::kPublic;
    case eTk::kConst:
      return eAst::kConst;
    case eTk::kStatic:
      return eAst::kStatic;
    case eTk::kRef:
      return eAst::kRef;
    case eTk::kIf:
      return eAst::kIf;
    case eTk::kElse:
      return eAst::kElse;
    case eTk::kElif:
      return eAst::kElif;
    case eTk::kWhile:
      return eAst::kWhile;
    case eTk::kFor:
      return eAst::kFor;
    case eTk::kSwitch:
      return eAst::kSwitch;
    case eTk::kCase:
      return eAst::kCase;
    case eTk::kDefault:
      return eAst::kDefault;
    case eTk::kBreak:
      return eAst::kBreak;
    case eTk::kContinue:
      return eAst::kContinue;
    case eTk::kReturn:
      return eAst::kReturn;
    case eTk::kAuto:
      return eAst::kAuto;
    case eTk::kAny:
      return eAst::kAny;
    case eTk::kDef:
      return eAst::kDef;
    case eTk::kFn:
      return eAst::kFn;
    case eTk::kLib:
      return eAst::kLib;
    case eTk::kMain:
      return eAst::kMain;
    case eTk::kImport:
      return eAst::kImport;
    case eTk::kList:
      return eAst::kList;
    case eTk::kNamespace:
      return eAst::kNamespace;
    case eTk::kDoubleColon:
      return eAst::kDoubleColon;
    default:
      return eAst::kInvalid;
  }
}

constexpr eAssoc kTkTypeAssoc(eTk t) {
  switch (t) {
    case eTk::kSimpleAssignment:
    case eTk::kAdditionAssignment:
    case eTk::kSubtractionAssignment:
    case eTk::kMultiplicationAssignment:
    case eTk::kDivisionAssignment:
    case eTk::kRemainderAssignment:
    case eTk::kBitwiseAndAssignment:
    case eTk::kBitwiseOrAssignment:
    case eTk::kBitwiseXorAssignment:
    case eTk::kLeftShiftAssignment:
    case eTk::kRightShiftAssignment:
    case eTk::kNegation:
    case eTk::kBitwiseNot:
    case eTk::kEllipsis:
    case eTk::kCommercialAt:
      return eAssoc::kRight;
    case eTk::kAddition:
    case eTk::kSubtraction:
    case eTk::kMultiplication:
    case eTk::kDivision:
    case eTk::kRemainder:
    case eTk::kBitwiseAnd:
    case eTk::kBitwiseOr:
    case eTk::kBitwiseXor:
    case eTk::kBitwiseLeftShift:
    case eTk::kBitwiseRightShift:
    case eTk::kLogicalAnd:
    case eTk::kLogicalOr:
    case eTk::kEqual:
    case eTk::kNotEqual:
    case eTk::kLessThan:
    case eTk::kGreaterThan:
    case eTk::kLessThanOrEqual:
    case eTk::kGreaterThanOrEqual:
    case eTk::kThreeWayComparison:
    case eTk::kIncrement:
    case eTk::kDecrement:
    case eTk::kDoubleColon:
    case eTk::kPeriod:
      return eAssoc::kLeft;
    default:
      return eAssoc::kNone;
  }
}

constexpr eOperation kTkTypeOperation(eTk t) {
  switch (t) {
    case eTk::kSimpleAssignment:
    case eTk::kAdditionAssignment:
    case eTk::kSubtractionAssignment:
    case eTk::kMultiplicationAssignment:
    case eTk::kDivisionAssignment:
    case eTk::kRemainderAssignment:
    case eTk::kBitwiseAndAssignment:
    case eTk::kBitwiseOrAssignment:
    case eTk::kBitwiseXorAssignment:
    case eTk::kLeftShiftAssignment:
    case eTk::kRightShiftAssignment:
      return eOperation::kBinary;
      break;
    case eTk::kIncrement:
    case eTk::kDecrement:
      return eOperation::kPostfix;
      break;
    case eTk::kAddition:
    case eTk::kSubtraction:
    case eTk::kMultiplication:
    case eTk::kDivision:
    case eTk::kRemainder:
    case eTk::kBitwiseAnd:
    case eTk::kBitwiseOr:
    case eTk::kBitwiseXor:
    case eTk::kBitwiseLeftShift:
    case eTk::kBitwiseRightShift:
      return eOperation::kBinary;
      break;
    case eTk::kNegation:
    case eTk::kBitwiseNot:
      return eOperation::kPrefix;
      break;
    case eTk::kLogicalAnd:
    case eTk::kLogicalOr:
    case eTk::kEqual:
    case eTk::kNotEqual:
    case eTk::kLessThan:
    case eTk::kGreaterThan:
    case eTk::kLessThanOrEqual:
    case eTk::kGreaterThanOrEqual:
    case eTk::kThreeWayComparison:
      return eOperation::kBinary;
    case eTk::kOpenParen:
    case eTk::kCloseParen:
    case eTk::kOpenBrace:
    case eTk::kCloseBrace:
    case eTk::kOpenBracket:
    case eTk::kCloseBracket:
      return eOperation::kPostfix;
    case eTk::kPeriod:
    case eTk::kDoubleColon:
      return eOperation::kBinary;
    case eTk::kEllipsis:
      return eOperation::kBinary;
    case eTk::kCommercialAt:
      return eOperation::kPrefix;
    default:
      return eOperation::kNone;
  }
}

constexpr ePriority kTkTypePriority(eTk t) {
  switch (t) {
    case eTk::kNone:
    case eTk::kInvalid:
    case eTk::kEof:
    case eTk::kLineComment:
    case eTk::kBlockComment:
    case eTk::kNewline:
    case eTk::kWhitespace:
      return ePriority::kNone;
      break;
    case eTk::kStringLiteral:
    case eTk::kNumberLiteral:
    case eTk::kDoubleLiteral:
    case eTk::kByteLiteral:
    case eTk::kBoolLiteral:
    case eTk::kUnsignedLiteral:
    case eTk::kIdentifier:
      return ePriority::kMax;
      break;
    case eTk::kSimpleAssignment:
    case eTk::kAdditionAssignment:
    case eTk::kSubtractionAssignment:
    case eTk::kMultiplicationAssignment:
    case eTk::kDivisionAssignment:
    case eTk::kRemainderAssignment:
    case eTk::kBitwiseAndAssignment:
    case eTk::kBitwiseOrAssignment:
    case eTk::kBitwiseXorAssignment:
    case eTk::kLeftShiftAssignment:
    case eTk::kRightShiftAssignment:
      return ePriority::kAssignment;
      break;
    case eTk::kIncrement:
    case eTk::kDecrement:
      return ePriority::kPostfix;
      break;
    case eTk::kAddition:
    case eTk::kSubtraction:
      return ePriority::kTerm;
      break;
    case eTk::kMultiplication:
    case eTk::kDivision:
    case eTk::kRemainder:
      return ePriority::kFactor;
      break;
    case eTk::kNegation:
    case eTk::kBitwiseNot:
      return ePriority::kPrefix;
      break;
    case eTk::kBitwiseAnd:
    case eTk::kBitwiseOr:
    case eTk::kBitwiseXor:
    case eTk::kBitwiseLeftShift:
    case eTk::kBitwiseRightShift:
    case eTk::kLogicalAnd:
    case eTk::kLogicalOr:
    case eTk::kEqual:
    case eTk::kNotEqual:
    case eTk::kLessThan:
    case eTk::kGreaterThan:
    case eTk::kLessThanOrEqual:
    case eTk::kGreaterThanOrEqual:
    case eTk::kThreeWayComparison:
      return ePriority::kComparison;
      break;
    case eTk::kOpenParen:
    case eTk::kCloseParen:
    case eTk::kOpenBrace:
    case eTk::kCloseBrace:
    case eTk::kOpenBracket:
    case eTk::kCloseBracket:
      return ePriority::kPostfix;
      break;
    case eTk::kSemicolon:
    case eTk::kColon:
    case eTk::kComma:
    case eTk::kEllipsis:
    case eTk::kCommercialAt:
      return ePriority::kMax;
      break;
    case eTk::kPeriod:
    case eTk::kDoubleColon:
      return ePriority::kAccess;
      break;
    case eTk::kNoneLiteral:
    case eTk::kTrueLiteral:
    case eTk::kFalseLiteral:
    case eTk::kInt:
    case eTk::kUint:
    case eTk::kReal:
    case eTk::kByte:
    case eTk::kBit:
    case eTk::kStr:
    case eTk::kArray:
    case eTk::kUse:
    case eTk::kClass:
    case eTk::kPrivate:
    case eTk::kPublic:
    case eTk::kConst:
    case eTk::kStatic:
    case eTk::kRef:
    case eTk::kIf:
    case eTk::kElse:
    case eTk::kElif:
    case eTk::kWhile:
    case eTk::kFor:
    case eTk::kSwitch:
    case eTk::kCase:
    case eTk::kDefault:
    case eTk::kBreak:
    case eTk::kContinue:
    case eTk::kReturn:
    case eTk::kDef:
    case eTk::kAuto:
    case eTk::kFn:
    case eTk::kAny:
    case eTk::kLib:
    case eTk::kMain:
    case eTk::kImport:
    case eTk::kList:
    case eTk::kNamespace:
      return ePriority::kMax;
      break;
    default:
      return ePriority::kNone;
  }
}

constexpr std::string_view kTkTypeLiteral(eTk t) {
  switch (t) {
    case eTk::kNone:
      return "none";
    case eTk::kInvalid:
      return "invalid";
    case eTk::kEof:
      return "eof";
    case eTk::kLineComment:
      return "line comment";
    case eTk::kBlockComment:
      return "block comment";
    case eTk::kNewline:
      return "newline";
    case eTk::kWhitespace:
      return "whitespace";
    case eTk::kStringLiteral:
      return "string literal";
    case eTk::kNumberLiteral:
      return "number literal";
    case eTk::kDoubleLiteral:
      return "double literal";
    case eTk::kByteLiteral:
      return "byte literal";
    case eTk::kBoolLiteral:
      return "bool literal";
    case eTk::kUnsignedLiteral:
      return "unsigned literal";
    case eTk::kIdentifier:
      return "identifier";
    case eTk::kSimpleAssignment:
      return "=";
    case eTk::kAdditionAssignment:
      return "+=";
    case eTk::kSubtractionAssignment:
      return "-=";
    case eTk::kMultiplicationAssignment:
      return "*=";
    case eTk::kDivisionAssignment:
      return "/=";
    case eTk::kRemainderAssignment:
      return "%=";
    case eTk::kBitwiseAndAssignment:
      return "&=";
    case eTk::kBitwiseOrAssignment:
      return "|=";
    case eTk::kBitwiseXorAssignment:
      return "^=";
    case eTk::kLeftShiftAssignment:
      return "<<=";
    case eTk::kRightShiftAssignment:
      return ">>=";
    case eTk::kIncrement:
      return "++";
    case eTk::kDecrement:
      return "--";
    case eTk::kAddition:
      return "+";
    case eTk::kSubtraction:
      return "-";
    case eTk::kMultiplication:
      return "*";
    case eTk::kDivision:
      return "/";
    case eTk::kRemainder:
      return "%";
    case eTk::kBitwiseNot:
      return "~";
    case eTk::kBitwiseAnd:
      return "&";
    case eTk::kBitwiseOr:
      return "|";
    case eTk::kBitwiseXor:
      return "^";
    case eTk::kBitwiseLeftShift:
      return "<<";
    case eTk::kBitwiseRightShift:
      return ">>";
    case eTk::kNegation:
      return "!";
    case eTk::kLogicalAnd:
      return "&&";
    case eTk::kLogicalOr:
      return "||";
    case eTk::kEqual:
      return "==";
    case eTk::kNotEqual:
      return "!=";
    case eTk::kLessThan:
      return "<";
    case eTk::kGreaterThan:
      return ">";
    case eTk::kLessThanOrEqual:
      return "<=";
    case eTk::kGreaterThanOrEqual:
      return ">=";
    case eTk::kThreeWayComparison:
      return "<=>";
    case eTk::kOpenParen:
      return "(";
    case eTk::kCloseParen:
      return ")";
    case eTk::kOpenBrace:
      return "{";
    case eTk::kCloseBrace:
      return "}";
    case eTk::kOpenBracket:
      return "[";
    case eTk::kCloseBracket:
      return "]";
    case eTk::kSemicolon:
      return ";";
    case eTk::kColon:
      return ":";
    case eTk::kComma:
      return ",";
    case eTk::kPeriod:
      return ".";
    case eTk::kEllipsis:
      return "...";
    case eTk::kCommercialAt:
      return "@";
    case eTk::kNoneLiteral:
      return "none";
    case eTk::kTrueLiteral:
      return "true";
    case eTk::kFalseLiteral:
      return "false";
    case eTk::kInt:
      return "int";
    case eTk::kUint:
      return "unsigned";
    case eTk::kReal:
      return "double";
    case eTk::kByte:
      return "byte";
    case eTk::kBit:
      return "bool";
    case eTk::kStr:
      return "string";
    case eTk::kArray:
      return "array";
    case eTk::kDirInclude:
      return "#include";
    case eTk::kDirMacro:
      return "#macro";
    case eTk::kDirEndmacro:
      return "#endmacro";
    case eTk::kDirIf:
      return "#if";
    case eTk::kDirElse:
      return "#else";
    case eTk::kDirElif:
      return "#elif";
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
      return "static";
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
      return "::";
    default:
      return "invalid";
  }
}

constexpr bool kTkTypeIsKeyword(eTk t) {
  switch (t) {
    case eTk::kUse:
    case eTk::kClass:
    case eTk::kPrivate:
    case eTk::kPublic:
    case eTk::kConst:
    case eTk::kStatic:
    case eTk::kIf:
    case eTk::kElse:
    case eTk::kElif:
    case eTk::kWhile:
    case eTk::kFor:
    case eTk::kSwitch:
    case eTk::kCase:
    case eTk::kDefault:
    case eTk::kBreak:
    case eTk::kContinue:
    case eTk::kReturn:
    case eTk::kNoneLiteral:
    case eTk::kTrueLiteral:
    case eTk::kFalseLiteral:
    case eTk::kInt:
    case eTk::kUint:
    case eTk::kReal:
    case eTk::kByte:
    case eTk::kBit:
    case eTk::kStr:
    case eTk::kDef:
    case eTk::kAuto:
    case eTk::kFn:
    case eTk::kLib:
    case eTk::kMain:
    case eTk::kImport:
    case eTk::kList:
    case eTk::kNamespace:
      return true;
    default:
      return false;
  }
}

constexpr bool kTkTypeIsModifierKeyword(eTk t) {
  switch (t) {
    case eTk::kPrivate:
    case eTk::kPublic:
    case eTk::kConst:
    case eTk::kStatic:
    case eTk::kRef:
      return true;
    default:
      return false;
  }
}

constexpr bool kTkTypeIsDeclarativeKeyword(eTk t) {
  // One of: def,fn,class,obj,use,lib,main,import
  switch (t) {
    case eTk::kDef:
    case eTk::kFn:
    case eTk::kClass:
    case eTk::kUse:
    case eTk::kLib:
    case eTk::kMain:
    case eTk::kImport:
    case eTk::kNamespace:
    case eTk::kIf:
    case eTk::kElse:
    case eTk::kElif:
    case eTk::kWhile:
    case eTk::kFor:
    case eTk::kSwitch:
    case eTk::kCase:
    case eTk::kDefault:
    case eTk::kBreak:
    case eTk::kContinue:
    case eTk::kReturn:
      return true;
    default:
      return false;
  }
}

constexpr bool kTkTypeIsOpeningScope(eTk t) {
  switch (t) {
    case eTk::kOpenParen:
    case eTk::kOpenBrace:
    case eTk::kOpenBracket:
      return true;
    default:
      return false;
  }
}

constexpr bool kTkTypeIsClosingScope(eTk t) {
  switch (t) {
    case eTk::kCloseParen:
    case eTk::kCloseBrace:
    case eTk::kCloseBracket:
      return true;
    default:
      return false;
  }
}

constexpr bool kTkTypeIsClosingScopeOf(eTk topen, eTk tclose) {
  return (topen == eTk::kOpenParen && tclose == eTk::kCloseParen) ||
         (topen == eTk::kOpenBrace && tclose == eTk::kCloseBrace) ||
         (topen == eTk::kOpenBracket && tclose == eTk::kCloseBracket);
}

constexpr bool kTkTypeIsSingularOperand(eTk t) {
  switch (t) {
    // Literals
    case eTk::kNumberLiteral:
    case eTk::kDoubleLiteral:
    case eTk::kStringLiteral:
    case eTk::kIdentifier:
    case eTk::kNoneLiteral:
    case eTk::kUnsignedLiteral:
    case eTk::kByteLiteral:
    case eTk::kBoolLiteral:
    case eTk::kTrueLiteral:
    case eTk::kFalseLiteral:
    // Keyword Types which are operands.
    case eTk::kInt:
    case eTk::kUint:
    case eTk::kReal:
    case eTk::kByte:
    case eTk::kBit:
    case eTk::kStr:
    case eTk::kList:
    case eTk::kArray:
      return true;
    default:
      return false;
  }
}

constexpr bool kTkTypeIsSingularPrefixOperator(eTk t) {
  switch (t) {
    case eTk::kNegation:
    case eTk::kBitwiseNot:
      return true;
    default:
      return false;
  }
}

using AllKeywordsTupleT = std::tuple<TkTrait<eTk::kUse>, TkTrait<eTk::kClass>,
                                    TkTrait<eTk::kPrivate>, TkTrait<eTk::kPublic>,
                                    TkTrait<eTk::kConst>, TkTrait<eTk::kStatic>,
                                    TkTrait<eTk::kRef>, TkTrait<eTk::kIf>,
                                    TkTrait<eTk::kElse>, TkTrait<eTk::kElif>,
                                    TkTrait<eTk::kWhile>, TkTrait<eTk::kFor>,
                                    TkTrait<eTk::kSwitch>, TkTrait<eTk::kCase>,
                                    TkTrait<eTk::kDefault>, TkTrait<eTk::kBreak>,
                                    TkTrait<eTk::kContinue>, TkTrait<eTk::kReturn>,
                                    TkTrait<eTk::kNoneLiteral>, TkTrait<eTk::kTrueLiteral>,
                                    TkTrait<eTk::kFalseLiteral>, TkTrait<eTk::kInt>,
                                    TkTrait<eTk::kUint>, TkTrait<eTk::kReal>,
                                    TkTrait<eTk::kByte>, TkTrait<eTk::kBit>,
                                    TkTrait<eTk::kStr>, TkTrait<eTk::kDef>,
                                    TkTrait<eTk::kAuto>, TkTrait<eTk::kFn>,
                                    TkTrait<eTk::kLib>, TkTrait<eTk::kMain>,
                                    TkTrait<eTk::kImport>, TkTrait<eTk::kList>,
                                    TkTrait<eTk::kNamespace>>;
constexpr AllKeywordsTupleT kAllKeywordsTuple = AllKeywordsTupleT{};
using AllDirectivesTupleT = std::tuple<TkTrait<eTk::kDirInclude>,
                                       TkTrait<eTk::kDirMacro>,
                                       TkTrait<eTk::kDirEndmacro>,
                                       TkTrait<eTk::kDirIf>,
                                       TkTrait<eTk::kDirElse>,
                                       TkTrait<eTk::kDirElif>>;
constexpr AllDirectivesTupleT kAllDirectivesTuple = AllDirectivesTupleT{};
};  // namespace tk_traits
