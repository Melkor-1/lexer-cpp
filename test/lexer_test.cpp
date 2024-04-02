#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stddef.h>
#include <string_view>
#include <vector>

#include "../lexer/lexer.h"
#include "../token/token.h"

static std::size_t tests_run = 0;

static void run_test(const std::string_view &input,
                     const std::vector<Token> &tests)
{
    ++tests_run;
    fprintf(stderr, "[INFO]: Running test suite %zu...\n", tests_run);
    Lexer l {input};

    for (std::size_t i {0}; i < std::size(tests); ++i) {
        const Token tt {tests[i]};
        const Token t {l.next()};

        if (t.type != tt.type) {
            std::cerr << "[FAIL]: tests[" << i + 1 << "] - Token::Type wrong. "
                      << "Expected '" << Token::to_str(tt.type) << "'. Got '"
                      << Token::to_str(t.type) << "'.\n",
                std::exit(EXIT_FAILURE);
        }

        /* The strncmp() is for illegal tokens; they are non-null terminated,
         * whilst the expected tokens are null-terminated.
         */
        if (t.lit != tt.lit &&
            (t.type == Token::Type::Illegal &&
             strncmp(t.lit.c_str(), tt.lit.c_str(), 1) == 0)) {
            std::cerr << "[FAIL]: tests[" << i + 1 << "] - Literal wrong. "
                      << "Expected '" << tt.lit << "'. Got '" << t.lit
                      << "'.\n",
                std::exit(EXIT_FAILURE);
        }
    }

    std::cerr << "[INFO]: All tests passed!\n";
}

