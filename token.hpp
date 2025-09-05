//
// Created by zbook15 on 9/11/2020.
//

#ifndef FUNTIME_TOKEN_HPP
#define FUNTIME_TOKEN_HPP

#include <map>
#include <string>
#include <concepts>
#include <type_traits>


template<typename T>
concept Void_ = std::is_void_v<T>;

template<typename T>
concept NotVoid_ = not std::is_void_v<T>;

template<typename T>
concept Scalar_ = std::is_scalar_v<T>;

template<typename T>
concept Function_ = std::is_function_v<T>;

namespace token_req {
    namespace detail {
        struct $Token_ {
            virtual constexpr const char *string() { return ""; };
        };

        template<typename ValT>
        struct $Operand_ {
            virtual constexpr const char *string() { return ""; };
            virtual constexpr ValT value() { return ValT(); }
        };

        template<typename ... ArgTs>
        struct $Operation_ {
            virtual constexpr const char *string() { return ""; };
            virtual constexpr int nargs() { return 1; };
            virtual constexpr int perform(ArgTs...) {return nargs();}
        };
    }

    enum class eOperatorPrecedence : int {
        MIN = 0,
        ASSIGNMENT,
        LOGOR,
        LOGXOR,
        LOGAND,
        BITOR,
        BITXOR,
        BITAND,
        EQUALITY,
        RELATIONAL,
        BITSHIFT,
        ADDITIVE,
        MULTIPLICATIVE,
        POWER,
        UNARY,
        POSTFIX,
        MAX
    };

    enum class eOperatorAssociation : int {
        RIGHT,
        LEFT,
        UNARY
    };

    template<typename T>
    concept Token_ = requires(T t) {
        { t.string() } -> std::same_as<const char *>;
    };
    static_assert(Token_<detail::$Token_>);

    template<typename T>
    concept Operand_ = requires(T t) {
        requires Token_<T>;
        { t.value() } -> NotVoid_;
    };
    static_assert(Operand_<detail::$Operand_<int>>);

    template<typename T>
    concept Operation_ = requires (T t) {
        requires Token_<T>;
        { t.nargs() } -> std::convertible_to<int>;
        { t.perform() } -> Function_;
    };
    //static_assert(Operation_<detail::$Operation_<int>> );

    template<typename T>
    concept Operator_ = requires (T t) {
        requires Operation_<T>;
        { t.prece() } -> std::same_as<eOperatorPrecedence>;
        { t.assoc() } -> std::same_as<eOperatorAssociation>;
    };




}



#endif //FUNTIME_TOKEN_HPP
