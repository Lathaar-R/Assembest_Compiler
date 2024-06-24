// The lexer is supposed to take in a string containing an entire files worth of source code and spit out a list containing every token.
// The lexer will also be responsible for handling comments and whitespace.

#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <string>
#include <regex>
#include <unordered_set>
#include <queue>
#include "../header/globals.h"

extern const std::unordered_set<std::string> reserved_keywords;

extern std::map<token_type, std::regex> token_regexes;

extern token current_token;

extern int currentTokenIndex;

void get_next_token();

std::vector<token> tokanize(std::string &source_code);



#endif // LEXER_H