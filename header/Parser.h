// The parser is supposed to take in a list of tokens and spit out an abstract syntax tree.
// The parser will also be responsible for checking the syntax of the source code.
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include "../header/globals.h"
#include <variant>
#include <vector>

/*
program ::= instruction | instruction program | ε

instruction ::= long_instruction | medium_instruction | short_instruction | compound_instruction

long_instruction ::= long_keyword value ',' value ',' identifier

medium_instruction ::= medium_keyword identifier ',' value

short_instruction ::= short_keyword identifier

compound_instruction ::= comp_keyword identifier compound_instruction_tail

compound_instruction_tail ::= comp_keyword program comp_keyword
                            | ',' identifier [comp_keyword identifier]
                            | ε

long_keyword ::= "ADD"
medium_keyword ::= "INT"
short_keyword ::= "OUT" | "INP" 
comp_keyword ::= "BGN" | "END" | "IIF" | "ELS" | "LBL" 

value ::= number 
         | identifier 
         | character

number ::= ([0-9]+(\.[0-9]+)?)   

identifier ::= ([a-zA-Z_][a-zA-Z0-9_]*)

character ::= ('.')
*/

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <stack>
#include "ASTNodes.h"


using ASTNodePtr = std::shared_ptr<ASTNode>;

ASTNodePtr parseProgram();
ASTNodePtr parseInstruction();
ASTNodePtr parseLongInstruction();
ASTNodePtr parseMediumInstruction();
ASTNodePtr parseShortInstruction();
ASTNodePtr parseCompoundInstruction();
ASTNodePtr parseCompoundInstructionTail();

ASTNodePtr parse();


#endif // PARSER_H
