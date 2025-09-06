//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: unit_tests
// File: ut0_parser_basics.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_UT0_PARSER_BASICS_H
#define HEADER_GUARD_CAOCO_UT0_PARSER_BASICS_H
// Includes:
#include "lark_parser.h"
#include "lexer.h"
#include "minitest_flags.h"  // Flags to enable or disable the unit tests
#include "minitest_pch.h"    // All pre includes for each unit test
#include "minitest_util.h"   // Utility methods shared among the all unit tests
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
// todo:seperate file to control test flags
#define CAOCO_TEST_PARSER_BASICS true

#if CAOCO_TEST_PARSER_BASICS
#define CAOCO_TEST_PARSER_BASICS_SingleOperand 1
#define CAOCO_TEST_PARSER_BASICS_ValueExpr 1
#define CAOCO_TEST_PARSER_BASICS_ExprExtensive 1
#define CAOCO_TEST_PARSER_BASICS_Statements 1
#endif

#if CAOCO_TEST_PARSER_BASICS_SingleOperand
MINITEST(TestParserBasics, TestCaseSingleOperand) {
  auto expected_source = Lexer::Lex(
      "1 1.1 'string literal' '\\\'' alnumus 1c 1b 1u none ");  // TODO: #false
                                                                // #true
  ASSERT_TRUE_LOG(expected_source.Valid(),
                  "Could not tokenize source:" + expected_source.Error());
  if (!expected_source.Valid()) {
    std::cout << "Lexing Error:" << expected_source.Error();
  } else {
    auto source = expected_source.Extract();
    // Operands.
    std::vector<Ast> expected_result = {
        Ast(eAst::kNumberLiteral, "1"),
        Ast(eAst::kDoubleLiteral, "1.1"),
        Ast(eAst::kStringLiteral, "'string literal'"),
        Ast(eAst::kStringLiteral, "'\\\''"),
        Ast(eAst::kIdentifier, "alnumus"),
        Ast(eAst::kByteLiteral, "1c"),
        Ast(eAst::kBoolLiteral, "1b"),
        Ast(eAst::kUnsignedLiteral, "1u"),
        Ast(eAst::kNoneLiteral, "none"),
    };
    // Ast(eAst::kTrueLiteral, "true"),
    // Ast(eAst::kFalseLiteral, "false")};

    auto num_lit = LarkParser::ParseOperand({source.cbegin(), source.cend()});
    if (num_lit.Valid()) {
      std::cout << "Parsed number literal: " << ToStr(num_lit.Value().Type())
                << std::endl;
      EXPECT_EQ(num_lit.Value().Type(), expected_result[0].Type());
      EXPECT_EQ(num_lit.Value().Literal(), expected_result[0].Literal());
    } else {
      std::cout << "Error parsing number literal: " << num_lit.Error()
                << std::endl;
    }

    auto real_lit = LarkParser::ParseOperand(num_lit.Always());
    if (real_lit.Valid()) {
      std::cout << "Parsed real literal: " << ToStr(real_lit.Value().Type())
                << std::endl;
      EXPECT_EQ(real_lit.Value().Type(), expected_result[1].Type());
      EXPECT_EQ(real_lit.Value().Literal(), expected_result[1].Literal());
    } else {
      std::cout << "Error parsing real literal: " << real_lit.Error()
                << std::endl;
    }

    auto str_lit = LarkParser::ParseOperand(real_lit.Always());
    if (str_lit.Valid()) {
      std::cout << "Parsed string literal: " << ToStr(str_lit.Value().Type())
                << std::endl;
      EXPECT_EQ(str_lit.Value().Type(), expected_result[2].Type());
      EXPECT_EQ(str_lit.Value().Literal(), expected_result[2].Literal());
    } else {
      std::cout << "Error parsing string literal: " << str_lit.Error()
                << std::endl;
    }

    auto str_lit2 = LarkParser::ParseOperand(str_lit.Always());
    if (str_lit2.Valid()) {
      std::cout << "Parsed string literal: " << ToStr(str_lit2.Value().Type())
                << std::endl;
      EXPECT_EQ(str_lit2.Value().Type(), expected_result[3].Type());
      EXPECT_EQ(str_lit2.Value().Literal(), expected_result[3].Literal());
    } else {
      std::cout << "Error parsing string literal: " << str_lit2.Error()
                << std::endl;
    }

    auto alnumus_lit = LarkParser::ParseOperand(str_lit2.Always());
    if (alnumus_lit.Valid()) {
      std::cout << "Parsed alnumus literal: "
                << ToStr(alnumus_lit.Value().Type()) << std::endl;
      EXPECT_EQ(alnumus_lit.Value().Type(), expected_result[4].Type());
      EXPECT_EQ(alnumus_lit.Value().Literal(), expected_result[4].Literal());
    } else {
      std::cout << "Error parsing alnumus literal: " << alnumus_lit.Error()
                << std::endl;
    }

    auto byte_lit = LarkParser::ParseOperand(alnumus_lit.Always());

    if (byte_lit.Valid()) {
      std::cout << "Parsed byte literal: " << ToStr(byte_lit.Value().Type())
                << std::endl;
      EXPECT_EQ(byte_lit.Value().Type(), expected_result[5].Type());
      EXPECT_EQ(byte_lit.Value().Literal(), expected_result[5].Literal());
    } else {
      std::cout << "Error parsing byte literal: " << byte_lit.Error()
                << std::endl;
    }

    auto bit_lit = LarkParser::ParseOperand(byte_lit.Always());

    if (bit_lit.Valid()) {
      std::cout << "Parsed bit literal: " << ToStr(bit_lit.Value().Type())
                << std::endl;
      EXPECT_EQ(bit_lit.Value().Type(), expected_result[6].Type());
      EXPECT_EQ(bit_lit.Value().Literal(), expected_result[6].Literal());
    } else {
      std::cout << "Error parsing bit literal: " << bit_lit.Error()
                << std::endl;
    }

    auto unsigned_lit = LarkParser::ParseOperand(bit_lit.Always());

    if (unsigned_lit.Valid()) {
      std::cout << "Parsed unsigned literal: "
                << ToStr(unsigned_lit.Value().Type()) << std::endl;
      EXPECT_EQ(unsigned_lit.Value().Type(), expected_result[7].Type());
      EXPECT_EQ(unsigned_lit.Value().Literal(), expected_result[7].Literal());
    } else {
      std::cout << "Error parsing unsigned literal: " << unsigned_lit.Error()
                << std::endl;
    }

    auto none_lit = LarkParser::ParseOperand(unsigned_lit.Always());
    if (none_lit.Valid()) {
      std::cout << "Parsed none literal: " << ToStr(none_lit.Value().Type())
                << std::endl;
      EXPECT_EQ(none_lit.Value().Type(), expected_result[8].Type());
      EXPECT_EQ(none_lit.Value().Literal(), expected_result[8].Literal());
    } else {
      std::cout << "Error parsing none literal: " << none_lit.Error()
                << std::endl;
    }

    // auto true_lit = LarkParser::ParseOperand({source.cbegin(),
    // source.cend()});

    // if (true_lit.Valid()) {
    //   std::cout << "Parsed true literal: " << ToStr(true_lit.Value().Type())
    //             << std::endl;
    //   EXPECT_EQ(true_lit.Value().Type(), expected_result[9].Type());
    //   EXPECT_EQ(true_lit.Value().Literal(), expected_result[9].Literal());
    // } else {
    //   std::cout << "Error parsing true literal: " << true_lit.Error()
    //             << std::endl;
    // }
  }
}
END_MINITEST;
#endif

