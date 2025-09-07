//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: cand_official_compiler_tests
// Directory: ut/castd
// File: ut0_system_io.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAND_OFFICIAL_COMPILER_TESTS_UT_CASTD_SYSTEM_IO_H
#define HEADER_GUARD_CAND_OFFICIAL_COMPILER_TESTS_UT_CASTD_SYSTEM_IO_H
// Includes:
#include <Windows.h>
#undef min
#undef max
#include "../../pch.h"
#include "../../caoco_gtest.h"

//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

TEST(utc0_LoadFileToU8VecAndPrint, ut0_system_io) {
  auto result = LoadFileToVec("ut_scripts/ut0_system_io_utc0.candi");

  // note: in case of MSVC, cl.exe /utf-8  is in use
  // msvc c++20 , type of elem is int
  // same is for clang and g++
  
  // Test printing a u8 literal.
  UINT console_code_page = GetConsoleOutputCP();
  SetConsoleOutputCP(CP_UTF8);
  for (auto const &elem : u8"Всем привет! от буквы u8.\n") {
    std::printf("%c", elem);
  }
  SetConsoleOutputCP(console_code_page); // Reset console code page

  // Test print u8 string
  console_code_page = GetConsoleOutputCP();
  SetConsoleOutputCP(CP_UTF8);
  std::basic_string<char8_t> message = u8"Всем привет! из струны U8.\n";
  for (auto const &elem : message) {
    std::printf("%c", elem);
  }
  SetConsoleOutputCP(console_code_page);  // Reset console code page

  // Test printing a u8 vector loaded from a file
  console_code_page = GetConsoleOutputCP();
  SetConsoleOutputCP(CP_UTF8);
  for (auto const &elem : result) {
    std::printf("%c", elem);
  }
  SetConsoleOutputCP(console_code_page);  // Reset console code page
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
// File: ut0_system_io.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAND_OFFICIAL_COMPILER_TESTS_UT_CASTD_SYSTEM_IO_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
