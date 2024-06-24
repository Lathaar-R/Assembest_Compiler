// Forward declarations
#ifndef ASTNODES_H
#define ASTNODES_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include "globals.h"


class ASTNode;
class Program;
class LongInstruction;
class MediumInstruction;
class ShortInstruction;
class CompoundInstruction;
class Value;
class Identifier;
class Number;
class Character;

using ASTNodePtr = std::shared_ptr<ASTNode>;

class ASTNode
{
public:
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;
};

inline void printIndent(int indent)
{
    for (int i = 0; i < indent; ++i)
        std::cout << "  ";
}

class Program : public ASTNode
{
public:
    std::vector<ASTNodePtr> instructions;
    void print(int indent = 0) const override
    {
        printIndent(indent);
        std::cout << "Program\n";
        for (const auto &instr : instructions)
        {
            instr->print(indent + 1);
        }
    }
};

class LongInstruction : public ASTNode
{
public:
    ASTNodePtr keyword;
    ASTNodePtr value1;
    ASTNodePtr value2;
    ASTNodePtr identifier;
    LongInstruction(ASTNodePtr kw, ASTNodePtr val1, ASTNodePtr val2, ASTNodePtr id)
        : keyword(kw), value1(val1), value2(val2), identifier(id) {}
    void print(int indent = 0) const override
    {
        printIndent(indent);
        std::cout << "LongInstruction:\n";
        keyword->print(indent + 1);
        value1->print(indent + 1);
        value2->print(indent + 1);
        identifier->print(indent + 1);
    }
};

class MediumInstruction : public ASTNode
{
public:
    ASTNodePtr keyword;
    ASTNodePtr identifier;
    ASTNodePtr value;
    MediumInstruction(ASTNodePtr kw, ASTNodePtr id, ASTNodePtr val)
        : keyword(kw), identifier(id), value(val) {}
    void print(int indent = 0) const override
    {
        printIndent(indent);
        std::cout << "MediumInstruction:\n";
        keyword->print(indent + 1);
        identifier->print(indent + 1);
        value->print(indent + 1);
    }
};

class ShortInstruction : public ASTNode
{
public:
    ASTNodePtr keyword;
    ASTNodePtr identifier;
    ShortInstruction(ASTNodePtr kw, ASTNodePtr id) : keyword(kw), identifier(id) {}
    void print(int indent = 0) const override
    {
        printIndent(indent);
        std::cout << "ShortInstruction:\n";
        keyword->print(indent + 1);
        identifier->print(indent + 1);

    }
};

class CompoundInstruction : public ASTNode
{
public:
    ASTNodePtr keyword;
    ASTNodePtr identifier;
    ASTNodePtr tail;
    CompoundInstruction(ASTNodePtr kw, ASTNodePtr id, ASTNodePtr t) : keyword(kw), identifier(id), tail(t) {}
    void print(int indent = 0) const override
    {
        printIndent(indent);
        std::cout << "CompoundInstruction:\n";
        keyword->print(indent + 1);
        identifier->print(indent + 1);
        tail->print(indent + 1);
    }
};

class CompoundInstructionTail : public ASTNode
{
public:
    ASTNodePtr keyword1;
    ASTNodePtr keyword2;
    ASTNodePtr identifier1;
    ASTNodePtr identifier2;
    std::vector<ASTNodePtr> instructions;
    CompoundInstructionTail(ASTNodePtr kw1, ASTNodePtr kw2, ASTNodePtr id1, ASTNodePtr id2, std::vector<ASTNodePtr> instrs)
        : keyword1(kw1), keyword2(kw2), identifier1(id1), identifier2(id2), instructions(instrs) {}
    void print(int indent = 0) const override
    {
        printIndent(indent);
        std::cout << "CompoundInstructionTail:\n";
        if (keyword1)
            keyword1->print(indent + 1);
        if (identifier1)
            identifier1->print(indent + 1);
        if (identifier2)
            identifier2->print(indent + 1);
        for (const auto &instr : instructions)
        {
            instr->print(indent + 1);
        }
        if (keyword2)
            keyword2->print(indent + 1);
    }
};

class Value : public ASTNode
{
public:
    ASTNodePtr value;
    Value(ASTNodePtr v) : value(v) {}
    void print(int indent = 0) const override
    {
        value->print(indent);
    }
};

class Identifier : public ASTNode
{
public:
    std::string value;
    Identifier(const std::string &n) : value(n) {}
    void print(int indent = 0) const override
    {
        printIndent(indent + 1);
        std::cout << "Identifier: " << value << "\n";
    }
};

class Number : public ASTNode
{
public:
    std::string value;
    Number(const std::string &num) : value(num) {}
    void print(int indent = 0) const override
    {
        printIndent(indent + 1);
        std::cout << "Number: " << value << "\n";
    }
};

class Character : public ASTNode
{
public:
    const char value;
    Character(const char &c) : value(c) {}
    void print(int indent = 0) const override
    {
        printIndent(indent + 1);
        std::cout << "Character: " << value << "\n";
    }
};

class Keyword : public ASTNode
{
public:
    std::string keyword;
    Keyword(const std::string &kw) : keyword(kw) {}
    void print(int indent = 0) const override
    {
        printIndent(indent + 1);
        std::cout << "Keyword: " << keyword << "\n";
    }
};

#endif // ASTNODES_H