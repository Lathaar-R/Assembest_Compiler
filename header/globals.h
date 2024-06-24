#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

enum token_type
{
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    CHARACTER,
    COMMA,
    DOT,
    QUOTE,
    EOF_TOKEN,
    TOTAL_TOKENS
};

struct token
{
    token_type type;
    std::string value;
};

std::string tokenTypeToString(token_type type);

#endif // GLOBALS_H