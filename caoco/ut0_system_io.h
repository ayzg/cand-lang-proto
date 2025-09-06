//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: unit_tests
// File: ut0_system_io.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_UT_CASTD_SYSTEM_IO_H
#define HEADER_GUARD_CAOCO_UT_CASTD_SYSTEM_IO_H
// Includes:
#include "minitest_pch.h" // All pre includes for each unit test
#include "minitest_flags.h" // Flags to enable or disable the unit tests
#include "minitest_util.h" // Utility methods shared among the all unit tests

#include "system_io.h"
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

MINITEST(utc0_LoadFileToU8VecAndPrint, ut0_system_io) {
  EXPECT_NO_THROW([]() {
    auto result = LoadFileToVec("ut0_system_io_utc0.candi");
  });
  auto result = LoadFileToVec("ut0_system_io_utc0.candi");

  // note: in case of MSVC, cl.exe /utf-8  is in use
  // msvc c++20 , type of elem is int
  // same is for clang and g++

  // Test printing a u8 literal.
  for (auto const &elem : u8"Всем привет! от буквы u8.\n") {
    std::printf("%c", elem);
  }

  // Test print u8 string
  std::basic_string<char8_t> message = u8"Всем привет! из струны U8.\n";
  for (auto const &elem : message) {
    std::printf("%c", elem);
  }

  // Test printing a u8 vector loaded from a file
  for (auto const &elem : result) {
    std::printf("%c", elem);
  }
} END_MINITEST

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: unit_tests
// File: ut0_system_io.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_UT_CASTD_SYSTEM_IO_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
