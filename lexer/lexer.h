#ifndef LEXER_H
#define LEXER_H 1

#include <cstddef>
#include <string_view>

#include "../token/token.h"

class Lexer {
    std::string_view input{};
    std::size_t pos{};
    std::size_t read_pos{};
    char ch{};

    void read_char();
    char peek_char() const;
    bool is_letter(char c) const;
    void skip_whitespace();
    Token read_ident();
    Token read_int();
    Token read_string();

  public:
    Lexer(const std::string_view &input);
    Token next();
};

#endif /* LEXER_H */
