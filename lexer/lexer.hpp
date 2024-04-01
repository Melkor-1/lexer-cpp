#ifndef LEXER_HPP
#define LEXER_HPP 1

#include <cstddef>
#include <string_view>

#include "../token/token.hpp"

class Lexer {
    std::string_view input;
    std::size_t pos{};
    std::size_t read_pos{};
    char ch;

    void read_char();
    char peek_char();
    bool is_letter(char c);
    void skip_whitespace();
    std::string_view read_ident();
    std::string_view read_int();
    std::string_view read_string();

  public:
    Lexer(const std::string_view &input);
    Token next();
};

#endif /* LEXER_HPP */