#if CAOCO_TEST_PARSER_BASICS_ValueExpr
MINITEST(TestParserBasics, TestCaseValueExpr) {
  // Test a parsing functor given a subset of tokens. Prints the test_name
  // followed by the AST.
  lambda test_and_compare_primary_parser = [](const std::string& test_name,
                                              const Ast& expected_ast,
                                              const std::string& code) -> bool {
    auto expected_source = Lexer::Lex(code);
    EXPECT_TRUE(expected_source.Valid());
    if (!expected_source.Valid()) {
      std::cout << "Lexing Error:" << expected_source.Error();
      return false;
    } else {
      auto source = expected_source.Extract();
      auto parse_result = LarkParser::RParseValueExpression().Parse(
          {source.cbegin(), source.cend()});
      std::cout << "[Testing Parsing Method][Test Case:" << test_name << "]"
                << std::endl;
      if (parse_result.Valid()) {
        Ast ast = parse_result.Extract();
        // PrintAst(ast);
        bool compared = CompareAst(ast, expected_ast);
        EXPECT_TRUE(compared);
        if (compared) {
          return true;
        } else {
          return false;
        }
      } else {
        std::cout << "Error parsing " << test_name << ": "
                  << parse_result.Error() << std::endl;
        return false;
      }
    }
  };

  test_and_compare_primary_parser("Single Number",
                                  Ast(eAst::kNumberLiteral, "1"), "1");
  test_and_compare_primary_parser("Single Real",
                                  Ast(eAst::kDoubleLiteral, "1.1"), "1.1");
  test_and_compare_primary_parser("Single String",
                                  Ast(eAst::kStringLiteral, "'string literal'"),
                                  "'string literal'");
  test_and_compare_primary_parser(
      "Single String Escaped", Ast(eAst::kStringLiteral, "'\\\''"), "'\\\''");
  test_and_compare_primary_parser("Single Identifier",
                                  Ast(eAst::kIdentifier, "alnumus"), "alnumus");
  test_and_compare_primary_parser("Single Byte", Ast(eAst::kByteLiteral, "1c"),
                                  "1c");
  test_and_compare_primary_parser("Single Bit", Ast(eAst::kBoolLiteral, "1b"),
                                  "1b");
  test_and_compare_primary_parser("Single Unsigned",
                                  Ast(eAst::kUnsignedLiteral, "1u"), "1u");
  test_and_compare_primary_parser("Single None Literal",
                                  Ast(eAst::kNoneLiteral, "none"), "none");
  //// test_and_compare_primary_parser("Single True Literal",
  //// Ast(eAst::kTrueLiteral, "true"), "true");
  //// test_and_compare_primary_parser("Single False Literal",
  //// Ast(eAst::kFalseLiteral, "false"), "false");

  test_and_compare_primary_parser("Operand in Parenthesis",
                                  Ast(eAst::kNumberLiteral, "1"), "(1)");

  test_and_compare_primary_parser(
      "Binary Addition",
      Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
          Ast(eAst::kNumberLiteral, "1")),
      "1 + 1");

  test_and_compare_primary_parser(
      "Binary Diff Priority",
      Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
          Ast(eAst::kMultiplication, "*", Ast(eAst::kNumberLiteral, "2"),
              Ast(eAst::kNumberLiteral, "3"))),
      "1 + 2 * 3");

  test_and_compare_primary_parser(
      "Binary Left Associative",
      Ast(eAst::kSubtraction, "-",
          Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
              Ast(eAst::kNumberLiteral, "2")),
          Ast(eAst::kNumberLiteral, "3")),
      "1 + 2 - 3");

  test_and_compare_primary_parser(
      "Binary Right Associative . member acces operator.",
      Ast(eAst::kPeriod, ".",
          Ast(eAst::kPeriod, ".",
              Ast(eAst::kPeriod, ".", Ast(eAst::kIdentifier, "a"),
                  Ast(eAst::kIdentifier, "b")),
              Ast(eAst::kIdentifier, "c")),
          Ast(eAst::kIdentifier, "d")),
      "a.b.c.d");

  test_and_compare_primary_parser(
      "Unary Postfix", Ast(eAst::kIncrement, "++", Ast(eAst::kIdentifier, "a")),
      "a++");

  test_and_compare_primary_parser(
      "Parenthesis is resolved first (1+2)*3",
      Ast(eAst::kMultiplication, "*",
          Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
              Ast(eAst::kNumberLiteral, "2")),
          Ast(eAst::kNumberLiteral, "3")),
      "(1+2)*3");

  test_and_compare_primary_parser(
      "Unary Prefix", Ast(eAst::kNegation, "!", Ast(eAst::kIdentifier, "a")),
      "!a");

  test_and_compare_primary_parser(
      "Unary Prefix Repeated",
      Ast(eAst::kNegation, "!",
          Ast(eAst::kNegation, "!", Ast(eAst::kIdentifier, "a"))),
      "!!a");

  test_and_compare_primary_parser(
      "Assingment is right associative",
      Ast(eAst::kSimpleAssignment, "=", Ast(eAst::kIdentifier, "a"),
          Ast(eAst::kSimpleAssignment, "=", Ast(eAst::kIdentifier, "b"),
              Ast(eAst::kIdentifier, "c"))),
      "a=b=c");

  test_and_compare_primary_parser(
      "Prefix Unary then Binary",
      Ast(eAst::kAddition, "+",
          Ast(eAst::kNegation, "!", Ast(eAst::kNumberLiteral, "1")),
          Ast(eAst::kNumberLiteral, "2")),
      "!1+2");

  test_and_compare_primary_parser(
      "Prefix Unary then Higher Priority Member Access",
      Ast(eAst::kNegation, "!",
          Ast(eAst::kPeriod, ".", Ast(eAst::kIdentifier, "a"),
              Ast(eAst::kIdentifier, "b"))),
      "!a.b");

  test_and_compare_primary_parser(
      "Prefix Unary After Binary 1+!2",
      Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
          Ast(eAst::kNegation, "!", Ast(eAst::kNumberLiteral, "2"))),
      "1+!2");

  test_and_compare_primary_parser(
      "Empty Function Call",
      Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "a"),
          Ast(eAst::kArguments, "")),
      "a()");

  test_and_compare_primary_parser(
      "Prefix Unary then Function Call",
      Ast(eAst::kNegation, "!",
          Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "a"),
              Ast(eAst::kArguments, ""))),
      "!a()");

  test_and_compare_primary_parser(
      "Repeated Prefix and Repeated Function Call",
      Ast(eAst::kNegation, "!",
          Ast(eAst::kNegation, "!",
              Ast(eAst::kFunctionCall, "",
                  Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "a"),
                      Ast(eAst::kArguments, "")),
                  Ast(eAst::kArguments, "")))),
      "!!a()()");

  test_and_compare_primary_parser(
      "Function Call then Binary",
      Ast(eAst::kAddition, "+",
          Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "a"),
              Ast(eAst::kArguments, "")),
          Ast(eAst::kNumberLiteral, "2")),
      "a()+2");

  test_and_compare_primary_parser(
      "Function Call then Member Access",
      Ast(eAst::kPeriod, ".",
          Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "a"),
              Ast(eAst::kArguments, "")),
          Ast(eAst::kIdentifier, "b")),
      "a().b");

  test_and_compare_primary_parser(
      "Member Access then Function Call",
      Ast(eAst::kFunctionCall, "",
          Ast(eAst::kPeriod, ".", Ast(eAst::kIdentifier, "a"),
              Ast(eAst::kIdentifier, "b")),
          Ast(eAst::kArguments, "")),
      "a.b()");

  test_and_compare_primary_parser(
      "Binary Then Function Call 1 + a()",
      Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
          Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "a"),
              Ast(eAst::kArguments, ""))),
      "1+a()");

  // a.b().c is parsed as (a.b()).c and not as a.(b().c) because of
  // left-to-right
  // associativity of member access. source: cppreference.com
  test_and_compare_primary_parser(
      "Repeated Member Access with internal Function Call",
      Ast(eAst::kPeriod, ".",
          Ast(eAst::kPeriod, ".",
              Ast(eAst::kFunctionCall, "",
                  Ast(eAst::kPeriod, ".", Ast(eAst::kIdentifier, "a"),
                      Ast(eAst::kIdentifier, "b")),
                  Ast(eAst::kArguments, "")),
              Ast(eAst::kIdentifier, "c")),
          Ast(eAst::kIdentifier, "d")),
      "a.b().c.d");

  test_and_compare_primary_parser(
      "Complex Expression: foo.bar()*1+1",
      Ast(eAst::kAddition, "+",
          Ast(eAst::kMultiplication, "*",
              Ast(eAst::kFunctionCall, "",
                  Ast(eAst::kPeriod, ".", Ast(eAst::kIdentifier, "foo"),
                      Ast(eAst::kIdentifier, "bar")),
                  Ast(eAst::kArguments, "")),
              Ast(eAst::kNumberLiteral, "1")),
          Ast(eAst::kNumberLiteral, "1")),
      "foo.bar()*1+1");

  test_and_compare_primary_parser(
      "Complex Expression: (foo.bar() + 1) * 1",
      Ast(eAst::kMultiplication, "*",
          Ast(eAst::kAddition, "+",
              Ast(eAst::kFunctionCall, "",
                  Ast(eAst::kPeriod, ".", Ast(eAst::kIdentifier, "foo"),
                      Ast(eAst::kIdentifier, "bar")),
                  Ast(eAst::kArguments, "")),
              Ast(eAst::kNumberLiteral, "1")),
          Ast(eAst::kNumberLiteral, "1")),
      "(foo.bar() + 1) * 1");

  test_and_compare_primary_parser(
      "Assingment Experssion : foo = 1 + 2",
      Ast(eAst::kSimpleAssignment, "=", Ast(eAst::kIdentifier, "foo"),
          Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
              Ast(eAst::kNumberLiteral, "2"))),
      "foo = 1 + 2");

  test_and_compare_primary_parser(
      "Function Call After Binary Left Associative:  e + d + c + b()",
      Ast(eAst::kAddition, "+",
          Ast(eAst::kAddition, "+",
              Ast(eAst::kAddition, "+", Ast(eAst::kIdentifier, "e"),
                  Ast(eAst::kIdentifier, "d")),
              Ast(eAst::kIdentifier, "c")),
          Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "b"),
              Ast(eAst::kArguments, ""))),
      "e + d + c + b()");

  test_and_compare_primary_parser(
      "Function Call With Arguments",
      Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "foo"),
          Ast(eAst::kArguments, "", Ast(eAst::kNumberLiteral, "1"),
              Ast(eAst::kNumberLiteral, "2"), Ast(eAst::kNumberLiteral, "3"))),
      "foo(1, 2, 3)");

  test_and_compare_primary_parser(
      "Indexing",
      Ast(eAst::kIndexOperator, "", Ast(eAst::kIdentifier, "foo"),
          Ast(eAst::kArguments, "", Ast(eAst::kNumberLiteral, "1"),
              Ast(eAst::kNumberLiteral, "2"), Ast(eAst::kNumberLiteral, "3"))),
      "foo[1, 2, 3]");

  test_and_compare_primary_parser(
      "Listing Operator foo{1,2,3}",
      Ast(eAst::kListingOperator, "", Ast(eAst::kIdentifier, "foo"),
          Ast(eAst::kArguments, "", Ast(eAst::kNumberLiteral, "1"),
              Ast(eAst::kNumberLiteral, "2"), Ast(eAst::kNumberLiteral, "3"))),
      "foo{1,2,3}");

  test_and_compare_primary_parser(
      "Access Operator foo::bar",
      Ast(eAst::kDoubleColon, "::", Ast(eAst::kIdentifier, "foo"),
          Ast(eAst::kIdentifier, "bar")),
      "foo::bar");

  test_and_compare_primary_parser("Unary Minus",
                                  Ast(eAst::kNumberLiteral, "-1"), "-1");

