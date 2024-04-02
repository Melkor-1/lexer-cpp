#ifndef TOKEN_H
#define TOKEN_H 1

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

#define GEN_ENUM(ENUM) ENUM,

class Token {
    // FIXME: This shouldn't be a vector. How do we write an array without
    // providing a size?
    static const std::vector<std::string_view> token_strs;
    static int token_strs_count;

  public:
    enum class Type { FOREACH_TOKEN(GEN_ENUM) };

    Token::Type type;
    std::string lit;

    Token(Token::Type type, const std::string_view &lit) : type(type), lit(lit)
    {
    }
    static Token::Type lookup_ident(const std::string_view &ident);
    static std::string_view to_str(Token::Type t);
};

#undef GEN_ENUM

#endif /* TOKEN_H */
