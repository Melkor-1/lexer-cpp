#ifndef TOKEN_H
#define TOKEN_H 1

#include <array>
#include <string>
#include <string_view>
#include <vector>

/* This solution is vastly superior to any switch case or array based one,
 * because it doesn't duplicate the names, making it easy to change the
 * enumeration.
 */
#define FOREACH_TOKEN(_)    \
    _(Illegal)              \
    _(Eof)                  \
    _(Ident)                \
    _(Int)                  \
    _(String)               \
    _(Assign)               \
    _(Plus)                 \
    _(Minus)                \
    _(Asterisk)             \
    _(Slash)                \
    _(Bang)                 \
    _(Lt)                   \
    _(Gt)                   \
    _(Eq)                   \
    _(Not_eq)               \
    _(Comma)                \
    _(Semicolon)            \
    _(Colon)                \
    _(Lparen)               \
    _(Rparen)               \
    _(Lbrace)               \
    _(Rbrace)               \
    _(Lbracket)             \
    _(Rbracket)             \
    _(Function)             \
    _(Let)                  \
    _(True)                 \
    _(False)                \
    _(If)                   \
    _(Else)                 \
    _(Return)               \

class Token {
    static constexpr std::array token_names {
        #define GEN_STRING_COMMA(STRING) #STRING,
        FOREACH_TOKEN(GEN_STRING_COMMA)
        #undef GEN_STRING_COMMA
    };
    static constexpr std::size_t token_count {token_names.size()};

  public:
    enum class Type {
        #define GEN_ENUM_COMMA(ENUM) ENUM,
        FOREACH_TOKEN(GEN_ENUM_COMMA) 
        #undef GEN_ENUM_COMMA
    };

    Token::Type type;
    std::string literal;

    Token(Token::Type type, const std::string_view &literal) 
        : type {type}, literal {literal} { }
    static Token::Type lookup_ident(const std::string_view &ident);
    static std::string_view to_string_view(Token::Type token);
};

#undef FOREACH_TOKEN

#endif /* TOKEN_H */
