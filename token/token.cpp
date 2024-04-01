#include "token.hpp"

#include <string_view>
#include <unordered_map>
#include <vector>

#define GEN_STRING(STRING) #STRING,

const std::vector<std::string_view> Token::token_strs{
    FOREACH_TOKEN(GEN_STRING)};

int Token::token_strs_count{static_cast<int>(token_strs.size())};

Token::Type Token::lookup_ident(const std::string_view &ident)
{
    static const std::unordered_map<std::string_view, Token::Type> kws{
        {"fn", Token::Type::Function},   {"let", Token::Type::Let},
        {"true", Token::Type::True},     {"false", Token::Type::False},
        {"if", Token::Type::If},         {"else", Token::Type::Else},
        {"return", Token::Type::Return},
    };

    auto it{kws.find(ident)};
    return it != kws.end() ? it->second : Token::Type::Ident;
}

std::string_view Token::to_str(Token::Type t)
{
    // XXX: Is returning "" an okay decision?
    const int idx{static_cast<int>(t)};
    return idx < 0 || idx > token_strs_count ? "" : token_strs[idx];
}

#undef GEN_STRING
#undef FOREACH_TOKEN
