//=-------------------------------------------------------------------------=//
// Global Dependencies
//---------------------------------------------------------------------------//
#include "import_stl.h"
#include "minitest.h"  // Minimal Unit Testing Framework
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

//=-------------------------------------------------------------------------=//
// C& Official Compiler
//---------------------------------------------------------------------------//

// C& Standard Library
#include "expected.h"
#include "string_constant.h"
#include "system_io.h"

// Compiler Common Utilities
#include "cand_char_traits.h"
#include "cand_syntax.h"
#include "compiler_error.h"
#include "token_cursor.h"
#include "token_scope.h"

// Compiler Tools
#include "lark_parser.h"
#include "lexer.h"
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

//=-------------------------------------------------------------------------=//
// Unit Tests
//---------------------------------------------------------------------------//
#include "minitest_flags.h"  // Flags to enable or disable the unit tests
#include "minitest_pch.h"    // All pre includes for each unit test
#include "minitest_util.h"   // Utility methods shared among the all unit tests

// Unit Tests
// Convention:
// 1. All unit test files should be named as utX_name.h
//      where X is the iteration of the unit test module.
//
// 2. All unit test files should include the following headers
//        #include "minitest_flags.h" // Flags to enable or disable the unit
//        tests #include "minitest_pch.h" // All pre includes for each unit test
//        #include "minitest_util.h" // Utility methods shared among the all
//        unit tests
//
// 3. Unit test headers must NOT EVER include another unit test 'utX_' header.
//    or have dependencies on other unit tests.
//
// 4. Unit tests auto-register.
//    To disable and enable a test create a flag in minitest_flags.h.
//    The format should be for example unit test of parser_utils.h:
//      1.Inside minitest_flags.h:
//          #define CAOCO_UNIT_TEST0_PARSER_UTILS 1
//      2.Inside your unit test file:
//          #if CAOCO_UNIT_TEST0_PARSER_UTILS
//          #define CAOCO_UNIT_TEST0_PARSER_UTILS_BasicScopeFinder 1
//          #define CAOCO_UNIT_TEST0_PARSER_UTILS_ListScopeFinder 1
//          #define CAOCO_UNIT_TEST0_PARSER_UTILS_FrameScopeFinder 1
//          #define CAOCO_UNIT_TEST0_PARSER_UTILS_StatementScopeFinder 1
//          #endif
//    Surround each unit test with #if #endif.
// 5. Provide detailed description of the unit test above the include.
//        - Provide the macro to enable or disable the unit test.
//        - Provide the sub-macros to enable or disable the sub-tests.
//        - Provide info on which header file is being tested.
//        - Provide a brief description of the unit test.
//    For example:
//        // Unit Test: parser_utils.h
//        // Header: parser_utils.h
//        // Description: testing scope finders in parser_utils.h
//        // Macros:
//            1.CAOCO_UNIT_TEST0_PARSER_UTILS
//              1.1.CAOCO_UNIT_TEST0_PARSER_UTILS_BasicScopeFinder
//              1.2.CAOCO_UNIT_TEST0_PARSER_UTILS_ListScopeFinder
//              1.3.CAOCO_UNIT_TEST0_PARSER_UTILS_FrameScopeFinder
//              1.4.CAOCO_UNIT_TEST0_PARSER_UTILS_StatementScopeFinder

#include "ut0_expected.h"
#include "ut0_lexer.h"
#include "ut0_parser_basics.h"
#include "ut0_system_io.h"
#include "ut0_token_scope.h"
//#include "ut0_runtime.h"
FINISH_MINITESTS;  // Macro to finish the test suite
// Undefine all the minitest macros except MINITEST_RESULT
#undef MINITEST
#undef END_MINITEST
#undef FINISH_MINITESTS
#undef EXPECT_TRUE
#undef EXPECT_FALSE
#undef EXPECT_EQ
#undef EXPECT_NE
#undef EXPECT_ANY_THROW
#undef EXPECT_NO_THROW

//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

int main() { std::cout << "Hello World!\n"; }
