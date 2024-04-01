#include "lexer.hpp"

#include <cctype>
#include <cstddef>
#include <string_view>

void Lexer::read_char()
{
    ch = read_pos >= input.length() ? '\0' : input[read_pos];
    pos = read_pos++;
}

char Lexer::peek_char()
{
    return read_pos >= input.length() ? '\0' : input[read_pos];
}

bool Lexer::is_letter(char c)
{
    return c == '_' || std::isalpha(static_cast<unsigned char>(c));
}

void Lexer::skip_whitespace()
{
    for (; std::isspace(static_cast<unsigned char>(ch)); read_char())
        ;
}

std::string_view Lexer::read_ident()
{
    const std::size_t orig_pos{pos};
    for (; is_letter(ch); read_char())
        ;
    return input.substr(orig_pos, pos - orig_pos);
}

std::string_view Lexer::read_int()
{
    const std::size_t orig_pos{pos};
    for (; isdigit(static_cast<unsigned char>(ch)); read_char())
        ;
    return input.substr(orig_pos, pos - orig_pos);
}

std::string_view Lexer::read_string()
{
    /* Monkey doesn't support escape characters.
     * XXX: How to signal EOF? Returning an empty string can not be an error.
     *      Raise an exception?
     */
    const std::size_t orig_pos{pos + 1};
    do {
        read_char();
    } while (ch != '"' && ch != '\0');
    return input.substr(orig_pos, pos - orig_pos);
}

Lexer::Lexer(const std::string_view &input) : input(input) { read_char(); }

Token Lexer::next()
{
    skip_whitespace();

    switch (ch) {
        case '\0':
            return Token("", Token::Type::Eof);

        case '=':
            if (peek_char() == '=') {
                read_char();
                return Token("==", Token::Type::Eq);
            }
            read_char();
            return Token("=", Token::Type::Assign);

        case '+':
            read_char();
            return Token("+", Token::Type::Plus);

        case '-':
            read_char();
            return Token("-", Token::Type::Minus);

        case '!':
            if (peek_char() == '=') {
                read_char();
                return Token("!=", Token::Type::Not_eq);
            }
            read_char();
            return Token("!", Token::Type::Bang);

        case '*':
            read_char();
            return Token("*", Token::Type::Asterisk);

        case '/':
            read_char();
            return Token("/", Token::Type::Slash);

        case '<':
            read_char();
            return Token("<", Token::Type::Lt);

        case '>':
            read_char();
            return Token(">", Token::Type::Gt);

        case ',':
            read_char();
            return Token(",", Token::Type::Comma);

        case ';':
            read_char();
            return Token(";", Token::Type::Semicolon);

        case ':':
            read_char();
            return Token(":", Token::Type::Colon);

        case '(':
            read_char();
            return Token("(", Token::Type::Lparen);

        case ')':
            read_char();
            return Token(")", Token::Type::Rparen);

        case '{':
            read_char();
            return Token("{", Token::Type::Lbrace);

        case '}':
            read_char();
            return Token("}", Token::Type::Rbrace);

        case '[':
            read_char();
            return Token("[", Token::Type::Lbracket);

        case ']':
            read_char();
            return Token("]", Token::Type::Rbracket);

        case '"': {
            const std::string_view ident{read_string()};
            read_char();
            return Token(ident, Token::Type::String);
        }

        default:
            if (is_letter(ch)) {
                const std::string_view ident{read_ident()};
                return Token(ident, Token::lookup_ident(ident));
            }
            else if (std::isdigit(static_cast<unsigned char>(ch))) {
                return Token(read_int(), Token::Type::Int);
            }
            Token t{Token(std::string_view(&ch, 1), Token::Type::Illegal)};
            read_char();
            return t;
        }
}
