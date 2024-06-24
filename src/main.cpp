#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include "../header/codeGenerator.h"
#include "../header/Lexer.h"
#include "../header/Parser.h"
#include "../header/ASTNodes.h"
#include "../header/globals.h"

std::string tokenTypeToString(token_type type) {
    switch (type) {
        case KEYWORD: return "KEYWORD";
        case IDENTIFIER: return "IDENTIFIER";
        case NUMBER: return "NUMBER";
        case CHARACTER: return "CHARACTER";
        case COMMA: return "COMMA";
        case DOT: return "DOT";
        case QUOTE: return "QUOTE";
        case EOF_TOKEN: return "EOF_TOKEN";
        default: return "UNKNOWN";
    }
}

int main(int argc, char const *argv[])
{
    //check if the user has provided a file to compile
    if (argc < 2)
    {
        std::cerr << "No input file provided" << std::endl;
        std::cerr << "Usage: " << "ASBCompiler.exe" << " <input_file>" << std::endl;
        return 1;
    }

    //read the source code from the file
    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }

    //check if the file has the correct extension .asb
    if (std::string(argv[1]).substr(std::string(argv[1]).find_last_of('.')) != ".asb")
    {
        std::cerr << "Invalid file extension. File must have the extension .asb" << std::endl;
        return 1;
    }

    std::string source_code_test;
    std::string line;
    while (std::getline(file, line))
    {
        source_code_test += line + " ";
    }

    //tokenize the source code
    std::vector<token> tokens = tokanize(source_code_test);

    for (auto t : tokens)
    {
        std::cout << '<' << tokenTypeToString(t.type) << ", " << t.value << '>' << std::endl;
    }

    //now parse the tokens
    ASTNodePtr ast;
    try {
        ast = parse();
        std::cout << "Parsing completed successfully.\n";
        ast->print();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    std::ofstream codeFile;

    // check is a file name for out file is provided
    if (argc < 3)
    {
        codeFile.open("out.cpp", std::ios::out);
    }
    else
    {
        codeFile.open(argv[2], std::ios::out);
    }

    //generate the code
    try {
        generateCode(ast, codeFile);
        std::cout << "Code generation completed successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    //run the generated code in the terminal
    // system("g++ out.cpp -o out");
    // system(".\\out");

    //close the file
    codeFile.close();



    return 0;
}
