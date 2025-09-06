//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: castd
// File: string_constant.h
//---------------------------------------------------------------------------//
// Brief: Constant evaluated string constants. Passable as template parameters.
//        Utilizes new C++20 lambda features to allow passing a string as a
//        template parameter to the string_constant struct. constexpr const char
//        * value is then used to access the string value.
// Example:
//       string_constant<[]() consteval { return "Hello, World!"; }> hello_world;
//       static_assert(hello_world.value == "Hello, World!");
//    
// Define a STRING_CONSTANT macro to make it easier to define. 
//    - Always #UNDEFINE AFTER!.
//    - Always provide description of the macro.
//    - Always use the macro in the following format:
// // Macro: STRING_CONSTANT
// // Description: Creates a string_constant from a string literal.
// //              Access string value with string_constant<>::value.
//    #define STRING_CONSTANT(STR) string_constant<[]() consteval { return STR; }>
// 
//    STRING_CONSTANT("Hello, World!") hello_world;
// Undef after!
//    #undef STRING_CONSTANT
//  
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_CASTD_STRING_CONSTANT_H
#define HEADER_GUARD_CAOCO_CASTD_STRING_CONSTANT_H
// Includes:
#include "import_stl.h"

//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

template <char CHAR>
using character_constant = std::integral_constant<char, CHAR>;

template <auto STR>
struct string_constant {
  static constexpr const char* value = STR();
};

using null_string_constant = string_constant<[]() consteval { return ""; }>;
// Macro: STRING_CONSTANT
// Description: Creates a string_constant from a string literal.
//              Access string value with string_constant<>::value.
#define STRING_CONSTANT(STR) string_constant<[]() consteval { return STR; }>
#undef STRING_CONSTANT

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: cand_official_compiler
// Directory: castd
// File: string_constant.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_CASTD_STRING_CONSTANT_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
