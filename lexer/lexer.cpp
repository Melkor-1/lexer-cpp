#include "lexer.h"

#include <cctype>
#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>

void Lexer::read_char()
{
    ch = read_pos >= input.length() ? '\0' : input[read_pos];
    pos = read_pos++;
}

char Lexer::peek_char() const
{
    return read_pos >= input.length() ? '\0' : input[read_pos];
}

bool Lexer::is_letter(char c) const
{
    return c == '_' || std::isalpha(static_cast<unsigned char>(c));
}

void Lexer::skip_whitespace()
{
    for (; std::isspace(static_cast<unsigned char>(ch)); read_char())
        ;
}

Token Lexer::read_ident()
{
    const std::size_t orig_pos{pos};
    for (; is_letter(ch); read_char())
        ;
    const std::string ident{input.substr(orig_pos, pos - orig_pos)};
    return Token{Token::lookup_ident(ident), ident};
}

Token Lexer::read_int()
{
    const std::size_t orig_pos{pos};
    for (; isdigit(static_cast<unsigned char>(ch)); read_char())
        ;
    return Token{Token::Type::Int, input.substr(orig_pos, pos - orig_pos)};
}

Token Lexer::read_string()
{
    /* Monkey doesn't support escape characters.  */
    const std::size_t orig_pos{pos + 1};
    do {
        read_char();
    } while (ch != '"' && ch != '\0');

    read_char();
    return Token{ch == '\0' ? Token::Type::Illegal : Token::Type::String,
                 input.substr(orig_pos, pos - orig_pos)};
}

Lexer::Lexer(const std::string_view &input) : input{input} { read_char(); }

Token Lexer::next()
{
    skip_whitespace();

    /* For '==' && '!=', call read_char() twice before returning.
     * For is_letter and isdigit, do not call read_char().
     * For '"', call read_char() once.
     * For an illegal token, call read_char() once after instantiating the Token
     * object.
     * For the rest of the tokens, call read_char() once.
     */
    static std::unordered_map<std::string_view, Token::Type> token_map{
        {"=", Token::Type::Assign},   {"+", Token::Type::Plus},
        {"-", Token::Type::Minus},    {"!", Token::Type::Bang},
        {"*", Token::Type::Asterisk}, {"*", Token::Type::Asterisk},
        {"/", Token::Type::Slash},    {"<", Token::Type::Lt},
        {">", Token::Type::Gt},       {",", Token::Type::Comma},
        {":", Token::Type::Colon},    {";", Token::Type::Semicolon},
        {"(", Token::Type::Lparen},   {")", Token::Type::Rparen},
        {"{", Token::Type::Lbrace},   {"}", Token::Type::Rbrace},
        {"[", Token::Type::Lbracket}, {"]", Token::Type::Rbracket},
        {"\"", Token::Type::String},
    };

    if (is_letter(ch)) {
        return read_ident();
    } else if (std::isdigit(static_cast<unsigned char>(ch))) {
        return read_int();
    }

    if (const auto it{token_map.find(std::string_view{&ch, 1})};
        it != token_map.end()) {
        if (ch == '=' && peek_char() == '=') {
            read_char();
            read_char();
            return Token{Token::Type::Eq, "=="};
        } else if (ch == '!' && peek_char() == '=') {
            read_char();
            read_char();
            return Token{Token::Type::Not_eq, "!="};
        } else if (ch == '"') {
            return read_string();
        }
        read_char();
        return Token{it->second, it->first};
    }

    if (ch == '\0') {
        return Token{Token::Type::Eof, ""};
    }

    Token token{Token{Token::Type::Illegal, std::string{1, ch}}};
    read_char();
    return token;
}