test_and_compare_primary_parser("Unary Minus in expression",
                               Ast(eAst::kAddition, "+",
                                   Ast(eAst::kNumberLiteral, "-1"),
                                   Ast(eAst::kNumberLiteral, "2")),
                               "-1+2");

test_and_compare_primary_parser("Unary Minus in expression -1 - -1;",
Ast(eAst::kSubtraction, "-",
                                    Ast(eAst::kNumberLiteral, "-1"),
                                    Ast(eAst::kNumberLiteral, "-1")),
                                "-1 - -1");

}
END_MINITEST;
#endif

#if CAOCO_TEST_PARSER_BASICS_ExprExtensive
MINITEST(TestParserBasics, TestCaseValueExprExtensive) {
  // Test a parsing functor given a subset of tokens. Prints the test_name
  // followed by the AST.
  lambda test_and_print_primary_parser_statements =
      [](const std::string& test_name, const std::string& code) -> bool {
    auto expected_source = Lexer::Lex(code);
    EXPECT_TRUE(expected_source.Valid());
    if (!expected_source.Valid()) {
      std::cout << "Lexing Error:" << expected_source.Error();
      return false;
    } else {
      auto source = expected_source.Extract();
      TkVectorConstIter last_parsed = source.cbegin();
      while (last_parsed != source.cend()) {
        TkScope statement_scope =
            TkScope::find_program_statement(last_parsed, source.cend());
        if (!statement_scope.Valid()) {
          std::cout << "Error parsing statement: " << statement_scope.Error()
                    << std::endl;
          return false;
        } else {
          last_parsed = statement_scope.End();
          auto parse_result = LarkParser::RParseValueExpression().Parse(
              {statement_scope.Begin(), statement_scope.ContainedEnd()});
          if (parse_result.Valid()) {
            Ast ast = parse_result.Extract();
            // Check for any nodes that dont belong in the primary parser.
            bool has_open_paren = AstContains(ast, eAst::kOpenParen, "(");
            bool has_close_paren = AstContains(ast, eAst::kCloseParen, ")");
            bool has_open_brace = AstContains(ast, eAst::kOpenBrace, "{");
            bool has_close_brace = AstContains(ast, eAst::kCloseBrace, "}");
            bool has_open_bracket = AstContains(ast, eAst::kOpenBracket, "[");
            bool has_close_bracket = AstContains(ast, eAst::kCloseBracket, "]");
            if (has_open_paren || has_close_paren || has_open_brace ||
                has_close_brace || has_open_bracket || has_close_bracket) {
              std::cout << "Invalid parsing result " << test_name
                        << ": The result contains parenthesis, braces or "
                           "brackets."
                        << std::endl;
              PrintAst(ast);
            }
            EXPECT_FALSE(has_open_paren);
            EXPECT_FALSE(has_close_paren);
            EXPECT_FALSE(has_open_brace);
            EXPECT_FALSE(has_close_brace);
            EXPECT_FALSE(has_open_bracket);
            EXPECT_FALSE(has_close_bracket);
          } else {
            std::cout << "Error parsing " << test_name << ": "
                      << parse_result.Error() << std::endl;
            return false;
          }
        }
      }
      return true;
    }
  };

  // This test is just to test the testing function and TkScope::find_statement
  // function.
  test_and_print_primary_parser_statements("Single Operands",
                                           "1;1.1;'string';"
                                           "'\\'';alnumus;1c;1b;1u;none;");
  test_and_print_primary_parser_statements(
      "Postfix And Prefix In Binary Expression",
      "a + a + a + a; a + a + a + a(); a + a + a + !a; a + a() + a + a; a + a() + a + a();\
a + a() + a + !a; a + !a + a + a; a + !a + a + a(); a + !a + a + !a; a() + a + a + a; a() + a + a + a(); a() + a + a + !a;\
a() + a() + a + a; a() + a() + a + a(); a() + a() + a + !a; a() + !a + a + a; a() + !a + a + a(); a() + !a + a + !a; !a + a + a + a;\
!a + a + a + a(); !a + a + a + !a; !a + a() + a + a; !a + a() + a + a(); !a + a() + a + !a; !a + !a + a + a; !a + !a + a + a();\
!a + !a + a + !a; a + a + a() + a; a + a + a() + a(); a + a + a() + !a; a + a() + a() + a; a + a() + a() + a(); a + a() + a() + !a;\
a + !a + a() + a; a + !a + a() + a(); a + !a + a() + !a; a() + a + a() + a; a() + a + a() + a(); a() + a + a() + !a; a() + a() + a()\
+ a; a() + a() + a() + a(); a() + a() + a() + !a; a() + !a + a() + a; a() + !a + a() + a(); a() + !a + a() + !a; !a + a + a() + a; !a\
+ a + a() + a(); !a + a + a() + !a; !a + a() + a() + a; !a + a() + a() + a(); !a + a() + a() + !a; !a + !a + a() + a; !a + !a + a() + \
a(); !a + !a + a() + !a; a + a + !a + a; a + a + !a + a(); a + a + !a + !a; a + a() + !a + a; a + a() + !a + a(); a + a() + !a + !a; a \
+ !a + !a + a; a + !a + !a + a(); a + !a + !a + !a; a() + a + !a + a; a() + a + !a + a(); a() + a + !a + !a; a() + a() + !a + a; a() + \
a() + !a + a(); a() + a() + !a + !a; a() + !a + !a + a; a() + !a + !a + a(); a() + !a + !a + !a; !a + a + !a + a; !a + a + !a + a(); !a\
+ a + !a + !a; !a + a() + !a + a; !a + a() + !a + a(); !a + a() + !a + !a; !a + !a + !a + a; !a + !a + !a + a(); !a + !a + !a + !a;");

  test_and_print_primary_parser_statements(
      "Postfix And Prefix In Member Access",
      "a . a . a . a; a . a . a . a(); a . a . a() . a; a . a . a() . a(); a . a() . a . a; a . a() . a . a();\
 a . a() . a() . a; a . a() . a() . a(); a() . a . a . a; a() . a . a . a(); a() . a . a() . a; a() . a . a() . a(); a() . a() . a . a; a() .\
 a() . a . a(); a() . a() . a() . a; a() . a() . a() . a(); !a . a . a . a; !a . a . a . a(); !a . a . a() . a; !a . a . a() . a(); !a . a() .\
 a . a; !a . a() . a . a(); !a . a() . a() . a; !a . a() . a() . a(); !a() . a . a . a; !a() . a . a . a(); !a() . a . a() . a; !a() . a . a()\
 . a(); !a() . a() . a . a; !a() . a() . a . a(); !a() . a() . a() . a; !a() . a() . a() . a();");

  test_and_print_primary_parser_statements(
      "Random Weird but Valid Expressions",
      "!!!!!!(foo).bar(aa) + !!1[]() * !!a.google{ 1 } ++ ++ ++ ++ + !!a()();");
}
END_MINITEST;
#endif

