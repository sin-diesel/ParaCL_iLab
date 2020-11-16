#include "parser.h"
#include "lexer.h"
#include <assert.h>
#include <iostream>

/* TODO: 1) add debug information to log.txt (num of lexem) 
        2) add dot file for tree
        3) upgrade connector to be simply a node
        4) connect parents */
int next = 0;

#define CAST(pointer, what, to) pointer = static_cast<to>(what)
//--------------- get tokens tree head
Parse_tree_t::Parse_tree_t(std::vector<Token*> tokens)
{
    head = get_stmts(tokens);
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

Node_t* get_cond(std::vector<Token*> tokens) {
    Node_t* ret_n = nullptr;
    Node_t* lhs = nullptr;
    BinOP_t* ret_b = nullptr;

    lhs = get_expr(tokens);

    switch(tokens[next]->token_kind) {
        case EQUAL:
        case NOTEQUAL:
        case LESS:
        case GREATER:
        case LESSEQ:
        case GREQ: {
            ret_b = new BinOP_t(tokens[next]);
            ++next;
            ret_b->lhs = lhs;
            ret_b->rhs = get_expr(tokens); // set right node
            return ret_b;
        }
        default: {
            fprintf(stdout, "Parsing condition error!");
            return nullptr;
        }
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

Node_t* get_stmt(std::vector<Token*> tokens) {
    Node_t* ret_n = nullptr;
    Node_t* lhs = nullptr;
    Node_t* rhs = nullptr;
    BinOP_t* ret_b = nullptr;

    switch(tokens[next]->token_kind) {
        case IF:
        case WHILE: {
            ret_b = new BinOP_t(tokens[next]);
            ++next;
            assert(tokens[next]->token_kind == LBRAC);
            ++next;

            lhs = get_cond(tokens);
            ++next;
            assert(tokens[next]->token_kind == LSBRAC);
            ++next;
            
            rhs = get_stmts(tokens);
            ret_b->lhs = lhs;
            ret_b->rhs = rhs;

            assert(ret_b != nullptr);
            return ret_b;
        }
        case ID: {
            lhs = new Node_t(tokens[next]);
            ++next;
            assert(tokens[next]->token_kind == ASSIGN);
             
            ret_b = new BinOP_t(tokens[next]);
            ++next;
            rhs = get_expr(tokens);

            assert(lhs != nullptr);
            assert(rhs != nullptr);

            ret_b->lhs = lhs;
            ret_b->rhs = rhs;
            return ret_b;
        }
        case PRINT: {

            ret_b = new BinOP_t(tokens[next]);
        
            rhs = nullptr;

            ++next;
            lhs = get_expr(tokens);
            assert(lhs != nullptr);

            ret_b->lhs = lhs;
            ret_b->rhs = rhs;
            return ret_b;
        }
        default:
            fprintf(stderr, "Lexer error!");
        return nullptr;
    }
}

Node_t* get_stmts(std::vector<Token*> tokens) {
    Node_t* ret_n = nullptr;
    Node_t* lhs = nullptr;
    Node_t* rhs = nullptr;
    BinOP_t* ret_b = nullptr;

    lhs = get_stmt(tokens);
    assert(tokens[next]->token_kind == SEMICOL || tokens[next]->token_kind == RSBRAC);
    DBG(fprintf(stdout, "Token array size: %d\n", tokens.size()))

    Token* stmt = new Token();

    stmt->token_kind = STMT;
    stmt->token_str = "Statement";

    if (next == tokens.size() - 1) {
        ret_b = new BinOP_t(stmt);
        ret_b->lhs = lhs;
        ret_b->rhs = nullptr;
        return ret_b;
    }

    ret_b = new BinOP_t(stmt);
    ++next;
    if (tokens[next]->token_kind == RSBRAC) {
        ret_b->lhs = lhs;
        ret_b->rhs = nullptr;
        return ret_b;
    } else {
        ret_b->lhs = lhs;
        ret_b->rhs = get_stmts(tokens);
        return ret_b;
    }
}