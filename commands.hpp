//
// Created by zbook15 on 9/16/2020.
//

#ifndef FUNTIME_COMMANDS_HPP
#define FUNTIME_COMMANDS_HPP

#include <concepts>
#include <type_traits>


template<typename T>
concept Command_ = requires (T t){
    { t.string() } -> std::same_as<const char *>;
};

namespace command {
    struct Define {

    };
}

#endif //FUNTIME_COMMANDS_HPP
