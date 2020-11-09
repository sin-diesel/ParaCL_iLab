#pragma once

#include "lexer.h"

/* ParaCL grammar: 
   Terminals: ASSIGN, ADD, SUB, MUL, DIV, LESS, GREATER, LESEQ,
               GREQ, NOTEQUAL, EQUAL, WHILE, IF, PRINT, IN, LBRAC, RBRAC, RSBRAC, LSBRAC, SEMICOL, ID, VALUE, END
   Non-terminals: expr, stmt, stmts, factor
   Productions: expr -> expr {+, -} term | term
                term -> term {*, /} factor | factor
                factor -> {ID, VALUE} | ( expr )
                stmt -> ID = expr ;
                        | IF ( expr ) stmt
                        | WHILE (expr) stmt
                        | { stmts }
                stmts -> stmts stmt | end
   */


struct Parse_tree_t {
    Node_t* head;

    Parse_tree_t(std::vector<Token*> tokens);
};

struct Node_t {
    int node_kind;
    Token* token_;

    virtual Node_t* get_left_node() const;
    virtual Node_t* get_right_node() const;
    void tree_traverse_left();
    Node_t(Token* token);
    
};

struct BinOP_t:public Node_t {
    Node_t* lhs = nullptr;
    Node_t* rhs = nullptr;

    Node_t* get_left_node() const override;
    Node_t* get_right_node() const override;
};



Node_t* get_stmt(std::vector<Token*> tokens);

Node_t* get_expr(std::vector<Token*> tokens);

Node_t* get_stmts(std::vector<Token*> tokens);

Node_t* get_factor(std::vector<Token*> tokens);

// Token* get_mult(std::vector<Token*> lexems);

// Token* get_term(std::vector<Token*> lexems);

// Token* get_value(std::vector<Token*> lexems);
