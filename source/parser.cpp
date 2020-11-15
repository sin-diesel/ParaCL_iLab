#include "parser.h"
#include "lexer.h"
#include <assert.h>
#include <iostream>

/* TODO: 1) add debug information to log.txt (num of lexem) 
        2) add dot file for tree
        3) upgrade connector to be simply a node
        4) connect parents */
static int next = 0;
#define CAST(pointer, what, to) pointer = static_cast<to>(what)
//--------------- get tokens tree head
Parse_tree_t::Parse_tree_t(std::vector<Token*> tokens)
{
    //head = get_stmts(tokens);
}

Node_t::Node_t(Token* token) {
    node_kind = token->token_kind;
    if (token->token_kind == ID) {
        m_token = new Word(token);
    } else if (token->token_kind == VALUE) {
        m_token = new Value(token);
    } else {
        m_token = new Token(token);
    }
}

BinOP_t::BinOP_t(Token* token):Node_t(token) {}

void Node_t::tree_ltraverse() {
    Node_t* left = get_left_node();
    Node_t* right = get_right_node();
    DBG(print());
    if (left != nullptr) {
        left->tree_ltraverse();
    }
    if (right != nullptr) {
        right->tree_ltraverse();
    }
}

Node_t* Node_t::get_left_node() const {
    return nullptr;
}

Node_t* Node_t::get_right_node() const {
    return nullptr;
}

Node_t* BinOP_t::get_left_node() const {
    return lhs;
}

Node_t* BinOP_t::get_right_node() const {
    return rhs;
}

void Node_t::print() {
    std::cout << "Node contains:";
    m_token->print();
    if (m_token->token_kind == ID) {
        Word* id = static_cast<Word*>(m_token);
        std::cout << "\t ID: " << id->m_word << "\n";
    } else if (m_token->token_kind == VALUE) {
        Value* id = static_cast<Value*>(m_token);
        std::cout << "\t Value: " << id->m_value << "\n";
    }
}

Node_t* get_expr(std::vector<Token*> tokens) {
    Node_t* ret_n = nullptr;
    Node_t* lhs = nullptr;
    BinOP_t* ret_b = nullptr;

    lhs = get_term(tokens);

    if (tokens[next]->token_kind == ADD || tokens[next]->token_kind == SUB) {
        ret_b = new BinOP_t(tokens[next]);
        ++next;
        //CAST(ret, ret, BinOP_t*);

        ret_b->lhs = lhs;
        ret_b->rhs = get_expr(tokens); // set right node
        return ret_b;
    } else {
        ret_n = lhs;
        return ret_n;
    }
}

Node_t* get_term(std::vector<Token*> tokens) {
    Node_t* ret_n = nullptr;
    Node_t* lhs = nullptr;
    BinOP_t* ret_b = nullptr;

    lhs = get_factor(tokens);

    if (tokens[next]->token_kind == MUL || tokens[next]->token_kind == DIV) {
        ret_b = new BinOP_t(tokens[next]);
        ++next;
        //CAST(ret, ret, BinOP_t*);

        ret_b->lhs = lhs;
        ret_b->rhs = get_term(tokens); // set right node
        return ret_b;
    } else {
        ret_n = lhs;
        return ret_n;
    }

}

Node_t* get_factor(std::vector<Token*> tokens) {
    Node_t* ret_n = nullptr;

    if (tokens[next]->token_kind == LBRAC) {
        ++next;
        ret_n = get_expr(tokens);
        return ret_n;
    } else {
        ret_n = new Node_t(tokens[next]);
        ++next;
        return ret_n;
    }

}

//--------------- get statement
// Lexem *Get_statement(std::vector<Lexem *> lexems)
// {
//     KeyWord *key = nullptr;
//     BinOP *assignment = nullptr;

//     //--------------- statement can be either a keyword, or assignment of ID to ID/value
//     switch (lexems[next]->token_kind)
//     {

//     case KEYWORD:
//     {
//         KeyWord *current_keyword = static_cast<KeyWord *>(lexems[next]);
//         key = new KeyWord(current_keyword->keyword_kind);

