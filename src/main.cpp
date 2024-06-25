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

    file.close();

    //tokenize the source code
    std::vector<token> tokens = tokanize(source_code_test);


    // for (auto t : tokens)
    // {
    //     //std::cout << '<' << tokenTypeToString(t.type) << ", " << t.value << '>' << std::endl;
    // }

    //save the tokens to a file
    std::ofstream outputFile;
    std::string outputName = std::string(argv[1]).substr(0, std::string(argv[1]).find_last_of('.')) + "_tokens_comandos.txt";
    outputFile.open(outputName);
    outputFile << "Tokens and commands recognized: " << std::endl;
    outputFile << "Tokens: EBNF <token_type, token_value>" << std::endl;
    
    int i = 0;
    for (auto t : tokens)
    {
        outputFile << '<' << tokenTypeToString(t.type) << ", " << t.value << '>' << std::endl;
        i++;
    }

    

    //now parse the tokens
    ASTNodePtr ast;
    try {
        ast = parse();
        outputFile << "Abstract Syntax Tree (AST) generated: " << std::endl;
        //print the AST        
        ast->print(outputFile);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    outputFile.close();

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

    codeFile.close();

    //compile the generated code
    std::string compileCommand = "g++ -o out.exe out.cpp";
    system(compileCommand.c_str());

    //delete the generated code file
    std::remove("out.cpp");

    //close the file
    



    return 0;
}
