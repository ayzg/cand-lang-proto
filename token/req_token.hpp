//
// Created by zbook15 on 9/16/2020.
//

#ifndef FUNTIME_TOKEN_REQ_HPP
#define FUNTIME_TOKEN_REQ_HPP

#include <concepts>
#include <type_traits>

#include "../gtl/req_gtl.hpp"

template<typename T>
concept Token_ = requires(T t) {
    { t.string() } -> gtl::req::SameAs_<const char *>;
};

namespace stest {
    struct $Token_ {
        constexpr const char *string() { return ""; };
    };
    static_assert(Token_<$Token_>);
}



#endif //FUNTIME_TOKEN_REQ_HPP
