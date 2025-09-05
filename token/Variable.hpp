//
// Created by zbook15 on 9/16/2020.
//

#ifndef FUNTIME_VARIABLE_HPP
#define FUNTIME_VARIABLE_HPP

template<typename ValueT,const char * VAR_NAME>
struct VariableDeclaration {
    using value_type = ValueT;
    static constexpr const char * name = VAR_NAME;
    constexpr auto string() const { return name; }
};


#endif //FUNTIME_VARIABLE_HPP
