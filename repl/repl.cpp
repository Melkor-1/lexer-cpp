#include <iostream>
#include <string>
#include <system_error>

#include "../lexer/lexer.h"
#include "../token/token.h"

int main()
{
    std::cout << "Hello! This is the Monkey Programming Language!\n"
              << "Type in commands:\n";

    while (true) {
        std::cout << ">> " << std::flush;
        std::string input {};

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

        Lexer lexer {input};

        for (Token token {lexer.next()}; token.type != Token::Type::Eof; token = lexer.next()) {
            std::cout << "{Type: " << Token::to_string_view(token.type)
                      << ", Literal: " << token.literal << "}\n";
        }
    }
}
