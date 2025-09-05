#ifndef FUNTIME_TOKEN_HPP
#define FUNTIME_TOKEN_HPP

#include <concepts>
#include <type_traits>
#include "../gtl/req_gtl.hpp"
#include <string>
#include <sstream>
#include <vector>

// GTL
using cstring = const char *;

namespace asci {
    constexpr bool isdigit(int c) noexcept {
        return c >= '0' && c <= '9';
    }

    constexpr bool isxdigit(int c) noexcept {
        return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
    }

    constexpr bool islower(int c) noexcept {
        return c >= 'a' && c <= 'z';
    }

    constexpr bool isupper(int c) noexcept {
        return c >= 'A' && c <= 'Z';
    }

    constexpr bool isalpha(int c) noexcept {
        return islower(c) || isupper(c);
    }

    constexpr bool isalnum(int c) noexcept {
        return isalpha(c) || isdigit(c);
    }
}

// CANDIL
template<typename T>
concept Token_ = requires(T t) {
    { t.string() } -> gtl::req::SameAs_<cstring>;
};

enum class eToken : int {
    NUMBER,
    IDENTIFIER,
    SYMBOL,
    KEYWORD,
    BASE
};

enum class eSymbol : int {
    COMMAND,             /// # , direct command to Candil - usually precedes a command keyword.
    CAGE,                /// # , the opposite of AUTO, full constraints
    POINTER,             /// ^
    REFERENCE,           /// &
    AUTO,                /// @ , allows compiler to deduce best value in place of auto key symbol
    TYPEOF,              /// $ , converts values to types and back.
    CONSTFULL,           /// ! , indicates constexpr or constinit, or constmake (cmake compile time methods)
    CONSTONLY,           /// !! , indicates consteval
    LEFT_SBRACKET,       /// [ , beggining enclosing bracket for type parameters/compile time operations
    RIGHT_SBRACKET,      /// ]
    LEFT_CBRACKET,       /// {
    RIGHT_CBRACKET,      /// }
    LEFT_ABRACKET,       /// <
    RIGHT_ABRACKET,      /// >
    LEFT_BRACKET,        /// (
    RIGHT_BRAKET,        /// )
    BASE
};

// Keywords proceeding a COMMAND(#) symbol.
enum class eCommandKeyword : int {
    DEFINE = 1, /// Define : Defines an variable in memory. Use:
    /// #define identifier <generic-parameters>[type-constraints]{type-initializer}
    ///         = [value-constraints]{value-initializer}
    /// Example:
    /// #define myCoolVar <$int_type>[#enable<int,long,longlong>(int_type),gtl::vector<int_type]
    /// {#require (gtl::vector::input_values < 39), #require (gtl::vector::existing_values < 39)}
    /// Then later:
    /// #assign myAssignedVar myCoolVar <int> [1,10,obj.>{valuex,valuey},40]{&runtime_value}
    /// /// Error on 40 or or valuex and valuey at compile time.
    /// /// Warning on runtime_value being unconstrained to atleast the same amount.
    /// /// Runtime Error when runtime_var > 40, aborts unless handler exists
    /// #assign push40IntoMyVar myAssignedVar::push [40]
    ///
    /// #do [push40IntoMyVar] // Error
    ///                                                                        ///
    ASSIGN = 2, /// #assign existing-identifier <generic-value>[compile-time-init]{calltime-init]
    BUILD = 3,
    INCLUDE = 4,
};

template<eSymbol Symbol>
consteval const char * to_cstring() {
    if constexpr (Symbol == eSymbol::COMMAND){
        return "#";
    }
    if constexpr (Symbol == eSymbol::CAGE){
        return "#";
    }
    else if constexpr (Symbol == eSymbol::AUTO){
        return "@";
    }
}

template<eCommandKeyword CommandKeyword>
consteval const char * to_cstring() {
    if constexpr (CommandKeyword == eCommandKeyword::DEFINE){
        return "define";
    }
    if constexpr (CommandKeyword == eCommandKeyword::ASSIGN){
        return "assign";
    }
    if constexpr (CommandKeyword == eCommandKeyword::BUILD){
        return "build";
    }
    if constexpr (CommandKeyword == eCommandKeyword::INCLUDE){
        return "include";
    }
}

struct Identifier {
    const char * id;
    consteval Identifier(const char * id):id(id){
    }
};

template<typename T>
concept Identifier_ = requires(T t) {
    {t.id} -> gtl::req::SameAs_<const char *>;
};

template<Identifier_ auto Id>
consteval const char * to_cstring() {
    return Id.id;
}


consteval auto get_identifier(std::string_view::const_iterator cursor, std::string_view::const_iterator source_end){

    // While current char is alphanumeric or an underscore and not end of source.
    while ((asci::isalnum(*cursor) || *cursor == '_' )&& cursor != source_end){
        //
        cursor++;
    }
}

consteval auto tokenize(std::string_view source) {
    auto cursor = source.cbegin();
    for (;;){
        // Strip whitespace
        while (cursor != end(source) && (*cursor == ' '))
            ++cursor;

        // If the current character is an end - of - string then,
        if( cursor == std::end(source) ) break; // End loop

        // Find a CANDIL command symbol '#' otherwise - fatal compile error.
        if( cursor == to_cstring<eSymbol::COMMAND>()){
            // Find a command keyword

        }
        else {
            throw "[COMPILER][ERROR][FATAL] Cannot find CANDIL entry point.";
        }

    }
};



#endif //FUNTIME_TOKEN_HPP