#if CAOCO_TEST_PARSER_BASICS_Statements

#define PARSER_TEST_CASE(sourcetoparse, parsefunc, casename)             \
  if (true) {                                                            \
    auto expected_source = Lexer::Lex(sourcetoparse);                    \
    EXPECT_TRUE(expected_source.Valid());                                \
    if (!expected_source.Valid()) {                                      \
      std::cout << "Lexing Error:" << expected_source.Error();           \
    } else {                                                             \
      auto source = expected_source.Extract();                           \
      auto parse_result =                                                \
          LarkParser::##parsefunc({source.cbegin(), source.cend()});     \
      std::cout << "[Testing Parsing Method]" << #casename << std::endl; \
      EXPECT_TRUE(parse_result.Valid());                                 \
      if (!parse_result.Valid()) {                                       \
        std::cout << "Error parsing " << #casename << ": "               \
                  << parse_result.Error() << std::endl;                  \
      }                                                                  \
    }                                                                    \
  }

MINITEST(TestParserBasics, TestCasePrimaryStatement) {
  // test a primary expr statement starting with a subexpression.
  auto expected_source = Lexer::Lex("(1+2)*a;");
  EXPECT_TRUE(expected_source.Valid());
  if (!expected_source.Valid()) {
    std::cout << "Lexing Error:" << expected_source.Error();
  } else {
    auto source = expected_source.Extract();
    auto parse_result =
        LarkParser::ParsePrimaryStatement({source.cbegin(), source.cend()});
    std::cout << "[Testing Parsing Method][Test Case:Primary Statement]"
              << std::endl;
    EXPECT_TRUE(parse_result.Valid());
    if (parse_result.Valid()) {
      PrintAst(parse_result.Value());
    }
  }
}
END_MINITEST;

