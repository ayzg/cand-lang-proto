//
// Created by zbook15 on 9/10/2020.
//

#ifndef FUNTIME_TOKENIZER_HPP
#define FUNTIME_TOKENIZER_HPP

#include <map>
#include <vector>
#include <stdexcept>
#include <tuple>

class TokenizerEXC : public std::exception {
    const char * msg_;
    const char * source_code_;
    size_t		location_;
public:
    TokenizerEXC( const char ** source_code, size_t location, char const* msg )
    : std::exception()
    , msg_(msg)
    , source_code_(*source_code)
    , location_(location)
    {

    }

    [[nodiscard]] const char * getSourceCode() const { return source_code_; }
    [[nodiscard]] size_t getLocation() const { return location_; }
    [[nodiscard]] const char* what() const noexcept override {
        return msg_;
    };

};

class TokenizerBadCharEXC : public TokenizerEXC {
    TokenizerBadCharEXC(const char ** source_code, size_t location)
    : TokenizerEXC(source_code,location,"Bad character in source code.") {}
};

class TokenizerNumOverflowEXC : public TokenizerEXC {
    TokenizerNumOverflowEXC(const char ** source_code, size_t location)
            : TokenizerEXC(source_code,location,"Too many digits in numeric literal.") {}
};


class tokenizer {
    auto tokenize(const char * source,int source_size) {
        if(source_size == 0)
            throw "No input provided.";

        auto is_source_read = false;
        auto iter_counter = 0;
        auto cursor_counter = 0;
        auto cursor_current = 0;
        auto line_counter = 0;
        auto line_current = 0;

        auto current_char = &source[0];
        auto previous_char = ' ';


    }
};


#endif //FUNTIME_TOKENIZER_HPP
