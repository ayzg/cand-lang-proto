
#ifndef FUNTIME_KEYWORDS_HPP
#define FUNTIME_KEYWORDS_HPP

#include <map>

struct Keywords {
    std::map<const char *,std::type_info> reserved;
    std::map<const char *,std::type_info> symbol;


};

#endif //FUNTIME_KEYWORDS_HPP
