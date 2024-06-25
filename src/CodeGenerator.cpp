#include "../header/codeGenerator.h"
#include <sstream>

void generateProgram(ASTNodePtr node, std::stringstream &code);
void generateInstruction(ASTNodePtr node, std::stringstream &code);
void generateLongInstruction(ASTNodePtr node, std::stringstream &code);
void generateMediumInstruction(ASTNodePtr node, std::stringstream &code);
void generateShortInstruction(ASTNodePtr node, std::stringstream &code);
void generateCompoundInstruction(ASTNodePtr node, std::stringstream &code);
void generateCompoundInstructionTail(ASTNodePtr node, std::stringstream &code);
void generateValue(ASTNodePtr node, std::stringstream &code);
void generateIdentifier(ASTNodePtr node, std::stringstream &code);
void generateNumber(ASTNodePtr node, std::stringstream &code);
void generateCharacter(ASTNodePtr node, std::stringstream &code);
void generateKeyword(ASTNodePtr node, std::stringstream &code);

void generateCode(ASTNodePtr root, std::ofstream &outputFilename)
{
    if (!outputFilename.is_open())
    {
        throw std::runtime_error("Cannot open output file");
    }

    // create a string stream to store the generated code
    std::stringstream code;

    generateProgram(root, code);

    // write the generated code to the output file
    outputFilename << code.str();

    outputFilename.close();
}

void generateProgram(ASTNodePtr node, std::stringstream &out)
{
    out << "#include <iostream>\n";
    out << "using namespace std;\n";
    out << "int main() {\n";

    for (const auto &instr : std::dynamic_pointer_cast<Program>(node)->instructions)
    {
        generateInstruction(instr, out);
    }
    out << "    return 0;\n";
    out << "}\n";
}

void generateInstruction(ASTNodePtr node, std::stringstream &out)
{
    if (std::dynamic_pointer_cast<LongInstruction>(node))
    {
        generateLongInstruction(node, out);
    }
    else if (std::dynamic_pointer_cast<MediumInstruction>(node))
    {
        generateMediumInstruction(node, out);
    }
    else if (std::dynamic_pointer_cast<ShortInstruction>(node))
    {
        generateShortInstruction(node, out);
    }
    else if (std::dynamic_pointer_cast<CompoundInstruction>(node))
    {
        generateCompoundInstruction(node, out);
    }
    else
    {
        throw std::runtime_error("Unknown instruction");
    }
}

bool is_opration(const std::string &token)
{
    return token == "ADD" || token == "SUB" || token == "MUL" || token == "DIV" || token == "MOD";
}

bool is_comparison(const std::string &token)
{
    return token == "EQL" || token == "DIF" || token == "GRT" || token == "LES" || token == "GTE" || token == "LTE";
}

bool is_logical(const std::string &token)
{
    return token == "AND" || token == "OOR";
}

