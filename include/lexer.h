#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#define DEBUG true

#define BINOP_CREATE(pointer, type) pointer = new BinOP(type);
#define KWORD_CREATE(pointer, type) pointer = new KeyWord(type); 
#define BRAC_CREATE(pointer, type) pointer = new Brack(type); 

#define DBG(expr) if (DEBUG) {expr;}

enum tokens_t { BINOP, KEYWORD, ID, VALUE, CMP, LITERAL, SCOPE, BRAC,

 ASSIGN, ADD, SUB, MUL, DIV, LESS, GREATER, LESSEQ, GREQ, NOTEQUAL, EQUAL,

 WHILE, IF, PRINT, IN, SEMICOL,

 SCOPE_BEG, SCOPE_END, LBRAC, RBRAC,
 CONNECTOR };

struct Lexem {
public:
    int token_kind;
    Lexem* parent = nullptr;

    Lexem() = delete;
    Lexem(int kind);
    virtual void print() const = 0;
    virtual ~Lexem() {};
};

struct BinOP: public Lexem {
    int binop_kind;
    Lexem* lhs = nullptr;
    Lexem* rhs = nullptr;

    BinOP() = delete;
    BinOP(int binop);
    void print() const override;
};

struct KeyWord:public Lexem {
    int keyword_kind;
    Lexem* lhs = nullptr;
    Lexem* rhs = nullptr;

    KeyWord() = delete;
    KeyWord(int keyword_kind);
    void print() const override;
};

struct Brack:public Lexem {
    int brack_kind;

    Brack() = delete;
    Brack(int brack);
    void print() const override;
};

struct Decl:public Lexem {
    std::string* decl_;

    Decl() = delete;
    Decl(std::string* decl);
    void print() const override;
};

struct Value:public Lexem {
    int value;

    Value() = delete;
    Value(int val);
    void print() const override;
};

std::vector<Lexem*> lexer(char* buf);

Lexem* parse_token(const std::string token, const std::unordered_map<std::string, int>& map);

std::vector<std::string> extract_tokens(char* buf);

std::unordered_map<std::string, int> set_operations();

void unit_test_1();

void unit_test_2();

void unit_test_3();

void unit_test_4();

void unit_test_5();

void unit_test_6();

void unit_test_7();