// Declarations

MINITEST(TestParserBasics, TestCaseVariableDeclarationNoTypeNoAssignNoMod) {
  auto expected_source = Lexer::Lex("def@Foo;");
  EXPECT_TRUE(expected_source.Valid());
  if (!expected_source.Valid()) {
    std::cout << "Lexing Error:" << expected_source.Error();
  } else {
    auto source = expected_source.Extract();
    auto parse_result =
        LarkParser::ParseVariableDecl({source.cbegin(), source.cend()});
    std::cout << "[Testing Parsing Method][Test "
                 "Case:VariableDeclarationNoTypeNoAssignNoMod]"
              << std::endl;
    EXPECT_TRUE(parse_result.Valid());
    if (parse_result.Valid()) {
      PrintAst(parse_result.Value());
    }
  }
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseVariableDeclarationNoTypeNoAssign) {
  auto expected_source = Lexer::Lex("const def@Foo;");
  EXPECT_TRUE(expected_source.Valid());
  if (!expected_source.Valid()) {
    std::cout << "Lexing Error:" << expected_source.Error();
  } else {
    auto source = expected_source.Extract();
    auto parse_result =
        LarkParser::ParseVariableDecl({source.cbegin(), source.cend()});
    std::cout << "[Testing Parsing Method][Test "
                 "Case:VariableDeclarationNoTypeNoAssign]"
              << std::endl;
    EXPECT_TRUE(parse_result.Valid());
    if (parse_result.Valid()) {
      PrintAst(parse_result.Value());
    }
  }
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseVariableDeclarationNoAssign) {
  auto expected_source = Lexer::Lex("const def str@Foo;");
  EXPECT_TRUE(expected_source.Valid());
  if (!expected_source.Valid()) {
    std::cout << "Lexing Error:" << expected_source.Error();
  } else {
    auto source = expected_source.Extract();
    auto parse_result =
        LarkParser::ParseVariableDecl({source.cbegin(), source.cend()});
    std::cout
        << "[Testing Parsing Method][Test Case:VariableDeclarationNoAssign]"
        << std::endl;
    EXPECT_TRUE(parse_result.Valid());
    if (parse_result.Valid()) {
      PrintAst(parse_result.Value());
    }
  }
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseVariableDefinition) {
  PARSER_TEST_CASE("const def str@Foo: 42;", ParseVariableDecl,
                   VariableDefinition);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseTypeAlias) {
  PARSER_TEST_CASE("use @MyInteger: int;", ParseUsingDecl, TypeAlias);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseLibraryNamespaceInclusion) {
  PARSER_TEST_CASE("use lib my_math_lib;", ParseUsingDecl,
                   LibraryNamespaceInclusion);
}
END_MINITEST;
MINITEST(TestParserBasics, TestCaseNamespaceInclusion) {
  PARSER_TEST_CASE("use namespace my_ns;", ParseUsingDecl, NamespaceInclusion);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseObjectInclusion) {
  PARSER_TEST_CASE("use my_ns::Foo;", ParseUsingDecl, ObjectInclusion);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseTypeInclusion) {
  PARSER_TEST_CASE("use@MyFooType: my_ns::Foo;", ParseUsingDecl, TypeInclusion);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseObjectInclusionFromLibrary) {
  PARSER_TEST_CASE("use lib my_math_lib::add;", ParseUsingDecl,
                   ObjectInclusionFromLibrary);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseTypeInclusionFromLibrary) {
  PARSER_TEST_CASE("use @MyAddMethodImpl: lib my_math_lib::add;",
                   ParseUsingDecl, TypeInclusionFromLibrary);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseTypeImportDeclaration) {
  PARSER_TEST_CASE("import foo;", ParseImportDecl, ImportDeclaration);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseMethodDeclImplicitVoidArgNoRet) {
  PARSER_TEST_CASE("fn@add;", ParseMethodDecl, MethodDeclImplicitVoidArgNoRet);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseMethodDeclImplicitVoidArgNoRet2) {
  PARSER_TEST_CASE("fn@add();", ParseMethodDecl,
                   MethodDeclImplicitVoidArgNoRet);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseMethodDeclImplicitVoidArgAnyRet) {
  PARSER_TEST_CASE("fn@add>;", ParseMethodDecl,
                   TestCaseMethodDeclImplicitVoidArgAnyRet);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseMethodDeclImplicitVoidArgAnyRet2) {
  PARSER_TEST_CASE("fn@add()>;", ParseMethodDecl,
                   TestCaseMethodDeclImplicitVoidArgAnyRet2);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseMethodDeclArgNoRet) {
  PARSER_TEST_CASE("fn@add(a,b);", ParseMethodDecl, TestCaseMethodDeclArgNoRet);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseMethodDeclArgAnyRet) {
  PARSER_TEST_CASE("fn@add(a,b)>;", ParseMethodDecl,
                   TestCaseMethodDeclArgAnyRet);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseMethodDeclArgIdentifiedAnyRet) {
  PARSER_TEST_CASE("fn@add(@a,@b)>;", ParseMethodDecl,
                   TestCaseMethodDeclArgAnyRet);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseMethodDeclArgsTypedRet) {
  PARSER_TEST_CASE("fn@add(@a,@b)>int;", ParseMethodDecl,
                   TestCaseMethodDeclArgsTypedRet);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseMethodDeclTypedArgsTypedRet) {
  PARSER_TEST_CASE("fn@add(int @a,int @b)>int;", ParseMethodDecl,
                   TestCaseMethodDeclTypedArgsTypedRet);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseMethodDeclTypedArgsTypedRetWithModifiers) {
  PARSER_TEST_CASE("fn@add(const int @a,const int @b)>const int;",
                   ParseMethodDecl, TestCaseMethodDeclTypedArgsTypedRet);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseClassDecl) {
  PARSER_TEST_CASE("class@Husky;", ParseClassDecl,
                   TestCaseMethodDeclTypedArgsTypedRet);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseClassDeclWithMod) {
  PARSER_TEST_CASE("const static class@Husky;", ParseClassDecl,
                   TestCaseClassDeclWithMod);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseLibWithMod) {
  PARSER_TEST_CASE("const static lib@MathLib;", ParseLibDecl,
                   TestCaseCLibDeclWithMod);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseLibWithModAndDefinition) {
  PARSER_TEST_CASE(
      "const static lib@MathLib:{const def str@Foo: 42;use @MyInteger: int;};",
      ParseLibDecl, TestCaseCLibDeclWithMod);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseClassWithModAndDefinition) {
  PARSER_TEST_CASE(
      "const static class@Husky:{const def str@Foo: 42;use @MyInteger: int;};",
      ParseClassDecl, TestCaseClassDeclWithMod);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseMethodDefinition) {
  PARSER_TEST_CASE("fn@add(const int @a,const int @b)>const int:{a+b;};",
                   ParseMethodDecl, TestCaseMethodDefinition);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseMainDefinition) {
  PARSER_TEST_CASE("main(a,b):{a+b;};", ParseMainDecl, TestCaseMainDefinition);
}
END_MINITEST;

