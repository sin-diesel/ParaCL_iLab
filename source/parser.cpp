#include "parser.h"
#include "lexer.h"
#include <assert.h>

static int next = 0;

Parse_tree_t::Parse_tree_t(std::vector<Lexem*> lexems) {
    head = Get_statement(lexems);
}

Lexem* Get_statement(std::vector<Lexem*> lexems) {
    Lexem* statement = nullptr;
    KeyWord* key = nullptr;
    
    switch (lexems[next]->token_kind) {

        case KEYWORD: {
            KeyWord* kword = static_cast<KeyWord*>(lexems[next]);

            if (kword->keyword_kind == WHILE || kword->keyword_kind == IF) {
                statement = new KeyWord(kword->keyword_kind);
                ++next; // get next lexem
                key = static_cast<KeyWord*>(statement);

                ++next;
                key->lhs = Get_expression(lexems);
                //assert(lexems[next]->token_kind == BRAC) // get next lexem
                ++next;
                key->rhs = Get_scope(lexems);
            } else if (kword->keyword_kind == PRINT) {

                statement = new KeyWord(kword->keyword_kind); // get next lexem
                key = static_cast<KeyWord*>(statement);
                key->lhs = Get_ID(lexems);
            }
        }
            break;
        case ID:
        break;
    }

    return statement;
}

Lexem* Get_term(std::vector<Lexem*> lexems) {
    Lexem* lhs = nullptr;
    Lexem* rhs = nullptr;

    BinOP* lex = nullptr;
    BinOP* op = nullptr;

    op = new BinOP(CONNECTOR);
    if (lexems[next]->token_kind == BRAC) {
        ++next;
        lhs = Get_expression(lexems);
        ++next;
    } else {
        if (lexems[next]->token_kind == ID) {
            lhs = Get_ID(lexems);
        } else if (lexems[next]->token_kind == VALUE) {
            lhs = Get_value(lexems);
        } else {
            fprintf(stdout, "Parser failed!\n");
        }
    }

    op->lhs = lhs;
    op->rhs = rhs;

    return op;
}

Lexem* Get_mult(std::vector<Lexem*> lexems) {
    Lexem* lhs = nullptr;
    Lexem* rhs = nullptr;

    BinOP* lex = nullptr;
    BinOP* op = nullptr;

    lhs = Get_term(lexems);
    if (lexems[next]->token_kind == BINOP) {
        CAST(BinOP, lexems[next], lex)
        op = new BinOP(lex->binop_kind);
        ++next;
        rhs = Get_mult(lexems);
        
       // } else {
        //}
    } else {
        op = new BinOP(CONNECTOR);
    }
    op->lhs = lhs;
    op->rhs = rhs;

    return op;
}

Lexem* Get_expression(std::vector<Lexem*> lexems) {
    
    Lexem* lhs = nullptr;
    Lexem* rhs = nullptr;
    BinOP* op = nullptr;

    BinOP* lex = nullptr;

    //assert(lexems[next]->token_kind == BRAC);
    //++next;
    DBG(lexems[next]->print();)

    // switch (lexems[next]->token_kind) {
        //case BRAC: {
            lhs = Get_mult(lexems);
            //assert(lexems[next]->token_kind == BINOP);
            if (lexems[next]->token_kind == BINOP) {
                lex = static_cast<BinOP*>(lexems[next]);
                op = new BinOP(lex->binop_kind);
                ++next;
                rhs = Get_expression(lexems);
            } else {
                op = new BinOP(CONNECTOR);
                rhs = nullptr;
            }
            op->lhs = lhs;
            op->rhs = rhs;
            //assert(lexems[next]->token_kind == BRAC);
     //   break;
      //  }
        //case ID: {
        //    lhs = Get_ID(lexems);
            //assert(lexems[next]->token_kind == BINOP);
        //    break;
       // }
        // break;
        // case VALUE:
        // break;
        // default:
        //     fprintf(stdout, "Parser failure!");
   // }
}

Lexem* Get_ID(std::vector<Lexem*> lexems) {
    Decl* op = static_cast<Decl*>(lexems[next]);
    std::string* identifier = op->decl_;
    Decl* id = new Decl(identifier);
    ++next;
    return id;
}

Lexem* Get_scope(std::vector<Lexem*> lexems) {
    KeyWord* scope = new KeyWord(SCOPE);
    scope->lhs = Get_statement(lexems);
    scope->rhs = nullptr;
    return scope;
}


Lexem* Get_value(std::vector<Lexem*> lexems) {
    CAST(Value, lexems[next], num);
    int number = num->value;
    Value* res = new Value(number);
    ++next;
    return res;
}