void generateLongInstruction(ASTNodePtr node, std::stringstream &out)
{
    auto instr = std::dynamic_pointer_cast<LongInstruction>(node);

    if (is_opration(std::dynamic_pointer_cast<Keyword>(instr->keyword)->keyword))
    {
        auto keyword = std::dynamic_pointer_cast<Keyword>(instr->keyword);
        auto value1 = std::dynamic_pointer_cast<Value>(instr->value1);
        auto value2 = std::dynamic_pointer_cast<Value>(instr->value2);
        auto identifier = std::dynamic_pointer_cast<Identifier>(instr->identifier);

        // find the value type
        // auto v_1 = typeid(*instr->value1) == typeid(Number)      ? std::dynamic_pointer_cast<Number>(instr->value1)->value
        //            : typeid(*instr->value1) == typeid(Character) ? std::to_string(static_cast<int>(std::dynamic_pointer_cast<Character>(instr->value1)->value) + 48)
        //                                                          : std::dynamic_pointer_cast<Identifier>(instr->value1)->value;

        // auto v_2 = typeid(*instr->value2) == typeid(Number)      ? std::dynamic_pointer_cast<Number>(instr->value2)->value
        //            : typeid(*instr->value2) == typeid(Character) ? std::to_string(static_cast<int>(std::dynamic_pointer_cast<Character>(instr->value2)->value) + 48)
        //                                                          : std::dynamic_pointer_cast<Identifier>(instr->value2)->value;

        std::string op = keyword->keyword == "ADD" ? "+" : keyword->keyword == "SUB" ? "-"
                                                       : keyword->keyword == "MUL"   ? "*"
                                                       : keyword->keyword == "DIV"   ? "/"
                                                                                     : "%";

        out << identifier->value << " = ";
        generateValue(value1->value, out);
        out << " " << op << " ";
        generateValue(value2->value, out);
        out << ";\n";
    }
    else if (is_comparison(std::dynamic_pointer_cast<Keyword>(instr->keyword)->keyword))
    {
        auto keyword = std::dynamic_pointer_cast<Keyword>(instr->keyword);
        auto value1 = std::dynamic_pointer_cast<Value>(instr->value1);
        auto value2 = std::dynamic_pointer_cast<Value>(instr->value2);
        auto identifier = std::dynamic_pointer_cast<Identifier>(instr->identifier);

        std::string op = keyword->keyword == "EQL" ? "==" : keyword->keyword == "DIF" ? "!="
                                                        : keyword->keyword == "GRT"   ? ">"
                                                        : keyword->keyword == "LES"   ? "<"
                                                        : keyword->keyword == "GTE"   ? ">="
                                                                                      : "<=";

        // find the value type
        // auto v_1 = typeid(*instr->value1) == typeid(Number)      ? std::dynamic_pointer_cast<Number>(instr->value1)->value
        //            : typeid(*instr->value1) == typeid(Character) ? std::to_string(static_cast<int>(std::dynamic_pointer_cast<Character>(instr->value1)->value) + 48)
        //                                                          : std::dynamic_pointer_cast<Identifier>(instr->value1)->value;

        // auto v_2 = typeid(*instr->value2) == typeid(Number)      ? std::dynamic_pointer_cast<Number>(instr->value2)->value
        //            : typeid(*instr->value2) == typeid(Character) ? std::to_string(static_cast<int>(std::dynamic_pointer_cast<Character>(instr->value2)->value) + 48)
        //                                                          : std::dynamic_pointer_cast<Identifier>(instr->value2)->value;

        // out << "if (" << v_1 << " " << op << " " << v_2 << ") {\n";
        // out << "    " << identifier->value << " = 1;\n";
        // out << "} else {\n";
        // out << "    " << identifier->value << " = 0;\n";
        // out << "}\n";

        out << "if (";
        generateValue(value1->value, out);
        out << " " << op << " ";
        generateValue(value2->value, out);
        out << ") {\n";
        out << "    " << identifier->value << " = 1;\n";
        out << "} else {\n";
        out << "    " << identifier->value << " = 0;\n";
        out << "}\n";
    }
    else if (is_logical(std::dynamic_pointer_cast<Keyword>(instr->keyword)->keyword))
    {
        auto keyword = std::dynamic_pointer_cast<Keyword>(instr->keyword);
        auto value1 = std::dynamic_pointer_cast<Value>(instr->value1);
        auto value2 = std::dynamic_pointer_cast<Value>(instr->value2);
        auto identifier = std::dynamic_pointer_cast<Identifier>(instr->identifier);

        // find the value type
        //std::cout << keyword->keyword << std::endl;
        auto op = keyword->keyword == "AND" ? "&&" : "||";

        // check what type is the value
        // std::cout << typeid(value1->value).name() << std::endl;
        // if (typeid(value1->value) == typeid(Identifier))
        // {
            // out << "if (" << v_1 << " " << op << " " << v_2 << ") {\n";
            out << "if (";
            generateValue(value1->value, out);
            out << " " << op << " ";
            generateValue(value2->value, out);
            out << ") {\n";
        // }
        // else
        // {
        //     auto b_1 = typeid(value1) == typeid(Character) ? static_cast<int>(std::dynamic_pointer_cast<Character>(instr->value1)->value) + 48
        //                                                            : std::stoi(std::dynamic_pointer_cast<Number>(instr->value1)->value);

        //     auto b_2 = typeid(value2) == typeid(Character) ? static_cast<int>(std::dynamic_pointer_cast<Character>(instr->value2)->value) + 48
        //                                                            : std::stoi(std::dynamic_pointer_cast<Number>(instr->value2)->value);

        //     auto p_1 = b_1 == 0 ? "false" : "true";
        //     auto p_2 = b_2 == 0 ? "false" : "true";

        //     out << "if (" << p_1 << " " << op << " " << p_2 << ") {\n";
        //     // out << "if (";
        //     // generateValue(instr->value1, out);
        //     // out << " " << op << " ";
        //     // generateValue(instr->value2, out);
        //     // out << ") {\n";
        // }

        out << "    " << identifier->value << " = 1;\n";
        out << "} else {\n";
        out << "    " << identifier->value << " = 0;\n";
        out << "}\n";
    }
}