// Pragmatic Statements: appears at top level, or in a library.
MINITEST(TestParserBasics, TestCasePragmaticDeclarations) {
  PARSER_TEST_CASE("const def str@Foo: 42;", ParsePragmaticStmt,
                   VariableDefinition);
  PARSER_TEST_CASE("use @MyInteger: int;", ParsePragmaticStmt, TypeAlias);
  PARSER_TEST_CASE("use lib my_math_lib;", ParsePragmaticStmt,
                   LibraryNamespaceInclusion);
  PARSER_TEST_CASE("use namespace my_ns;", ParsePragmaticStmt,
                   NamespaceInclusion);
  PARSER_TEST_CASE("use my_ns::Foo;", ParsePragmaticStmt, ObjectInclusion);
  PARSER_TEST_CASE("use@MyFooType: my_ns::Foo;", ParsePragmaticStmt,
                   TypeInclusion);
  PARSER_TEST_CASE("use lib my_math_lib::add;", ParsePragmaticStmt,
                   ObjectInclusionFromLibrary);
  PARSER_TEST_CASE("use @MyAddMethodImpl: lib my_math_lib::add;",
                   ParsePragmaticStmt, TypeInclusionFromLibrary);

  PARSER_TEST_CASE("const static lib@MathLib;", ParsePragmaticStmt,
                   PragmaticDeclLibrary);
  PARSER_TEST_CASE("import foo;", ParsePragmaticStmt, ImportDeclaration);
  PARSER_TEST_CASE("fn@add;", ParsePragmaticStmt,
                   MethodDeclImplicitVoidArgNoRet);
  PARSER_TEST_CASE("fn@add();", ParsePragmaticStmt,
                   MethodDeclImplicitVoidArgNoRet);
  PARSER_TEST_CASE("fn@add>;", ParsePragmaticStmt,
                   TestCaseMethodDeclImplicitVoidArgAnyRet);
  PARSER_TEST_CASE("fn@add()>;", ParsePragmaticStmt,
                   TestCaseMethodDeclImplicitVoidArgAnyRet2);
  PARSER_TEST_CASE("fn@add(a,b)>;", ParsePragmaticStmt,
                   TestCaseMethodDeclArgAnyRet);
  PARSER_TEST_CASE("fn@add(@a,@b)>int;", ParsePragmaticStmt,
                   TestCaseMethodDeclArgsTypedRet);
  PARSER_TEST_CASE("fn@add(int @a,int @b)>int;", ParsePragmaticStmt,
                   TestCaseMethodDeclTypedArgsTypedRet);
  PARSER_TEST_CASE("fn@add(const int @a,const int @b)>const int;",
                   ParsePragmaticStmt, TestCaseMethodDeclTypedArgsTypedRet);
  PARSER_TEST_CASE("class@Husky;", ParsePragmaticStmt,
                   TestCaseMethodDeclTypedArgsTypedRet);

  PARSER_TEST_CASE("const static class@Husky;", ParsePragmaticStmt,
                   TestCaseClassDeclWithMod);
  PARSER_TEST_CASE("const static lib@MathLib;", ParsePragmaticStmt,
                   TestCaseCLibDeclWithMod);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseParseProgramWithDeclrations) {
  PARSER_TEST_CASE(
      "import foo;"
      "const static lib@MathLib;"
      "use @MyAddMethodImpl: lib MathLib::add;"
      "const def str@Foo: 42;"
      "fn@add(const int @a,const int @b)>const int;"
      "const static class @Husky;",
      ParseProgram, TestCaseParseProgramWithDeclrations);
}
END_MINITEST;

