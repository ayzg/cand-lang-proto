//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: castd
// File: import_stl.h
//---------------------------------------------------------------------------//
// Brief: Project-Wide C++ Standard Library Includes
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_CASTD_IMPORT_STL_H
#define HEADER_GUARD_CAOCO_CASTD_IMPORT_STL_H
// Includes:
// Basic Types
#include <cstddef>  // std::size_t
#include <string>   // std::string

// Containers
#include <array>  // std::array
#include <deque>
#include <initializer_list>  // std::initializer_list
#include <list>              // std::list
#include <map>               // std::map
#include <span>              // std::span
#include <stack>
#include <tuple>
#include <unordered_map>  // std::unordered_map
#include <vector>         // std::vector
#include <set>

// Utils
#include <cstdlib>     // numeric string conversions
#include <functional>  // std::reference_wrapper
#include <iterator>    // reverse_iterator
#include <limits>      // std::numeric_limits
#include <memory>      // std::unique_ptr , std::shared_ptr
#include <optional>
#include <variant>
// Algorithms
#include <algorithm>  // std::move, std::forward, std::get, std::ref, std::cref, std::any_of

// Type
#include <concepts>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

// Error handling
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <source_location>

// Macro: lambda
// Convention: 
// 1. Use lambda macro instead of auto when declaring lambdas
// 2. Lambda variable should be CamelCase with a preceding 'x'.
//    'x' stands for lambda expression. 
//    ex. lambda xMyLambda = []() {};
#define lambda auto

// Macro: lambda_string
// helper macro to create a lambda that returns a string at compile time.
#define LAMBDA_STRING(x) []()consteval->auto{return x;}

//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: castd
// File: import_stl.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_CASTD_IMPORT_STL_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
