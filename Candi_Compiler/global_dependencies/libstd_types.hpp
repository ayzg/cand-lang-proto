// Rules about naming:
/* Source: https://stackoverflow.com/questions/228783/what-are-the-rules-about-using-an-underscore-in-a-c-identifier
The rules (which did not change in C++11):

Reserved in any scope, including for use as implementation macros:
	identifiers beginning with an underscore followed immediately by an uppercase letter
	identifiers containing adjacent underscores (or "double underscore")
Reserved in the global namespace:
	identifiers beginning with an underscore
Also, everything in the std namespace is reserved. (You are allowed to add template specializations, though.)

From the 2003 C++ Standard:

17.4.3.1.2 Global names [lib.global.names]
Certain sets of names and function signatures are always reserved to the implementation:

Each name that contains a double underscore (__) or begins with an underscore followed by an uppercase letter (2.11) is reserved to the implementation for any use.
Each name that begins with an underscore is reserved to the implementation for use as a name in the global namespace.165
165) Such names are also reserved in namespace ::std (17.4.3.1).

The C++ language is based on the C language (1.1/2, C++03), and C99 is a normative reference (1.2/1, C++03), 
so it's useful to know the restrictions from the 1999 C Standard (although they do not apply to C++ directly):

7.1.3 Reserved identifiers
Each header declares or defines all identifiers listed in its associated subclause, 
and optionally declares or defines identifiers listed in its associated future library directions 
subclause and identifiers which are always reserved either for any use or for use as file scope identifiers.

All identifiers that begin with an underscore and either an uppercase letter or another underscore are always reserved for any use.
All identifiers that begin with an underscore are always reserved for use as identifiers with file scope in both the ordinary and tag name spaces.
Each macro name in any of the following subclauses (including the future library directions) is reserved for use as specified 
if any of its associated headers is included; unless explicitly stated otherwise (see 7.1.4).
All identifiers with external linkage in any of the following subclauses (including the future library directions) 
are always reserved for use as identifiers with external linkage.154
Each identifier with file scope listed in any of the following subclauses (including the future library directions) 
is reserved for use as a macro name and as an identifier with file scope in the same name space if any of its associated headers is included.
No other identifiers are reserved. If the program declares or defines an identifier in a context in which it is reserved (other than as allowed by 7.1.4), 
or defines a reserved identifier as a macro name, the behavior is undefined.

If the program removes (with #undef) any macro definition of an identifier in the first group listed above, the behavior is undefined.

154) The list of reserved identifiers with external linkage includes errno, math_errhandling, setjmp, and va_end.

Other restrictions might apply. For example, the POSIX standard reserves a lot of identifiers that are likely to show up in normal code:

Names beginning with a capital E followed a digit or uppercase letter:
may be used for additional error code names.
Names that begin with either is or to followed by a lowercase letter
may be used for additional character testing and conversion functions.
Names that begin with LC_ followed by an uppercase letter
may be used for additional macros specifying locale attributes.
Names of all existing mathematics functions suffixed with f or l are reserved
for corresponding functions that operate on float and long double arguments, respectively.
Names that begin with SIG followed by an uppercase letter are reserved
for additional signal names.
Names that begin with SIG_ followed by an uppercase letter are reserved
for additional signal actions.
Names beginning with str, mem, or wcs followed by a lowercase letter are reserved
for additional string and array functions.
Names beginning with PRI or SCN followed by any lowercase letter or X are reserved
for additional format specifier macros
Names that end with _t are reserved
for additional type names.
While using these names for your own purposes right now might not cause a problem, they do raise the possibility of conflict with future versions of that standard.

*/

#pragma once
// Basic Types
#include <cstddef> // std::size_t
#include <string> // std::string

// Containers
#include <array> // std::array
#include <vector> // std::vector
#include <map> // std::map
#include <unordered_map> // std::unordered_map
#include <list> // std::list
#include <initializer_list> // std::initializer_list
#include <tuple>

// Utils
#include <cstdlib> // numeric string conversions
#include <memory> // std::unique_ptr , std::shared_ptr
#include <functional> // std::reference_wrapper
#include <limits> // std::numeric_limits
#include <iterator> // reverse_iterator
#include <optional>

// Algorithms
#include <algorithm> // std::move, std::forward, std::get, std::ref, std::cref, std::any_of

// Type
#include <typeinfo>
#include <typeindex>

// Error handling
#include <stdexcept>

//#include <sstream> // For error reporting
#include <iostream>
#include <fstream>

#define SL_S static
#define SL_IN inline
#define SL_CX constexpr
#define SL_SIN static inline
#define SL_CXIN constexpr inline
#define SL_CXSIN constexpr static inline
#define SL_CXS constexpr static

//#define __CINIT constinit
#define SL_CE consteval
#define SL_CINIT constinit
#define SL_CXIF if constexpr

#define SL_T typename
#define SL_CASE template
#define SL_CASET template<typename T>
#define SL_CASEF template<>

namespace caoco {

	/// <@section:Basic Types>
	using sl_size = std::size_t;
	using sl_string = std::string;
	
	/// <@section:Utils>
	template<class T,class Deleter = std::default_delete<T>> 
	using sl_uptr = std::unique_ptr<T, Deleter>;

	template<class T> 
	using sl_sptr = std::shared_ptr<T>;

	template<class T>
	using sl_ref = std::reference_wrapper<T>;

	template<class T>
	using sl_limits = std::numeric_limits<T>;

	template<class T>
	using sl_char_traits = std::char_traits<T>;

	template<class T>
	using sl_reverse_iterator = std::reverse_iterator<T>;

	template<class T>
	using sl_opt = std::optional<T>;
	using sl_nullopt = std::nullopt_t;

	/// <@section:Containers>
	template<typename T, std::size_t N>
	using sl_array = std::array<T, N>;

	template<class T,class Allocator = std::allocator<T>> 
	using sl_vector = std::vector<T,Allocator>;

	template<class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, T>>>
	using sl_map = std::map<Key, T, Compare, Allocator>;

	template<class Key, class T, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>, class Allocator = std::allocator<std::pair<const Key, T>>>
	using sl_unordered_map = std::unordered_map<Key, T, Hash, KeyEqual, Allocator>;

	template<class T, class Allocator = std::allocator<T>>
	using sl_list = std::list<T, Allocator>;

	template<class T>
	using sl_ilist = std::initializer_list<T>;

	template<class... Types>
	using sl_tuple = std::tuple<Types...>;

	// Error handing
	//using sl_exception = std::exception;
	using sl_runtime_error = std::runtime_error;
	using sl_out_of_range = std::out_of_range;



	// Type
	using sl_type_info = std::type_info;
	using sl_type_index = std::type_index;

	// Specific typedefs which will be used often.
	using sl_u8string = std::basic_string<char8_t>;
	using sl_char8_vector = std::vector<char8_t>;
	using sl_char8_vector_it = sl_char8_vector::iterator;
	using sl_char8_vector_cit = sl_char8_vector::const_iterator;

	// standard library functions
	namespace sl {
		constexpr sl_nullopt nullopt = std::nullopt;
		/// <@section:> utils
		using std::move;
		using std::forward;
		using std::get;
		using std::ref;
		using std::cref;

		/// <@section:Numeric string conversion>
		using std::atof;
		using std::atoi;
		using std::atol;
		using std::atoll;
		using std::strtol;
		using std::strtoll;
		using std::strtoul;
		using std::strtoull;
		using std::strtof;
		using std::strtod;
		using std::strtold;

		// Algorithms
		using std::any_of;
	}
}