#pragma once
#include "lexer.h"

#define CAST(dst, src, val) dst* val = static_cast<dst*>(src);


struct Parse_tree_t {
    Lexem* head;

    Parse_tree_t(std::vector<Lexem*> lexems);
};

Lexem* Get_statement(std::vector<Lexem*> lexems);

Lexem* Get_expression(std::vector<Lexem*> lexems);

Lexem* Get_scope(std::vector<Lexem*> lexems);

Lexem* Get_ID(std::vector<Lexem*> lexems);

Lexem* Get_mult(std::vector<Lexem*> lexems);

Lexem* Get_term(std::vector<Lexem*> lexems);

Lexem* Get_value(std::vector<Lexem*> lexems);
