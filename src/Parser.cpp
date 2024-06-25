#include "..\header/Parser.h"
#include "..\header/Lexer.h"
#include "..\header/globals.h"
#include "..\header/ASTNodes.h"

using ASTNodePtr = std::shared_ptr<ASTNode>;

const std::string epsilon = "ε";

std::vector<std::string> long_keywords = {"ADD", "SUB", "MUL", "DIV", "MOD", "EQL", "DIF", "GRT", "LES", "GTE", "LTE", "AND", "OOR"};
std::vector<std::string> medium_keywords = {"INT", "REA", "CHR", "ATR", "NOT", "RPT"};
std::vector<std::string> short_keywords = {"OUT", "INP"};
std::vector<std::string> comp_keywords = {"BGN", "END", "IIF", "ELS", "LBL"};

bool is_long_keyword(const std::string &token)
{
    return std::find(long_keywords.begin(), long_keywords.end(), token) != long_keywords.end();
}

bool is_medium_keyword(const std::string &token)
{
    return std::find(medium_keywords.begin(), medium_keywords.end(), token) != medium_keywords.end();
}

bool is_short_keyword(const std::string &token)
{
    return std::find(short_keywords.begin(), short_keywords.end(), token) != short_keywords.end();
}

bool is_comp_keyword(const std::string &token)
{
    return std::find(comp_keywords.begin(), comp_keywords.end(), token) != comp_keywords.end();
}

bool isIdentifier(const std::string &token)
{
    return std::regex_match(token, std::regex(R"([a-zA-Z_][a-zA-Z0-9_]*)"));
}

bool isValue(const std::string &token)
{
    return std::regex_match(token, std::regex(R"([0-9]*\\.?[0-9]+)")) || isIdentifier(token) || std::regex_match(token, std::regex(R"('(\\.|[^\\'])')"));
}

ASTNodePtr parseProgram()
{
    auto program = std::make_shared<Program>();
    while (current_token.type != EOF_TOKEN)
    {
        program->instructions.push_back(parseInstruction());
    }
    return program;
}

ASTNodePtr parseInstruction()
{
    //std::cout << current_token.value << std::endl;
    if (is_long_keyword(current_token.value))
    {
        return parseLongInstruction();
    }
    else if (is_medium_keyword(current_token.value))
    {
        return parseMediumInstruction();
    }
    else if (is_short_keyword(current_token.value))
    {
        return parseShortInstruction();
    }
    else if (is_comp_keyword(current_token.value))
    {
        return parseCompoundInstruction();
    }
    else if(current_token.type == EOF_TOKEN)
    {
        return nullptr;
    }
    else
    {
        throw std::runtime_error("Unknown instruction: " + current_token.value);
    }
}

ASTNodePtr parseLongInstruction()
{
    ASTNodePtr keyword = std::make_shared<Keyword>(current_token.value);
    get_next_token();
    ASTNodePtr value1 = current_token.type == IDENTIFIER ? std::make_shared<Value>(std::make_shared<Identifier>(current_token.value)) : std::make_shared<Value>(std::make_shared<Number>(current_token.value));
    get_next_token();
    if (current_token.value != ",")
        throw std::runtime_error("Expected ','");
    get_next_token();
    ASTNodePtr value2 = current_token.type == IDENTIFIER ? std::make_shared<Value>(std::make_shared<Identifier>(current_token.value)) : std::make_shared<Value>(std::make_shared<Number>(current_token.value));
    get_next_token();
    if (current_token.value != ",")
        throw std::runtime_error("Expected ','");
    get_next_token();
    ASTNodePtr identifier = std::make_shared<Identifier>(current_token.value);
    get_next_token();
    return std::make_shared<LongInstruction>(keyword, value1, value2, identifier);
}

ASTNodePtr parseMediumInstruction()
{
    ASTNodePtr keyword = std::make_shared<Keyword>(current_token.value);
    get_next_token();
    ASTNodePtr identifier = std::make_shared<Identifier>(current_token.value);
    get_next_token();
    if (current_token.value != ",")
        throw std::runtime_error("Expected ','");
    get_next_token();
    ASTNodePtr value = current_token.type == IDENTIFIER ? std::make_shared<Value>(std::make_shared<Identifier>(current_token.value)) : std::make_shared<Value>(std::make_shared<Number>(current_token.value));
    get_next_token();
    return std::make_shared<MediumInstruction>(keyword, identifier, value);
}

ASTNodePtr parseShortInstruction()
{
    ASTNodePtr keyword = std::make_shared<Keyword>(current_token.value);
    get_next_token();

    if (!isValue(current_token.value))
        throw std::runtime_error("Expected value");
    ASTNodePtr identifier = std::make_shared<Identifier>(current_token.value);
    get_next_token();
    return std::make_shared<ShortInstruction>(keyword, identifier);
}

ASTNodePtr parseCompoundInstruction()
{
    ASTNodePtr keyword = std::make_shared<Keyword>(current_token.value);
    //std::cout << keyword << std::endl;
    get_next_token();
    if (!isIdentifier(current_token.value))
    {
        //std::cout << current_token.value << std::endl;
        throw std::runtime_error("1Expected identifier");
    }
    ASTNodePtr identifier = std::make_shared<Identifier>(current_token.value);
    get_next_token();

    auto tail = parseCompoundInstructionTail();
    //std::cout << tail << std::endl;
    //auto cp = std::make_shared<CompoundInstruction>(keyword, identifier, tail);
    
    //std::cout << cp << std::endl;

    return std::make_shared<CompoundInstruction>(keyword, identifier, tail);
}

ASTNodePtr parseCompoundInstructionTail()
{
    //std::cout << current_token.value << std::endl;
    if (is_comp_keyword(current_token.value))
    {
        ASTNodePtr keyword = std::make_shared<Keyword>(current_token.value);
        get_next_token();

        std::vector<ASTNodePtr> instructions;
        while (1)
        {
            instructions.push_back(parseInstruction());
            if (is_comp_keyword(current_token.value))
                break;
        }
        
        ASTNodePtr keyword2 = std::make_shared<Keyword>(current_token.value);

        if (!is_comp_keyword(current_token.value))
            throw std::runtime_error("Expected compound keyword");

        get_next_token();

        return std::make_shared<CompoundInstructionTail>(keyword, keyword2, nullptr, nullptr, instructions);
    }
    else if (current_token.value == ",")
    {
        get_next_token();
        ASTNodePtr identifier1 = std::make_shared<Identifier>(current_token.value);
        get_next_token();
        // << std::endl;
        if (!is_comp_keyword(current_token.value)){
                //std::cout << current_token.value << std::endl;
                return std::make_shared<CompoundInstructionTail>(nullptr, nullptr, identifier1, nullptr, std::vector<ASTNodePtr>());
        }
        ASTNodePtr keyword = std::make_shared<Keyword>(current_token.value);
        get_next_token();
        if (!isIdentifier(current_token.value))
            throw std::runtime_error("2Expected identifier");
        ASTNodePtr identifier2 = std::make_shared<Identifier>(current_token.value);
        get_next_token();
        auto tail = std::make_shared<CompoundInstructionTail>(keyword, nullptr, identifier1, identifier2, std::vector<ASTNodePtr>());
        return tail;
    }
    else
    {
        throw std::runtime_error("Unexpected token in compound instruction tail: " + current_token.value);
    }
}

ASTNodePtr parse()
{
    get_next_token();

    return parseProgram();
}