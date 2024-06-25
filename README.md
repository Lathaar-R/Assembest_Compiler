# Assembest_Compiler
## Repositorio para o projeto do compilador da linguagem Assembest

Este repositório contém o código do compilador da linguagem "Assembest" da matéria ECOM06A (Compiladores) do curso Engenharia de Computação da Univercidade Federal de Itajubá.

## Utilização
Baixe o repositório e rode o executável do compilador ASBCompiler.exe [arquivo_de_entrada.asb]. O compilador irá gerar um arquivo de saída executável "out.exe" e o "rquivo_de_entrada_tokens_comandos.txt".

## Linguagem
Assembest é uma linguagem de programação de baixo nível interpretada, baseada em Assembly, que possui instruções de controle de fluxo, operações aritméticas e lógicas, e operações de entrada e saída. O compilador da linguagem Assembest foi programada em C++ e gera código intermediário em C++.

A linguagem foi buildada utilizando a ferramenta CMake gerando um executável para o compilador.

# Gramática

A Linguagem é uma gramática livre de contexto.

## Lexer

O Analizador Lexico foi criado usando regex do C++ e possui as seguintes regras:

```cpp
    {KEYWORD, std::regex(R"(([a-zA-Z]{3}))")},
    {IDENTIFIER, std::regex(R"(([a-zA-Z_][a-zA-Z0-9_]*))")},
    {NUMBER, std::regex(R"(([0-9]*\.?[0-9]+))")},
    {CHARACTER, std::regex(R"('(\\.|[^\\'])')")},
    {COMMA, std::regex(R"((,))")}
```


## Parser

O analizador sintático foi criado usando LL(1) (recursão a esquerda com lookahead de 1) com uma descending AST (Abstract Syntax Tree).
A analize é feita com um analizador sintático preditivo não recursivo, usando recursão como stack do analizador.

```
program ::= instruction | instruction program | ε
instruction ::= long_instruction | medium_instruction | short_instruction | compound_instruction
long_instruction ::= long_keyword value ',' value ',' identifier
medium_instruction ::= medium_keyword identifier ',' value
short_instruction ::= short_keyword identifier
compound_instruction ::= comp_keyword identifier compound_instruction_tail
compound_instruction_tail ::= comp_keyword program comp_keyword | ',' identifier [comp_keyword identifier] | ε
long_keyword ::= LONG
medium_keyword ::= MID
short_keyword ::= SHOR
comp_keyword ::= COMP
value ::= number | identifier | character
number ::= ([0-9]+(\.[0-9]+)?)
identifier ::= ([a-zA-Z_][a-zA-Z0-9_]*)
character ::= ('.')
```

### Conjuntos First e Follow

First(program) = {LONG, MID, SHOR, COMP, ε}

First(instruction) = {LONG, MID, SHOR, COMP}

First(long_instruction) = {LONG}

First(medium_instruction) = {MID}

First(short_instruction) = {SHOR}

First(compound_instruction) = {COMP}

First(compound_instruction_tail) = {COMP, ',', ε}


Follow(program) = {$}

Follow(instruction) = Follow(program) = {$, LONG, MID, SHOR, COMP}

Follow(long_instruction) = Follow(instruction) = {$, LONG, MID, SHOR, COMP}

Follow(medium_instruction) = Follow(instruction) = {$, LONG, MID, SHOR, COMP}

Follow(short_instruction) = Follow(instruction) = {$, LONG, MID, SHOR, COMP}

Follow(compound_instruction) = Follow(instruction) = {$, LONG, MID, SHOR, COMP}

Follow(compound_instruction_tail) = {COMP, $}


### Tabela de Parsing

|               | LONG          | MID           | SHOR          | COMP          | ,             | $             |
|---------------|---------------|---------------|---------------|---------------|---------------|---------------|
| program       | instruction   | instruction   | instruction   | instruction   | ε             | ε             |
| instruction   | long_instruction | medium_instruction | short_instruction | compound_instruction |               |               |
| long_instruction | long_keyword |               |               |               |               |               |
| medium_instruction |               | medium_keyword |               |               |               |               |
| short_instruction |               |               | short_keyword |               |               |               |
| compound_instruction |               |               |               | comp_keyword  |               |               |
| compound_instruction_tail |               |               |               | comp_keyword program comp_keyword | ',' identifier [comp_keyword identifier] | ε |

# Funcionamento
O programa a ser compilado passa por 3 etapas: Análise Léxica, Análise Sintática e Geração de Código.

## Análise Léxica
A análise léxica é feita por um analisador léxico que lê o arquivo de entrada e gera uma lista de tokens. Cada token é um par de um tipo e um valor. O analisador léxico é implementado em C++ e utiliza expressões regulares para identificar os tokens.

## Análise Sintática
A análise sintática é feita por um analisador sintático LL(1) preditivo não recursivo. O analisador sintático lê a lista de tokens gerada pela análise léxica e gera uma árvore sintática abstrata (AST). A AST é uma representação da estrutura do programa em forma de árvore. O analisador sintático é implementado em C++.

## Geração de Código
A geração de código é feita a partir da AST gerada pela análise sintática. A AST é percorrida em pré-ordem e cada nó é traduzido para código C++. O código gerado é salvo em um arquivo de saída.