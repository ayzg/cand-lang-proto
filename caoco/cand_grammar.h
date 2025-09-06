#pragma once
#include "import_stl.h"
#include "string_constant.h"
// Macro: STRING_CONSTANT
// Description: Creates a string_constant from a string literal.
//              Access string value with string_constant<>::value.
#define STRING_CONSTANT(STR) string_constant<LAMBDA_STRING(STR)>

namespace grammar {
namespace characters {
using kEofile = character_constant<'\0'>;
using kWhitespace = character_constant<' '>;
using kNewline = character_constant<'\n'>;

using kHash = character_constant<'#'>;
using kCommercialAt = character_constant<'@'>;
using kAdd = character_constant<'+'>;
using kSub = character_constant<'-'>;
using kMul = character_constant<'*'>;
using kDiv = character_constant<'/'>;
using kMod = character_constant<'%'>;
using kAnd = character_constant<'&'>;
using kOr = character_constant<'|'>;
using kXor = character_constant<'^'>;
using kNot = character_constant<'!'>;
using kLsh = character_constant<'<'>;
using kRsh = character_constant<'>'>;
using kEq = character_constant<'='>;

using kLparen = character_constant<'('>;
using kRparen = character_constant<')'>;
using kLbrace = character_constant<'{'>;
using kRbrace = character_constant<'}'>;
using kLbracket = character_constant<'['>;
using kRbracket = character_constant<']'>;

using kSemicolon = character_constant<';'>;
using kColon = character_constant<':'>;
using kComma = character_constant<','>;
using kPeriod = character_constant<'.'>;
using kBacklash = character_constant<'\\'>;
using kApostrophe = character_constant<'\''>;
using kTilde = character_constant<'~'>;
}  // namespace characters
namespace keywords {
using kLib = STRING_CONSTANT("lib");
using kMain = STRING_CONSTANT("main");
using kImport = STRING_CONSTANT("import");
using kUse = STRING_CONSTANT("use");
using kDef = STRING_CONSTANT("def");
using kFn = STRING_CONSTANT("fn");
using kClass = STRING_CONSTANT("class");
using kNamespace = STRING_CONSTANT("namespace");

using kRef = STRING_CONSTANT("ref");
using kPrivate = STRING_CONSTANT("private");
using kPublic = STRING_CONSTANT("public");
using kConst = STRING_CONSTANT("const");
using kStatic = STRING_CONSTANT("static");

using kIf = STRING_CONSTANT("if");
using kElse = STRING_CONSTANT("else");
using kElif = STRING_CONSTANT("elif");
using kWhile = STRING_CONSTANT("while");
using kFor = STRING_CONSTANT("for");
using kSwitch = STRING_CONSTANT("switch");
using kCase = STRING_CONSTANT("case");
using kDefault = STRING_CONSTANT("default");
using kBreak = STRING_CONSTANT("break");
using kContinue = STRING_CONSTANT("continue");
using kReturn = STRING_CONSTANT("return");

using kNone = STRING_CONSTANT("none");
using kTrue = STRING_CONSTANT("true");
using kFalse = STRING_CONSTANT("false");

using kAny = STRING_CONSTANT("any");
using kInt = STRING_CONSTANT("int");
using kUnsigned = STRING_CONSTANT("unsigned");
using kDouble = STRING_CONSTANT("double");
using kChar = STRING_CONSTANT("char");
using kByte = STRING_CONSTANT("byte");
using kBit = STRING_CONSTANT("bool");
using kStr = STRING_CONSTANT("string");
using kAuto = STRING_CONSTANT("auto");

using kList = STRING_CONSTANT("list");
using kArray = STRING_CONSTANT("array");

using kCout = STRING_CONSTANT("cout");
using kCin = STRING_CONSTANT("cin");

}  // namespace keywords
using kAllKeywords = std::tuple<
    keywords::kLib, keywords::kMain, keywords::kImport, keywords::kUse,
    keywords::kDef, keywords::kFn, keywords::kClass, keywords::kNamespace,
    keywords::kPrivate, keywords::kPublic, keywords::kConst, keywords::kStatic,
    keywords::kIf, keywords::kElse, keywords::kElif, keywords::kWhile,
    keywords::kFor, keywords::kSwitch, keywords::kCase, keywords::kDefault,
    keywords::kBreak, keywords::kContinue, keywords::kReturn, keywords::kNone,
    keywords::kTrue, keywords::kFalse, keywords::kAny, keywords::kInt,
    keywords::kUnsigned, keywords::kRef, keywords::kChar, keywords::kDouble,
    keywords::kByte, keywords::kBit, keywords::kStr, keywords::kAuto,
    keywords::kList, keywords::kArray>;
static auto constinit kAllKeywordsArray =
    []<typename... Keywords>(std::tuple<Keywords...> const& t) consteval
    -> auto {
  auto ret = std::array<std::string_view, sizeof...(Keywords)>{
      std::get<Keywords>(t).value...};
  return ret;
}(kAllKeywords{});

namespace directives {
using kInclude = STRING_CONSTANT("#include");
using kMacro = STRING_CONSTANT("#macro");
using kEndmacro = STRING_CONSTANT("#endmacro");
using kIf = STRING_CONSTANT("#if");
using kElse = STRING_CONSTANT("#else");
using kElif = STRING_CONSTANT("#elif");
}  // namespace directives

namespace operators {
using kHash = STRING_CONSTANT("#");
using kAdd = STRING_CONSTANT("+");
using kSub = STRING_CONSTANT("-");
using kMul = STRING_CONSTANT("*");
using kDiv = STRING_CONSTANT("/");
using kMod = STRING_CONSTANT("%");
using kAnd = STRING_CONSTANT("&");
using kOr = STRING_CONSTANT("|");
using kXor = STRING_CONSTANT("^");
using kNot = STRING_CONSTANT("!");
using kLsh = STRING_CONSTANT("<<");
using kRsh = STRING_CONSTANT(">>");
using kEq = STRING_CONSTANT("==");
using kNeq = STRING_CONSTANT("!=");
using kLt = STRING_CONSTANT("<");
using kGt = STRING_CONSTANT(">");
using kLte = STRING_CONSTANT("<=");
using kGte = STRING_CONSTANT(">=");
using kAssign = STRING_CONSTANT("=");
using kNewAssign = STRING_CONSTANT(" : =");
using kAddAssign = STRING_CONSTANT("+=");
using kSubAssign = STRING_CONSTANT("-=");
using kMulAssign = STRING_CONSTANT("*=");
using kDivAssign = STRING_CONSTANT("/=");
using kModAssign = STRING_CONSTANT("%=");
using kAndAssign = STRING_CONSTANT("&=");
using kOrAssign = STRING_CONSTANT("|=");
using kXorAssign = STRING_CONSTANT("^=");
using kLshAssign = STRING_CONSTANT("<<=");
using kRshAssign = STRING_CONSTANT(">>=");
using kInc = STRING_CONSTANT("++");
using kDec = STRING_CONSTANT("--");
using kDot = STRING_CONSTANT(".");
using kBnot = STRING_CONSTANT("~");
using kBand = STRING_CONSTANT("&&");
using kBor = STRING_CONSTANT("||");
}  // namespace operators

namespace scopes {
using kOpenParen = STRING_CONSTANT("(");
using kCloseParen = STRING_CONSTANT(")");
using kOpenBrace = STRING_CONSTANT("{");
using kCloseBrace = STRING_CONSTANT("}");
using kOpenBracket = STRING_CONSTANT("[");
using kCloseBracket = STRING_CONSTANT("]");
using kSemicolon = STRING_CONSTANT(";");
using kColon = STRING_CONSTANT(":");
using kComma = STRING_CONSTANT(",");
using kPeriod = STRING_CONSTANT(".");
using kDoubleColon = STRING_CONSTANT("::");
using kEllipsis = STRING_CONSTANT("...");
using kCommercialAt = STRING_CONSTANT("@");
}  // namespace scopes

using kEofile = STRING_CONSTANT("\0");
using kWhitespace = STRING_CONSTANT(" ");
using kNewline = STRING_CONSTANT("\n");
using kBlockComment = STRING_CONSTANT("///");
using kLineComment = STRING_CONSTANT("//");

}  // namespace grammar

#undef STRING_CONSTANT
