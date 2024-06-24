#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <iostream>
#include <fstream>

#include "ASTNodes.h"

using ASTNodePtr = std::shared_ptr<ASTNode>;    

void generateCode(ASTNodePtr node, std::ofstream &out);


#endif // CODE_GENERATOR_H