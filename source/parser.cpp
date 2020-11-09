#include "parser.h"
#include "lexer.h"
#include <assert.h>

/* TODO: 1) add debug information to log.txt (num of lexem) 
        2) add dot file for tree
        3) upgrade connector to be simply a node
        4) connect parents */

static int next = 0;

//--------------- get lexems tree head
Parse_tree_t::Parse_tree_t(std::vector<Lexem *> lexems)
{
    head = Get_statement(lexems);
}

//--------------- get statement
Lexem *Get_statement(std::vector<Lexem *> lexems)
{
    KeyWord *key = nullptr;
    BinOP *assignment = nullptr;

    //--------------- statement can be either a keyword, or assignment of ID to ID/value
    switch (lexems[next]->token_kind)
    {

    case KEYWORD:
    {
        KeyWord *current_keyword = static_cast<KeyWord *>(lexems[next]);
        key = new KeyWord(current_keyword->keyword_kind);

        if (current_keyword->keyword_kind == WHILE || current_keyword->keyword_kind == IF)
        {
            ++next; // get next lexem
            ++next;
            key->lhs = Get_expression(lexems);
            ++next;
            key->rhs = Get_scope(lexems);
        }
        else if (current_keyword->keyword_kind == PRINT)
        {
            key->lhs = Get_ID(lexems);
        }
    }
        return key;
        break;
    case ID:
    {
        //Decl* current_decl = static_cast<Decl*>(lexems[next]);
        Lexem *id = Get_ID(lexems); // get the ID
        CAST(Decl, id, decl);

        CAST(BinOP, lexems[next], op);
        assert(op->binop_kind == ASSIGN);
        assignment = new BinOP(op->binop_kind); // then get the assignment operator
        ++next;
        assert(lexems[next]->token_kind != BINOP);

        assignment->rhs = id;
        assignment->lhs = Get_expression(lexems);

        return assignment;
    }
    break;
    default:
        return nullptr;
        break;
    }
}

//--------------- either a expression or ID/VALUE
Lexem *Get_term(std::vector<Lexem *> lexems)
{
    Lexem *lhs = nullptr;
    Lexem *rhs = nullptr;

    BinOP *lex = nullptr;
    BinOP *op = nullptr;

    op = new BinOP(CONNECTOR);
    if (lexems[next]->token_kind == BRAC)
    {
        ++next;
        lhs = Get_expression(lexems);
        ++next;
    }
    else
    {
        if (lexems[next]->token_kind == ID)
        {
            lhs = Get_ID(lexems);
        }
        else if (lexems[next]->token_kind == VALUE)
        {
            lhs = Get_value(lexems);
        }
        else
        {
            fprintf(stdout, "Parser failed!\n");
        }
    }

    op->lhs = lhs;
    op->rhs = rhs;

    return op;
}

Lexem *Get_mult(std::vector<Lexem *> lexems)
{
    Lexem *lhs = nullptr;
    Lexem *rhs = nullptr;

    BinOP *lex = nullptr;
    BinOP *op = nullptr;

    lhs = Get_term(lexems);
    if (lexems[next]->token_kind == BINOP)
    {
        CAST(BinOP, lexems[next], lex)
        op = new BinOP(lex->binop_kind);
        ++next;
        rhs = Get_mult(lexems);

        // } else {
        //}
    }
    else
    {
        op = new BinOP(CONNECTOR);
    }
    op->lhs = lhs;
    op->rhs = rhs;

    return op;
}

Lexem *Get_expression(std::vector<Lexem *> lexems)
{

    Lexem *lhs = nullptr;
    Lexem *rhs = nullptr;
    BinOP *op = nullptr;

    BinOP *lex = nullptr;
    DBG(lexems[next]->print();)

    lhs = Get_mult(lexems);
    //assert(lexems[next]->token_kind == BINOP);
    if (lexems[next]->token_kind == BINOP)
    {
        lex = static_cast<BinOP *>(lexems[next]);
        op = new BinOP(lex->binop_kind);
        ++next;
        rhs = Get_expression(lexems);
    }
    else
    {
        op = new BinOP(CONNECTOR);
        rhs = nullptr;
    }
    op->lhs = lhs;
    op->rhs = rhs;
    return op;
}

//--------------- get identifier
Lexem *Get_ID(std::vector<Lexem *> lexems)
{
    Decl *op = static_cast<Decl *>(lexems[next]);
    std::string *identifier = op->decl_;
    Decl *id = new Decl(identifier);
    ++next;
    return id;
}

//--------------- get scope
Lexem *Get_scope(std::vector<Lexem *> lexems)
{
    ++next;
    KeyWord *scope = new KeyWord(SCOPE);
    scope->rhs = Get_statement(lexems);
    if (lexems[next + 1]->token_kind != BRAC)
    {
        scope->lhs = Get_scope(lexems);
    }
    return scope;
}

//--------------- get integer value
Lexem *Get_value(std::vector<Lexem *> lexems)
{
    CAST(Value, lexems[next], num);
    int number = num->value;
    Value *res = new Value(number);
    ++next;
    return res;
}