void generateMediumInstruction(ASTNodePtr node, std::stringstream &out)
{
    auto instr = std::dynamic_pointer_cast<MediumInstruction>(node);

    auto keyword = std::dynamic_pointer_cast<Keyword>(instr->keyword);
    auto value = std::dynamic_pointer_cast<Value>(instr->value);
    auto identifier = std::dynamic_pointer_cast<Identifier>(instr->identifier);

    // find the value type
    // auto v = typeid(*instr->value) == typeid(Number) ? std::dynamic_pointer_cast<Number>(instr->value)->value
    //          : typeid(*instr->value) == typeid(Character) ? std::to_string(static_cast<int>(std::dynamic_pointer_cast<Character>(instr->value)->value) + 48)
    //          : std::dynamic_pointer_cast<Identifier>(instr->value)->value;

    if (keyword->keyword == "NOT")
    {
        auto v = typeid(*instr->value) == typeid(Number)      ? std::dynamic_pointer_cast<Number>(instr->value)->value
                 : typeid(*instr->value) == typeid(Character) ? std::to_string(static_cast<int>(std::dynamic_pointer_cast<Character>(instr->value)->value) + 48)
                                                              : std::dynamic_pointer_cast<Identifier>(instr->value)->value;
        if (typeid(*instr->value) == typeid(Number) || typeid(*instr->value) == typeid(Character))
        {
            v = std::stoi(v) == 0 ? "false" : "true";
        }

        out << "if (" << v << " ) {\n";
        out << "    " << identifier->value << " = 1;\n";
        out << "} else {\n";
        out << "    " << identifier->value << " = 0;\n";
        out << "}\n";
    }
    else if (keyword->keyword == "RPT")
    {
        // out << "while (" << v << " != 0) {\n";
        // out << identifier->value << "();\n";
        // out << "}\n";
        out << "while (";
        generateValue(value->value, out);
        out << " != 0) {\n";
        out << identifier->value << "();\n";
        out << "}\n";
    }
    else
    {
        // transform the ss to string
        std::string s = out.str();

        int pos = s.length();

        out.str("");
        //std::cout << keyword->keyword << std::endl;
        if (keyword->keyword != "ATR" && keyword->keyword != "NOT" && keyword->keyword != "RPT")
        {
            // find the point before the main function
            pos = s.find("int main() {");
        }

        out << s.substr(0, pos);
        
        generateKeyword(instr->keyword, out);
        out << " " << identifier->value << " = ";
        generateValue(value->value, out);
        out << ";\n";

        
        out << s.substr(pos, s.size());
    }
}

