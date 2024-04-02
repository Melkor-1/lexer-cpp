#include "token.h"

#include <string_view>
#include <unordered_map>
#include <vector>

#define GEN_STRING(STRING) #STRING,

const std::vector<std::string_view> Token::token_names {
    FOREACH_TOKEN(GEN_STRING)
};

std::size_t Token::token_count{token_names.size()};

Token::Type Token::lookup_ident(const std::string_view &ident)
{
    static const std::unordered_map<std::string_view, Token::Type> keywords {
        {"fn", Token::Type::Function},   {"let", Token::Type::Let},
        {"true", Token::Type::True},     {"false", Token::Type::False},
        {"if", Token::Type::If},         {"else", Token::Type::Else},
        {"return", Token::Type::Return},
    };

    auto it {keywords.find(ident)};
    return it != keywords.end() ? it->second : Token::Type::Ident;
}

std::string_view Token::to_string_view(Token::Type token)
{
    // XXX: Is returning "" an okay decision?
    std::size_t index = static_cast<std::size_t> (token);
    return index < 0 || index > token_count ? "" : token_names[index];
}

#undef GEN_STRING
#undef FOREACH_TOKEN
