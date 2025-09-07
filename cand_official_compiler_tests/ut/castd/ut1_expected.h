//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: cand_official_compiler_tests
// Directory: ut/castd
// File: ut0_expected.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAND_OFFICIAL_COMPILER_TESTS_UT_CASTD_EXPECTED_H
#define HEADER_GUARD_CAND_OFFICIAL_COMPILER_TESTS_UT_CASTD_EXPECTED_H
// Includes:
#include "../../pch.h"
#include "../../caoco_gtest.h"
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

TEST(utc0_ExpectedConstrtuct, ut1_castd_expected) { 
  using ExpectedString = Expected<std::string>;
  ExpectedString e_good = ExpectedString::Success("42");
  ExpectedString e_bad = ExpectedString::Failure("42");
  std::string compare = "42";
  EXPECT_TRUE(e_good.Valid());
  EXPECT_FALSE(e_bad.Valid());
  EXPECT_EQ(e_good.Value(), compare);
  EXPECT_EQ(e_bad.Error(), compare);
  std::cout << e_bad.Error() << std::endl;
  
  // Good should be invalid after extraction of value.
  std::string value = e_good.Extract();
  EXPECT_FALSE(e_good.Valid());
  EXPECT_EQ(e_good.Error(), "Value has been moved out of the expected object.");
  std::cout << e_good.Error() << std::endl;
  
  // - Chaining Errors properly preserves order of errors.
  // - Chaining errors seperates error messages by a single newline character.
  // - Test Case: e_bad is a recieved error. 
  //              e_err is the chain error returned by the reciever.
  ExpectedString e_err = e_bad.ChainFailure("Error 2: 42 was caught!");
  EXPECT_FALSE(e_err.Valid());
  EXPECT_EQ(e_err.Error(), "42\nError 2: 42 was caught!");
  std::cout << e_err.Error() << std::endl;

  // Implicit conversion to boolean to conditional expressions.
  EXPECT_FALSE(e_err);
  EXPECT_FALSE(e_good);
  EXPECT_FALSE(e_bad);
}

TEST(utc1_PartialExpectedConstrtuct, ut1_castd_expected) {
  using PartialExpectedString = PartialExpected<std::string,std::string>;
  PartialExpectedString e_good = PartialExpectedString::Success("Always", "Good");
  PartialExpectedString e_bad = PartialExpectedString::Failure("Always", "Bad");
  EXPECT_TRUE(e_good.Valid());
  EXPECT_FALSE(e_bad.Valid());
  EXPECT_EQ(e_good.Always(), "Always");
  EXPECT_EQ(e_bad.Always(), "Always");
  EXPECT_EQ(e_good.Value(), "Good");
  EXPECT_EQ(e_bad.Error(), "Bad");
  std::cout << e_bad.Error() << std::endl;

  // Good should be invalid after extraction of value.
  std::string value = e_good.Extract();
  EXPECT_FALSE(e_good.Valid());
  EXPECT_EQ(e_good.Error(), "Value has been moved out of the expected object.");
  std::cout << e_good.Error() << std::endl;

  // - Chaining Errors properly preserves order of errors.
  // - Chaining errors seperates error messages by a single newline character.
  // - Test Case: e_bad is a recieved error.
  //              e_err is the chain error returned by the reciever.
  // ChainFailure is NOT provided with a new always value. 
  //      -> Old value is passed.
  PartialExpectedString e_err = e_bad.ChainFailure("Error 2: Bad was caught!");
  EXPECT_FALSE(e_err.Valid());
  EXPECT_EQ(e_err.Error(), "Bad\nError 2: Bad was caught!");
  EXPECT_EQ(e_err.Always(), e_bad.Always());
  std::cout << e_err.Error() << std::endl;

  // ChainFailure is NOT provided with a new always value.
  //      -> Old value is passed.
  PartialExpectedString e_err_new_always =
      PartialExpectedString::ChainFailure(e_bad,"VeryBad","Error 2: Bad was caught!");
  EXPECT_FALSE(e_err_new_always.Valid());
  EXPECT_EQ(e_err_new_always.Error(), "Bad\nError 2: Bad was caught!");
  EXPECT_NE(e_err_new_always.Always(), e_bad.Always());
  EXPECT_EQ(e_err_new_always.Always(), "VeryBad");
  std::cout << e_err_new_always.Always() << std::endl;
  
  // Implicit conversion to boolean to conditional expressions.
  EXPECT_FALSE(e_err);
  EXPECT_FALSE(e_good);
  EXPECT_FALSE(e_bad);
}

TEST(utc1_BoolErrorConstrtuct, ut1_castd_expected) { 
  BoolError bool_err = BoolError("Error");
  BoolError bool_good = BoolError(true);
  BoolError bool_bad = BoolError(false);


  // bool_err is not valid.
  // bool_good is valid.
  // bool_bad is not valid.
  EXPECT_FALSE(bool_err.Valid());
  EXPECT_TRUE(bool_good.Valid());
  EXPECT_FALSE(bool_bad.Valid());

  EXPECT_EQ(bool_err.Error(), "Error");
  EXPECT_EQ(bool_bad.Error(), "BoolError: Unspecified false error.");

  // Implicit conversion to boolean to conditional expressions.
  EXPECT_FALSE(bool_err);
  EXPECT_TRUE(bool_good);
  EXPECT_FALSE(bool_bad);
}

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: cand_official_compiler_tests
// Directory: ut/castd
// File: ut0_expected.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAND_OFFICIAL_COMPILER_TESTS_UT_CASTD_EXPECTED_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
