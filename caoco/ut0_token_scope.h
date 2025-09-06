//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: unit_tests
// File: token_scope.h
//---------------------------------------------------------------------------//
// Brief: {description}
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_UNIT_TESTS_UT0_TOKEN_SCOPE_H
#define HEADER_GUARD_CAOCO_UNIT_TESTS_UT0_TOKEN_SCOPE_H
// Includes:
#include "lexer.h"           // Lex strings into tokens
#include "minitest_flags.h"  // Flags to enable or disable the unit tests
#include "minitest_pch.h"    // All pre includes for each unit test
#include "minitest_util.h"   // Utility methods shared among the all unit tests
#include "token_scope.h"
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

#define MINITEST_TEST_UT0_TOKEN_SCOPE true
#if MINITEST_TEST_UT0_TOKEN_SCOPE
#define MINITEST_TEST_UT0_TOKEN_SCOPE_ParenScopeFinder 1
#define MINITEST_TEST_UT0_TOKEN_SCOPE_ListScopeFinder 1
#define MINITEST_TEST_UT0_TOKEN_SCOPE_FrameScopeFinder 1
#define MINITEST_TEST_UT0_TOKEN_SCOPE_StatementScopeFinder 1
#endif
#if MINITEST_TEST_UT0_TOKEN_SCOPE_ParenScopeFinder
MINITEST(TestTkScope, TestCaseParenScopeFinder) {
  // Assumes no Error with lexing can occur.
  auto test_source = Lexer::Lex(
      "()\n"
      "(1)\n"
      "((1))\n"
      "(1 + (a) - (a+(abc+(a+a+a))))\n"
      "(1 + (a) - (a+{abc+(a+a+a)}))\n"
      "(1 + (a) - [(a)+{abc+(a+a+a)}])\n"
      "(() // invalid scope\n\0").Extract();

  // empty scope
  std::cout << "Testing empty scope:" << std::endl;
  TkScope empty_scope =
      TkScope::find_paren(test_source.cbegin(), test_source.cend());
  EXPECT_TRUE(empty_scope.Valid());
  for (auto i = empty_scope.Begin(); i != empty_scope.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // scope with 1 element
  std::cout << "Testing scope with 1 element:" << std::endl;
  TkScope scope_with_1_element =
      TkScope::find_paren(empty_scope.End(), test_source.cend());
  EXPECT_TRUE(scope_with_1_element.Valid());
  for (auto i = scope_with_1_element.Begin(); i != scope_with_1_element.End();
       i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // double scope
  std::cout << "Testing double scope:" << std::endl;
  TkScope double_scope =
      TkScope::find_paren(scope_with_1_element.End(), test_source.cend());
  EXPECT_TRUE(double_scope.Valid());
  for (auto i = double_scope.Begin(); i != double_scope.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // complex scope
  std::cout << "Testing complex scope:" << std::endl;
  TkScope complex_scope =
      TkScope::find_paren(double_scope.End(), test_source.cend());
  EXPECT_TRUE(complex_scope.Valid());
  for (auto i = complex_scope.Begin(); i != complex_scope.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // complex scope with lists
  std::cout << "Testing complex scope with lists:" << std::endl;
  TkScope complex_scope_with_lists =
      TkScope::find_paren(complex_scope.End(), test_source.cend());
  EXPECT_TRUE(complex_scope_with_lists.Valid());
  for (auto i = complex_scope_with_lists.Begin();
       i != complex_scope_with_lists.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // complex scope with frames and lists
  std::cout << "Testing complex scope with frames and lists:" << std::endl;
  TkScope complex_scope_with_frames_and_lists =
      TkScope::find_paren(complex_scope_with_lists.End(), test_source.cend());
  EXPECT_TRUE(complex_scope_with_frames_and_lists.Valid());
  for (auto i = complex_scope_with_frames_and_lists.Begin();
       i != complex_scope_with_frames_and_lists.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;
  // Invalid scope should be invalid
  auto test_source2 = Lexer::Lex("(()").Extract();

  std::cout << "Testing invalid scope:";
  TkScope invalid_list =
      TkScope::find_paren(test_source2.cbegin(), test_source2.cend());
  EXPECT_FALSE(invalid_list.Valid());
  std::cout << "scope Error message:" << invalid_list.Error() << std::endl;
}
END_MINITEST;
#endif

#if MINITEST_TEST_UT0_TOKEN_SCOPE_ListScopeFinder
MINITEST(TestTkScope, TestCaseListScopeFinder) {
  auto test_source = Lexer::Lex("{}{a}{{}}{({})[{}]{}}{{}");
  auto result = test_source.Extract();
  // empty list
  std::cout << "Testing empty list:";
  TkScope empty_list = TkScope::find_brace(result.cbegin(), result.cend());
  EXPECT_TRUE(empty_list.Valid());
  for (auto i = empty_list.Begin(); i != empty_list.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // list with 1 element
  std::cout << "Testing list with 1 element:";
  TkScope list_with_1_element =
      TkScope::find_brace(empty_list.End(), result.cend());
  EXPECT_TRUE(list_with_1_element.Valid());
  for (auto i = list_with_1_element.Begin(); i != list_with_1_element.End();
       i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // double list
  std::cout << "Testing double list:";
  TkScope double_list =
      TkScope::find_brace(list_with_1_element.End(), result.cend());
  EXPECT_TRUE(double_list.Valid());
  for (auto i = double_list.Begin(); i != double_list.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // complex list
  std::cout << "Testing complex list:";
  TkScope complex_list =
      TkScope::find_brace(double_list.End(), result.cend());
  EXPECT_TRUE(complex_list.Valid());
  for (auto i = complex_list.Begin(); i != complex_list.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  std::cout << "Testing invalid list:";
  TkScope invalid_list =
      TkScope::find_brace(complex_list.End(), result.cend());
  EXPECT_FALSE(invalid_list.Valid());
  std::cout << "scope Error message:" << invalid_list.Error() << std::endl;
}
END_MINITEST;
#endif

#if MINITEST_TEST_UT0_TOKEN_SCOPE_FrameScopeFinder
MINITEST(TestTkScope, TestCaseFrameScopeFinder) {
  auto test_source = Lexer::Lex("[][a][[]][([])[[]][]][[]");
  auto result = test_source.Extract();

  // empty frame
  std::cout << "Testing empty frame:";
  TkScope empty_frame = TkScope::find_bracket(result.cbegin(), result.cend());
  EXPECT_TRUE(empty_frame.Valid());
  for (auto i = empty_frame.Begin(); i != empty_frame.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // frame with 1 element
  std::cout << "Testing frame with 1 element:";
  TkScope frame_with_1_element =
      TkScope::find_bracket(empty_frame.End(), result.cend());
  EXPECT_TRUE(frame_with_1_element.Valid());
  for (auto i = frame_with_1_element.Begin(); i != frame_with_1_element.End();
       i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // double frame
  std::cout << "Testing double frame:";
  TkScope double_frame =
      TkScope::find_bracket(frame_with_1_element.End(), result.cend());
  EXPECT_TRUE(double_frame.Valid());
  for (auto i = double_frame.Begin(); i != double_frame.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // complex frame
  std::cout << "Testing complex frame:";
  TkScope complex_frame =
      TkScope::find_bracket(double_frame.End(), result.cend());
  EXPECT_TRUE(complex_frame.Valid());
  for (auto i = complex_frame.Begin(); i != complex_frame.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  std::cout << "Testing invalid frame:";
  TkScope invalid_frame =
      TkScope::find_bracket(complex_frame.End(), result.cend());
  EXPECT_FALSE(invalid_frame.Valid());
  std::cout << "scope Error message:" << invalid_frame.Error() << std::endl;
}
END_MINITEST;
#endif

#if MINITEST_TEST_UT0_TOKEN_SCOPE_StatementScopeFinder
MINITEST(TestTkScope, TestCaseStatementScopeFinder) {
  auto source_file = Lexer::Lex(
   "1;\n"
   "a = 1;\n"
   "a = (1;2;3);\n"
   "a = 1 + ([ 2 ;3 + {4;5;6}]);\n"
   "a = a + a + ([ a ;a + {a;a;a}]);\n");
  auto result = source_file.Extract();

  // Single value statement : 1;
  std::cout << "Testing single value statement" << std::endl;
  TkScope empty_statement = TkScope::find_statement(
      eTk::kNumberLiteral, eTk::kSemicolon, result.cbegin(), result.cend());
  EXPECT_TRUE(empty_statement.Valid());
  for (auto i = empty_statement.Begin(); i != empty_statement.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // statement with multiple tokens: a = 1;
  std::cout << "Testing statement with multiple tokens" << std::endl;
  TkScope multiple_token_statement = TkScope::find_statement(
      eTk::kIdentifier, eTk::kSemicolon, empty_statement.End(), result.cend());
  EXPECT_TRUE(multiple_token_statement.Valid());
  for (auto i = multiple_token_statement.Begin();
       i != multiple_token_statement.End();
       i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // statement with multiple tokens and scopes: a = (1;2;3);
  std::cout << "Testing statement with multiple tokens and scopes" << std::endl;
  TkScope multiple_token_scope_statement =
      TkScope::find_statement(eTk::kIdentifier, eTk::kSemicolon,
                               multiple_token_statement.End(), result.cend());
  EXPECT_TRUE(multiple_token_scope_statement.Valid());
  for (auto i = multiple_token_scope_statement.Begin();
       i != multiple_token_scope_statement.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // statement with lists frames and scopes nested in diffrent ways containing
  // end tokens. a = 1 + ([ 2 ;3 + {4;5;6}]);
  std::cout << "Testing statement with lists frames and scopes nested in "
               "diffrent ways containing end tokens."
            << std::endl;
  TkScope complex_statement = TkScope::find_statement(
      eTk::kIdentifier, eTk::kSemicolon, multiple_token_scope_statement.End(),
      result.cend());
  EXPECT_TRUE(complex_statement.Valid());
  for (auto i = complex_statement.Begin(); i != complex_statement.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;

  // Test finding an "open" statement which allows for repeated open tokens. ex
  // a = a + a + ([ a ;a + {a;a;a}]);
  std::cout << "Testing statement with lists frames and scopes nested in "
               "diffrent ways containing begin and end tokens."
            << std::endl;
  TkScope open_statement = TkScope::find_open_statement(
      eTk::kIdentifier, eTk::kSemicolon, complex_statement.End(), result.cend());
  EXPECT_TRUE(open_statement.Valid());
  EXPECT_TRUE(open_statement.End() == result.cend());
  for (auto i = open_statement.Begin(); i != open_statement.End(); i++)
    std::cout << i->Literal();
  std::cout << std::endl;
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
// File: token_scope.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_UNIT_TESTS_UT0_TOKEN_SCOPE_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
