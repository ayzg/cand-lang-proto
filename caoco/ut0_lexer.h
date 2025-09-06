//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: unit_tests
// File: ut0_lexer.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_UNIT_TESTS_UT0_LEXER_H
#define HEADER_GUARD_CAOCO_UNIT_TESTS_UT0_LEXER_H
// Includes:
#include "minitest_pch.h" // All pre includes for each unit test
#include "minitest_flags.h" // Flags to enable or disable the unit tests
#include "minitest_util.h" // Utility methods shared among the all unit tests

#include "lexer.h"

//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
// todo:seperate file to control test flags
#define CAOCO_TEST_TOKENIZER false

#if CAOCO_TEST_TOKENIZER
#define CAOCO_TEST_TOKENIZER_Keywords true
#define CAOCO_TEST_TOKENIZER_KeywordsMixedShouldThrow true
#define CAOCO_TEST_TOKENIZER_KeywordsDirectiveReportEarlyMisspell true
#endif

#if CAOCO_TEST_TOKENIZER_Keywords

MINITEST(Test_Lexer, TestCase_Keywords) {
  auto keywords = Lexer::Lex(
      "include macro enter start use class obj private "
      "public const static if else elif while for switch "
      "break continue return print none int uint real "
      "byte bit str type value identity array pointer memory function "
      "def auto fn default any case");

  TkVector expected_test_result = TkVector(
      {{eTk::kInclude}, {eTk::kMacro},         
       {eTk::kUse},     {eTk::kClass},            {eTk::kPrivate},
       {eTk::kPublic},  {eTk::kConst},       {eTk::kStatic},   {eTk::kIf},
       {eTk::kElse},    {eTk::kElif},        {eTk::kWhile},    {eTk::kFor},
       {eTk::kSwitch},  {eTk::kBreak},       {eTk::kContinue}, {eTk::kReturn},
         {eTk::kNoneLiteral}, {eTk::kInt},      {eTk::kUint},
       {eTk::kReal},    {eTk::kByte},        {eTk::kBit},      {eTk::kStr},
        {eTk::kArray},
    {eTk::kDef},
       {eTk::kAuto},    {eTk::kFn},          {eTk::kDefault},  {eTk::kAny},
       {eTk::kCase}});

  // non-directive keywords
  if (keywords.Valid()) {
    TkVector result = keywords.Extract();
    for (Tk& tk : result) {
      std::cout << ToStr(tk.Type()) << " ";
    }
    std::cout << std::endl;
    for (size_t i = 0; i < result.size(); ++i) {
      EXPECT_EQ(result[i].Type(), expected_test_result[i].Type());
      if (result[i].Type() != expected_test_result[i].Type()) {
        std::cout << "Tokenization result expected keyword token: "
                  << ToStr(expected_test_result[i].Type())
                  << " Got: " << ToStr(result[i].Type()) << std::endl;
      }
    }
  } else {
    EXPECT_TRUE(keywords.Valid());
    std::cout << keywords.Error() << std::endl;
  }
}
END_MINITEST;
#endif

#if CAOCO_TEST_TOKENIZER_KeywordsDirectiveReportEarlyMisspell
MINITEST(Test_Lexer, TestCase_KeywordsDirectiveReportEarlyMisspell) {
  auto misspelled_keyword = Lexer::Lex("#inclde");
  EXPECT_FALSE(misspelled_keyword.Valid());
  if (!misspelled_keyword.Valid()) {
    std::cout << misspelled_keyword.Error() << std::endl;
  }
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
// File: ut0_lexer.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_UNIT_TESTS_UT0_LEXER_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
