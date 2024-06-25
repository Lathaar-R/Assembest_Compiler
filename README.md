# Assembest_Compiler
## Repositorio para o projeto do compilador da linguagem Assembest

Este repositório contém o código do compilador da linguagem "Assembest" da matéria ECOM06A (Compiladores) do curso Engenharia de Computação da Univercidade Federal de Itajubá.

## Linguagem
Assembest é uma linguagem de programação de baixo nível interpretada, baseada em Assembly, que possui instruções de controle de fluxo, operações aritméticas e lógicas, e operações de entrada e saída.



A Linguagem é uma gramática livre de contexto LL(1) e possui as seguintes regras:

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


## Parser

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

<!-- Não-terminal	long_keyword	medium_keyword	short_keyword	comp_keyword	,	$
program	instruction	instruction	instruction	instruction		ε
instruction	long_instruction	medium_instruction	short_instruction	compound_instruction		
long_instruction	long_keyword					
medium_instruction		medium_keyword				
short_instruction			short_keyword			
compound_instruction				comp_keyword		
compound_instruction_tail				comp_keyword program comp_keyword	',' identifier [comp_keyword identifier]	ε -->

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