void generateShortInstruction(ASTNodePtr node, std::stringstream &out)
{
    auto instr = std::dynamic_pointer_cast<ShortInstruction>(node);

    auto keyword = std::dynamic_pointer_cast<Keyword>(instr->keyword);
    auto identifier = std::dynamic_pointer_cast<Identifier>(instr->identifier);

    if (keyword->keyword == "INP")
    {
        out << "std::cin >> " << identifier->value << ";\n";
    }
    else if (keyword->keyword == "OUT")
    {
        out << "std::cout << " << identifier->value << ";\n";
    }
}

void generateCompoundInstruction(ASTNodePtr node, std::stringstream &out)
{
    auto instr = std::dynamic_pointer_cast<CompoundInstruction>(node);

    auto keyword = std::dynamic_pointer_cast<Keyword>(instr->keyword);
    auto identifier = std::dynamic_pointer_cast<Identifier>(instr->identifier);
    auto instr_tail = std::dynamic_pointer_cast<CompoundInstructionTail>(instr->tail);

    if (keyword->keyword == "IIF")
    {
        //std::cout << identifier->value << std::endl;
        out << "if (" << identifier->value << " != 0) {\n";
        generateCompoundInstructionTail(instr_tail, out);
        out << "}\n";
    }
    else if (keyword->keyword == "LBL")
    {
        // transform the ss to string
        std::string s = out.str();

        // find the point before the main function
        size_t pos = s.find("int main() {");

        // insert the label before the main function
        out.str("");
        out << s.substr(0, pos) << "void " << identifier->value << "() {\n";
        generateCompoundInstructionTail(instr_tail, out);
        out << "}\n";

        // insert the main function after the label
        out << s.substr(pos, s.size());
    }
}

void generateCompoundInstructionTail(ASTNodePtr node, std::stringstream &out)
{
    auto instr = std::dynamic_pointer_cast<CompoundInstructionTail>(node);

    if (instr->identifier1 != nullptr)
    {
        auto identifier1 = std::dynamic_pointer_cast<Identifier>(instr->identifier1);
        //std::cout << identifier1->value << std::endl;
        out << identifier1->value << "();\n";
        if (instr->identifier2 != nullptr)
        {
            auto identifier2 = std::dynamic_pointer_cast<Identifier>(instr->identifier2);
            //std::cout << identifier2->value << std::endl;
            out << "} else {\n";
            out << identifier2->value << "();\n";
        }
    }
    else
    {
        for (const auto &instr : instr->instructions)
        {
            //std::cout << typeid(*instr).name() << std::endl;
            generateInstruction(instr, out);
        }
    }
}

void generateValue(ASTNodePtr node, std::stringstream &out)
{
    //std::cout << typeid(*node).name() << std::endl;
    if (std::dynamic_pointer_cast<Number>(node))
    {
        generateNumber(node, out);
    }
    else if (std::dynamic_pointer_cast<Identifier>(node))
    {
        generateIdentifier(node, out);
    }
    else if (std::dynamic_pointer_cast<Character>(node))
    {
        generateCharacter(node, out);
    }
    else if (std::dynamic_pointer_cast<Keyword>(node))
    {
        generateKeyword(node, out);
    }
    else
    {
        throw std::runtime_error("Unknown value");
    }
}

void generateIdentifier(ASTNodePtr node, std::stringstream &out)
{
    auto identifier = std::dynamic_pointer_cast<Identifier>(node);
    out << identifier->value;
}

void generateNumber(ASTNodePtr node, std::stringstream &out)
{
    auto number = std::dynamic_pointer_cast<Number>(node);
    out << number->value;
}

void generateCharacter(ASTNodePtr node, std::stringstream &out)
{
    auto character = std::dynamic_pointer_cast<Character>(node);
    out << character->value;
}

void generateKeyword(ASTNodePtr node, std::stringstream &out)
{
    auto keyword = std::dynamic_pointer_cast<Keyword>(node);
    auto k = keyword->keyword;

    if (k == "INT")
    {
        out << "int";
    }
    else if (k == "REA")
    {
        out << "double";
    }
    else if (k == "CHR")
    {
        out << "char";
    }
}
