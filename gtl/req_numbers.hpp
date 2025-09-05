//
// Created by zbook15 on 9/6/2020.
//

#ifndef PPP_REQUIRE_NUMBERS_HPP
#define PPP_REQUIRE_NUMBERS_HPP

#include <type_traits>
#include <limits>

#include "def_shortcuts.hpp"
#include "req_std.hpp"

namespace gtl::req {

/// <@concept:Arithmetic_>
/// <@brief: Specifies an integral type, a floating-point type; or a cv-qualified version thereof.>
    __CASET
    concept Arithmetic_ = std::is_arithmetic_v<T>;

/// <@concept:Boolean_>
/// <@brief: Specifies a type which is a bool, or any cv ref qualified version thereof.>
    __CASET
    concept Boolean_ = SameAs_<std::remove_cvref_t<T>,bool>;

/// <@concept:IntegralNumeral_>
/// <@brief: Specifies a type which is an integral excluding bool; or any cv ref qualified version thereof.>
    __CASET
    concept IntegralNumeral_ = Integral_<T> && not Boolean_<T>;

/// <@concept:IntegralNumeral_>
/// <@brief: Specifies a type which is an unsigned integral excluding bool; or any cv ref qualified version thereof.>
    __CASET
    concept UnsignedIntegralNumeral_ = IntegralNumeral_<T> && UnsignedIntegral_<T>;

/// <@concept:IntegralNumeral_>
/// <@brief: Specifies a type which is an signed integral excluding bool; or any cv ref qualified version thereof.>
    __CASET
    concept SignedIntegralNumeral_ = IntegralNumeral_<T> && SignedIntegral_<T>;

/// <@concept:Numeral_>
/// <@brief: Specifies an integral numeral type, a floating-point type; or a cv-qualified version thereof.>
    __CASET
    concept Numeral_ = IntegralNumeral_<T> || FloatingPoint_<T>;

/// <@concept:SpecializedArithmetic_>
/// <@brief: Specifies an arithmetic type that is specialized in std::numeric_limits.>
    __CASET
    concept SpecializedArithmetic_ = Arithmetic_<T> && std::numeric_limits<T>::is_specialized;

/// <@concept:SpecializedArithmetic_>
/// <@brief: Specifies an arithmetic type that is specialized and has at least one value that,
///          if used as an argument to an arithmetic operation, will generate a trap.>
    __CASET
    concept TrappingArithmetic_ = SpecializedArithmetic_<T> && std::numeric_limits<T>::traps;

/// <@concept:Iec559Floating_>
/// <@brief: Specifies floating-point type which fulfill the requirements of IEC 559 (IEEE 754) standard.
/// This means that std::numeric_limits<T>::has_infinity, std::numeric_limits<T>::has_quiet_NaN,
/// and std::numeric_limits<T>::has_signaling_NaN are also true.>
    __CASET
    concept Iec559Floating_ = FloatingPoint_<T> && std::numeric_limits<T>::is_iec559;

/// <@concept: TrappingFloating_>
/// <@brief: Specifies an floating type that is specialized and has at least one value that,
///          if used as an argument to an arithmetic operation, will generate a trap.>
    __CASET
    concept TrappingFloating_ = FloatingPoint_<T> && TrappingArithmetic_<T>;

/// <@concept: FloatingHasInf_>
/// <@brief: Specifies an floating type that is specialized and capable of representing
///          the positive infinity as a distinct special value.>
    __CASET
    concept FloatingHasInf_ = FloatingPoint_<T> && std::numeric_limits<T>::has_infinity;

/// <@concept: FloatingHasQuietNan_>
/// <@brief: Specifies an floating type that is capable of representing the special value
///          "Quiet Not-A-Number".>
    __CASET
    concept FloatingHasQuietNan_ = FloatingPoint_<T> && std::numeric_limits<T>::has_quiet_NaN;

/// <@concept: FloatingHasSignalingNan_>
/// <@brief: Specifies an floating type that can represent the special value "signaling not-a-number" (NaN).>
    __CASET
    concept FloatingHasSignalingNan_ = FloatingPoint_<T> && std::numeric_limits<T>::has_signaling_NaN;


/// <@concept:SafeFloating_>
/// <@brief: Iec559 compliant, non-trapping floating point types.>
    __CASET
    concept SafeFloating_ = Iec559Floating_<T> && not TrappingFloating_<T>;


}

#endif //PPP_REQUIRE_NUMBERS_HPP