int main()
{
    /* FIXME: These shouldn't need to be a vector either. */
    static const std::vector<Token> test_suite1 {
        {Token::Type::Assign, "="},  {Token::Type::Plus, "+"},
        {Token::Type::Lparen, "("},  {Token::Type::Rparen, ")"},
        {Token::Type::Lbrace, "{"},  {Token::Type::Rbrace, "}"},
        {Token::Type::Comma, ","},   {Token::Type::Semicolon, ";"},
        {Token::Type::Illegal, "#"}, {Token::Type::Eof, ""},
    };

    run_test("=+(){},;#", test_suite1);

    static const std::vector<Token> test_suite2 {
        {Token::Type::Let, "let"},     {Token::Type::Ident, "five"},
        {Token::Type::Assign, "="},    {Token::Type::Int, "5"},
        {Token::Type::Semicolon, ";"}, {Token::Type::Let, "let"},
        {Token::Type::Ident, "ten"},   {Token::Type::Assign, "="},
        {Token::Type::Int, "10"},      {Token::Type::Semicolon, ";"},
        {Token::Type::Let, "let"},     {Token::Type::Ident, "add"},
        {Token::Type::Assign, "="},    {Token::Type::Function, "fn"},
        {Token::Type::Lparen, "("},    {Token::Type::Ident, "x"},
        {Token::Type::Comma, ","},     {Token::Type::Ident, "y"},
        {Token::Type::Rparen, ")"},    {Token::Type::Lbrace, "{"},
        {Token::Type::Ident, "x"},     {Token::Type::Plus, "+"},
        {Token::Type::Ident, "y"},     {Token::Type::Semicolon, ";"},
        {Token::Type::Rbrace, "}"},    {Token::Type::Semicolon, ";"},
        {Token::Type::Let, "let"},     {Token::Type::Ident, "result"},
        {Token::Type::Assign, "="},    {Token::Type::Ident, "add"},
        {Token::Type::Lparen, "("},    {Token::Type::Ident, "five"},
        {Token::Type::Comma, ","},     {Token::Type::Ident, "ten"},
        {Token::Type::Rparen, ")"},    {Token::Type::Semicolon, ";"},
        {Token::Type::Eof, ""},
    };

    run_test("let five = 5;"
             "let ten = 10;"
             "let add = fn(x, y) { x + y; };"
             "let result = add(five, ten);",
             test_suite2);

    static const std::vector<Token> test_suite3 {
        {Token::Type::Let, "let"},
        {Token::Type::Ident, "five"},
        {Token::Type::Assign, "="},
        {Token::Type::Int, "5"},
        {Token::Type::Semicolon, ";"},
        {Token::Type::Let, "let"},
        {Token::Type::Ident, "ten"},
        {Token::Type::Assign, "="},
        {Token::Type::Int, "10"},
        {Token::Type::Semicolon, ";"},
        {Token::Type::Let, "let"},
        {Token::Type::Ident, "add"},
        {Token::Type::Assign, "="},
        {Token::Type::Function, "fn"},
        {Token::Type::Lparen, "("},
        {Token::Type::Ident, "x"},
        {Token::Type::Comma, ","},
        {Token::Type::Ident, "y"},
        {Token::Type::Rparen, ")"},
        {Token::Type::Lbrace, "{"},
        {Token::Type::Ident, "x"},
        {Token::Type::Plus, "+"},
        {Token::Type::Ident, "y"},
        {Token::Type::Semicolon, ";"},
        {Token::Type::Rbrace, "}"},
        {Token::Type::Semicolon, ";"},
        {Token::Type::Let, "let"},
        {Token::Type::Ident, "result"},
        {Token::Type::Assign, "="},
        {Token::Type::Ident, "add"},
        {Token::Type::Lparen, "("},
        {Token::Type::Ident, "five"},
        {Token::Type::Comma, ","},
        {Token::Type::Ident, "ten"},
        {Token::Type::Rparen, ")"},
        {Token::Type::Semicolon, ";"},
        {Token::Type::Bang, "!"},
        {Token::Type::Minus, "-"},
        {Token::Type::Slash, "/"},
        {Token::Type::Asterisk, "*"},
        {Token::Type::Int, "5"},
        {Token::Type::Semicolon, ";"},
        {Token::Type::Int, "5"},
        {Token::Type::Lt, "<"},
        {Token::Type::Int, "10"},
        {Token::Type::Gt, ">"},
        {Token::Type::Int, "5"},
        {Token::Type::Semicolon, ";"},
        {Token::Type::If, "if"},
        {Token::Type::Lparen, "("},
        {Token::Type::Int, "5"},
        {Token::Type::Lt, "<"},
        {Token::Type::Int, "10"},
        {Token::Type::Rparen, ")"},
        {Token::Type::Lbrace, "{"},
        {Token::Type::Return, "return"},
        {Token::Type::True, "true"},
        {Token::Type::Semicolon, ";"},
        {Token::Type::Rbrace, "}"},
        {Token::Type::Else, "else"},
        {Token::Type::Lbrace, "{"},
        {Token::Type::Return, "return"},
        {Token::Type::False, "false"},
        {Token::Type::Semicolon, ";"},
        {Token::Type::Rbrace, "}"},
        {Token::Type::Int, "10"},
        {Token::Type::Eq, "=="},
        {Token::Type::Int, "10"},
        {Token::Type::Semicolon, ";"},
        {Token::Type::Int, "10"},
        {Token::Type::Not_eq, "!="},
        {Token::Type::Int, "9"},
        {Token::Type::Semicolon, ";"},
        {Token::Type::String, "\"foobar\""},
        {Token::Type::String, "\" foo bar \""},
        {Token::Type::Lbracket, "["},
        {Token::Type::Int, "1"},
        {Token::Type::Comma, ","},
        {Token::Type::Int, "2"},
        {Token::Type::Rbracket, "]"},
        {Token::Type::Semicolon, ";"},
        {Token::Type::Lbrace, "{"},
        {Token::Type::String, "\"foo\""},
        {Token::Type::Colon, ":"},
        {Token::Type::String, "\"bar\""},
        {Token::Type::Rbrace, "}"},
        {Token::Type::Eof, ""},
    };

    run_test("let five = 5;"
             " let ten = 10;"
             "let add = fn(x, y) { x + y; };"
             "let result = add(five, ten);"
             "!-/*5;"
             "5 < 10 > 5;"
             "if (5 < 10) {"
             "return true;"
             "} else {"
             "return false;"
             "}"
             "10 == 10;"
             "10 != 9;"
             "\"foobar\""
             "\" foo bar \""
             "[1, 2];"
             "{\"foo\": \"bar\"}",
             test_suite3);
}