// Control Flow Statements
MINITEST(TestParserBasics, TestCaseReturnStatement) {
  PARSER_TEST_CASE("return a + b;", ParseReturnStmt, TestCaseReturnStatement);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseIfStatement) {
  PARSER_TEST_CASE("if(a){ a + b; };", ParseIfDecl, TestCaseReturnStatement);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseIfElseStatement) {
  PARSER_TEST_CASE("if(a){ a + b; }else{a;};", ParseIfDecl,
                   TestCaseReturnStatement);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseIfElifStatement) {
  PARSER_TEST_CASE("if(a){ a + b; }elif(b){a;};", ParseIfDecl,
                   TestCaseReturnStatement);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseIfElifElseStatement) {
  PARSER_TEST_CASE("if(a){ a + b; }elif(b){a;}else{b;};", ParseIfDecl,
                   TestCaseReturnStatement);
}
END_MINITEST;

// Loop Statements
MINITEST(TestParserBasics, TestCaseWhileStatement) {
  PARSER_TEST_CASE("while(a){ a + b; };", ParseWhileDecl,
                   TestCaseReturnStatement);
}
END_MINITEST;

MINITEST(TestParserBasics, TestCaseForStatement) {
  PARSER_TEST_CASE("for(def@a:0;a!=end;a++){ a + b; };", ParseForDecl,
                   TestCaseReturnStatement);
}
END_MINITEST;

// Animals Example Program.
MINITEST(TestParserBasics, TestCaseAnimalsExampleProgram) {
  PARSER_TEST_CASE(
      "main(): {class @Horse : {fn @makeSound() : {return 'Neigh!';};};class "
      "@Cow "
      ": {  fn @makeSound() : { return 'Moo!'; };}"
      ";class @Wolf : {fn @makeSound() : { return 'Oooo!'; };};class @Cricket "
      ": "
      "{fn @makeSound() : { return 'Chirp!'; };};"
      "def @farm_animals : list{Horse(), Cow()};def @all_animals : "
      "farm_animals + list{Wolf(), Cricket()};"
      "fn @makeAnimalSounds(list @animal_list) : {def str @sounds;for (def "
      "@idx : 0; idx < animal_list.Size();"
      "idx++) {sounds += animal_list[idx].makeSound();};return sounds;};return "
      "makeAnimalSounds(all_animals);};",
      ParseProgram, TestCaseAnimalsExampleProgram);
}
END_MINITEST;

#endif

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: unit_tests
// File: ut0_parser_basics.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_UT0_PARSER_BASICS_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
