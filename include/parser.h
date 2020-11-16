#pragma once

#include "lexer.h"


#ifdef DEBUG
#undef DEBUG
#endif

#define DEBUG false

/* ParaCL grammar: 
   Terminals: ASSIGN, ADD, SUB, MUL, DIV, LESS, GREATER, LESEQ,
               GREQ, NOTEQUAL, EQUAL, WHILE, IF, PRINT, IN, LBRAC, RBRAC, RSBRAC, LSBRAC, SEMICOL, ID, VALUE, END
   Non-terminals: expr, stmt, stmts, factor, term
   Productions: cond -> expr {==, <, >, !=, <=, >=} expr
                expr -> term {+, -} expr | term
                term -> factor {*, /} term | factor
                factor -> ( expr ) | {ID, VALUE} 
                stmt -> ID = expr ;
                        | IF ( cond )  { stmts
                        | WHILE ( cond ) { stmts
                        | PRINT ID
                        | IN
                stmts -> stmt } | stmts
   */



struct Node_t {
    int node_kind;
    Token* m_token;

    virtual Node_t* get_left_node() const;
    virtual Node_t* get_right_node() const;
    virtual void print();
    void tree_ltraverse();
    Node_t(Token* token);
    
};

struct Parse_tree_t {
    Node_t* head = nullptr;

    Parse_tree_t() = default;
    Parse_tree_t(std::vector<Token*> tokens);
};

struct BinOP_t:public Node_t {
    Node_t* lhs = nullptr;
    Node_t* rhs = nullptr;

    BinOP_t(Token* token);
    Node_t* get_left_node() const override;
    Node_t* get_right_node() const override;
};



Node_t* get_stmt(std::vector<Token*> tokens);

Node_t* get_expr(std::vector<Token*> tokens);

Node_t* get_stmts(std::vector<Token*> tokens);

Node_t* get_factor(std::vector<Token*> tokens);

Node_t* get_cond(std::vector<Token*> tokens);

// Token* get_mult(std::vector<Token*> lexems);

Node_t* get_term(std::vector<Token*> tokens);

// Token* get_value(std::vector<Token*> lexems);
