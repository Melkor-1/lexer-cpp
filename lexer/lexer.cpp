#include "lexer.h"

#include <cctype>
#include <cstddef>
#include <string>
#include <string_view>

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
    const std::size_t orig_pos {pos};
    for (; is_letter(ch); read_char())
        ;
    const std::string ident {input.substr(orig_pos, pos - orig_pos)};
    return Token {Token::lookup_ident(ident), ident};
}

Token Lexer::read_int()
{
    const std::size_t orig_pos {pos};
    for (; isdigit(static_cast<unsigned char>(ch)); read_char())
        ;
    return Token {Token::Type::Int, input.substr(orig_pos, pos - orig_pos)};
}

Token Lexer::read_string()
{
    /* Monkey doesn't support escape characters.  */
    const std::size_t orig_pos {pos + 1};
    do {
        read_char();
    } while (ch != '"' && ch != '\0');

    read_char();
    return 
        Token {ch == '\0' ? Token::Type::Illegal : Token::Type::String, input.substr(orig_pos, pos - orig_pos)};
}

Lexer::Lexer(const std::string_view &input) : input {input} 
{ 
    read_char(); 
}

Token Lexer::next()
{
    skip_whitespace();

    switch (ch) {
        case '\0':
            return Token {Token::Type::Eof, ""};

        case '=':
            if (peek_char() == '=') {
                read_char();
                read_char();
                return Token {Token::Type::Eq, "=="};
            }
            read_char();
            return Token {Token::Type::Assign, "="};

        case '+':
            read_char();
            return Token {Token::Type::Plus, "+"};

        case '-':
            read_char();
            return Token {Token::Type::Minus, "-"};

        case '!':
            if (peek_char() == '=') {
                read_char();
                read_char();
                return Token {Token::Type::Not_eq, "!="};
            }
            read_char();
            return Token {Token::Type::Bang, "!"};

        case '*':
            read_char();
            return Token {Token::Type::Asterisk, "*"};

        case '/':
            read_char();
            return Token {Token::Type::Slash, "/"};

        case '<':
            read_char();
            return Token {Token::Type::Lt, "<"};

        case '>':
            read_char();
            return Token {Token::Type::Gt, ">"};

        case ',':
            read_char();
            return Token {Token::Type::Comma, ","};

        case ';':
            read_char();
            return Token {Token::Type::Semicolon, ";"};

        case ':':
            read_char();
            return Token {Token::Type::Colon, ":"};

        case '(':
            read_char();
            return Token {Token::Type::Lparen, "("};

        case ')':
            read_char();
            return Token{Token::Type::Rparen, ")"};

        case '{':
            read_char();
            return Token {Token::Type::Lbrace, "{"};

        case '}':
            read_char();
            return Token {Token::Type::Rbrace, "}"};

        case '[':
            read_char();
            return Token {Token::Type::Lbracket, "["};

        case ']':
            read_char();
            return Token {Token::Type::Rbracket, "]"};

        case '"': {
            return read_string();
        }

        default:
            if (is_letter(ch)) {
                return read_ident();
            } else if (std::isdigit(static_cast<unsigned char>(ch))) {
                return read_int();
            }

            Token token {Token {Token::Type::Illegal, std::string{1, ch}}};
            read_char();
            return token;
    }
}
