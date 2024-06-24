#include "../header/Lexer.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_set>
#include <map> 
#include <string>
#include <queue>


std::vector<token> tokens;

std::map<token_type, std::regex> token_regexes = {
    {KEYWORD, std::regex(R"(([a-zA-Z]{3}))")},
    {IDENTIFIER, std::regex(R"(([a-zA-Z_][a-zA-Z0-9_]*))")},
    {NUMBER, std::regex(R"(([0-9]*\.?[0-9]+))")},
    {CHARACTER, std::regex(R"('(\\.|[^\\'])')")},
    {COMMA, std::regex(R"((,))")}
};

token current_token = {EOF_TOKEN, ""};
int currentTokenIndex = -1;

void get_next_token()
{
    currentTokenIndex++;
    if (currentTokenIndex < tokens.size())
    {
        current_token = tokens[currentTokenIndex];
    }
    else
    {
        current_token = {EOF_TOKEN, "$"};
    }
}

std::vector<token> tokanize(std::string & source_code)
{
    std::regex tokenPattern = std::regex(R"(([a-zA-Z]{3})|([a-zA-Z_][a-zA-Z0-9_]*)|([0-9]*\.?[0-9]+)|('(\\.|[^\\'])')|(,))");
    std::sregex_iterator it(source_code.begin(), source_code.end(), tokenPattern);
    std::sregex_iterator it_end;

    while (it != it_end)
    {
        token_type type;
        std::smatch match = *it;
        std::string token = match.str();
        if (std::regex_match(token, token_regexes[KEYWORD]) && reserved_keywords.count(token) > 0)
        {
            type = KEYWORD;
        }
        else if (std::regex_match(token, token_regexes[IDENTIFIER]))
        {
            type = IDENTIFIER;
        }
        else if (std::regex_match(token, token_regexes[NUMBER]))
        {
            type = NUMBER;
        }
        else if (std::regex_match(token, token_regexes[CHARACTER]))
        {
            type = CHARACTER;
        }
        else if (std::regex_match(token, token_regexes[COMMA]))
        {
            type = COMMA;
        }

        tokens.push_back({type, token});
        
        it++;
    }

    //put an EOF token at the end
    //tokens.push_back({EOF_TOKEN, "$"});

    //save the tokens to a file
    std::ofstream file;
    file.open("tokens.txt");
    int i = 0;
    for (auto t : tokens)
    {
        file << '<' << tokenTypeToString(t.type) << ", " << t.value << '>' << std::endl;
        i++;
    }
    file.close();

    return tokens;
}

const std::unordered_set<std::string> reserved_keywords = {
    "INT", "REA", "CHR", "ATR", "INP", "OUT", "ADD", "SUB", "MUL", "DIV", "MOD", "EQL", "DIF", "GRT", "LES", "GTE", "LTE", "AND", "OOR", "NOT", "LBL", "BGN", "END", "IIF", "ELS", "RPT"
};