//         if (current_keyword->keyword_kind == WHILE || current_keyword->keyword_kind == IF)
//         {
//             ++next; // get next lexem
//             ++next;
//             key->lhs = Get_expression(lexems);
//             ++next;
//             key->rhs = Get_scope(lexems);
//         }
//         else if (current_keyword->keyword_kind == PRINT)
//         {
//             key->lhs = Get_ID(lexems);
//         }
//     }
//         return key;
//         break;
//     case ID:
//     {
//         //Decl* current_decl = static_cast<Decl*>(lexems[next]);
//         Lexem *id = Get_ID(lexems); // get the ID
//         CAST(Decl, id, decl);

//         CAST(BinOP, lexems[next], op);
//         assert(op->binop_kind == ASSIGN);
//         assignment = new BinOP(op->binop_kind); // then get the assignment operator
//         ++next;
//         assert(lexems[next]->token_kind != BINOP);

//         assignment->rhs = id;
//         assignment->lhs = Get_expression(lexems);

//         return assignment;
//     }
//     break;
//     default:
//         return nullptr;
//         break;
//     }
// }

// //--------------- either a expression or ID/VALUE
// Lexem *Get_term(std::vector<Lexem *> lexems)
// {
//     Lexem *lhs = nullptr;
//     Lexem *rhs = nullptr;

//     BinOP *lex = nullptr;
//     BinOP *op = nullptr;

//     op = new BinOP(CONNECTOR);
//     if (lexems[next]->token_kind == BRAC)
//     {
//         ++next;
//         lhs = Get_expression(lexems);
//         ++next;
//     }
//     else
//     {
//         if (lexems[next]->token_kind == ID)
//         {
//             lhs = Get_ID(lexems);
//         }
//         else if (lexems[next]->token_kind == VALUE)
//         {
//             lhs = Get_value(lexems);
//         }
//         else
//         {
//             fprintf(stdout, "Parser failed!\n");
//         }
//     }

//     op->lhs = lhs;
//     op->rhs = rhs;

//     return op;
// }

// Lexem *Get_mult(std::vector<Lexem *> lexems)
// {
//     Lexem *lhs = nullptr;
//     Lexem *rhs = nullptr;

//     BinOP *lex = nullptr;
//     BinOP *op = nullptr;

//     lhs = Get_term(lexems);
//     if (lexems[next]->token_kind == BINOP)
//     {
//         CAST(BinOP, lexems[next], lex)
//         op = new BinOP(lex->binop_kind);
//         ++next;
//         rhs = Get_mult(lexems);

//         // } else {
//         //}
//     }
//     else
//     {
//         op = new BinOP(CONNECTOR);
//     }
//     op->lhs = lhs;
//     op->rhs = rhs;

//     return op;
// }

// Lexem *Get_expression(std::vector<Lexem *> lexems)
// {

//     Lexem *lhs = nullptr;
//     Lexem *rhs = nullptr;
//     BinOP *op = nullptr;

//     BinOP *lex = nullptr;
//     DBG(lexems[next]->print();)

//     lhs = Get_mult(lexems);
//     //assert(lexems[next]->token_kind == BINOP);
//     if (lexems[next]->token_kind == BINOP)
//     {
//         lex = static_cast<BinOP *>(lexems[next]);
//         op = new BinOP(lex->binop_kind);
//         ++next;
//         rhs = Get_expression(lexems);
//     }
//     else
//     {
//         op = new BinOP(CONNECTOR);
//         rhs = nullptr;
//     }
//     op->lhs = lhs;
//     op->rhs = rhs;
//     return op;
// }

// //--------------- get identifier
// Lexem *Get_ID(std::vector<Lexem *> lexems)
// {
//     Decl *op = static_cast<Decl *>(lexems[next]);
//     std::string *identifier = op->decl_;
//     Decl *id = new Decl(identifier);
//     ++next;
//     return id;
// }

// //--------------- get scope
// Lexem *Get_scope(std::vector<Lexem *> lexems)
// {
//     ++next;
//     KeyWord *scope = new KeyWord(SCOPE);
//     scope->rhs = Get_statement(lexems);
//     if (lexems[next + 1]->token_kind != BRAC)
//     {
//         scope->lhs = Get_scope(lexems);
//     }
//     return scope;
// }

// //--------------- get integer value
// Lexem *Get_value(std::vector<Lexem *> lexems)
// {
//     CAST(Value, lexems[next], num);
//     int number = num->value;
//     Value *res = new Value(number);
//     ++next;
//     return res;
// }