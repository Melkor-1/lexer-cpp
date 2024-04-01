#include <iostream>
#include <string>
#include <system_error>

#include "../lexer/lexer.hpp"
#include "../token/token.hpp"

int main()
{
    std::cout << "Hello! This is the Monkey Programming Language!\n"
              << "Type in commands:\n";

    while (true) {
        std::cout << ">> " << std::flush;
        std::string input{};

        if (!std::getline(std::cin, input)) {
            if (std::cin.bad()) {
                std::cerr
                    << "error: failed to read input: "
                    << std::error_code{errno, std::generic_category()}.message()
                    << ".\n";
                return EXIT_FAILURE;
            }
            break;
        }

        Lexer l{input};

        for (Token t{l.next()}; t.type != Token::Type::Eof; t = l.next()) {
            std::cout << "{Type: " << Token::to_str(t.type)
                      << ", Literal: " << t.lit << "}\n";
        }
    }
}
