//
// Created by zbook15 on 9/6/2020.
//

#ifndef GTL_HEADGUARD_require_gtl_hpp
#define GTL_HEADGUARD_require_gtl_hpp

#include <concepts>
#include <type_traits>
#include "def_shortcuts.hpp"
#include "req_std.hpp"

namespace gtl::req {
    __CASET concept CopyConstructible_ = std::is_copy_constructible_v<T>;

    __CASET concept CopyAssignable_ = std::is_copy_assignable_v<T>;

    __CASET concept Copyable_ = CopyConstructible_<T> && CopyAssignable_<T>;

    __CASET concept NonCopyable_ = (not CopyConstructible_<T>) && (not CopyAssignable_<T>);

    __CASET concept MoveAssignable_ = std::is_move_assignable_v<T>;

    __CASET concept Movable_ = MoveConstructible_<T> && MoveAssignable_<T>;

    __CASET concept NonMovable_ = (not MoveConstructible_<T>) && (not MoveAssignable_<T>);

    __CASET concept MoveOnly_ = Movable_<T> && (not Copyable_<T>);

    __CASET concept NoThrowMoveConstructible_ = std::is_nothrow_move_constructible_v<T>;

    __CASET concept NoThrowMoveAssignable_ = std::is_nothrow_move_assignable_v<T>;

    __CASET concept MemberObjectPointer_ = std::is_member_object_pointer_v<T>;

    __CASET concept MemberFunctionPointer_ = std::is_member_function_pointer_v<T>;

    /// Cannot be moved copied or swapped
    __CASET concept Singular_ = NonCopyable_<T> && NonMovable_<T> && (not Swappable_<T>);

}
#endif // GTL_HEADGUARD_require_gtl_hpp
