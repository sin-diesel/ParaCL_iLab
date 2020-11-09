#pragma once

#include <vector>
#include <string>
#include <unordered_map>

//------------------- Debug mode switch
#define DEBUG true


#define DBG(expr) if (DEBUG) {expr;}

//------------------- All lexems present in grammar
enum lexems_t { ID, VALUE,

ASSIGN, ADD, SUB, MUL, DIV, LESS, GREATER, LESSEQ, GREQ, NOTEQUAL, EQUAL,

WHILE, IF, PRINT, IN,

LBRAC, RBRAC, RSBRAC, LSBRAC, SEMICOL,

};

struct Token {
public:
    int token_kind;
    std::string* token_str;

    Token(int kind, const std::unordered_map<int, std::string>* map_tostr);
    virtual void print();
};

//     int token_kind;
//     Lexem* parent = nullptr;

//     Lexem() = delete;
//     Lexem(int kind);
//     virtual void print() const = 0;
//     virtual void print_dot(FILE* dot_file) = 0;
//     virtual int get_type() const = 0;
//     virtual ~Lexem() {};
// };

// struct BinOP: public Token {
//     int binop_kind;

//     BinOP(int binop);
// };

// struct KeyWord:public Lexem {
//     int keyword_kind;
//     Lexem* lhs = nullptr;
//     Lexem* rhs = nullptr;

//     KeyWord() = delete;
//     KeyWord(int keyword_kind);
//     void print() const override;
//     void print_dot(FILE* dot_file) override;
//     int get_type() const override;
// };

// struct Brack:public Lexem {
//     int brack_kind;

//     Brack() = delete;
//     Brack(int brack);
//     void print() const override;
//     void print_dot(FILE* dot_file) override;
//     int get_type() const override;
// };

struct Word:public Token {
    std::string* word_;
    Word(std::string* word, int token_kind, const std::unordered_map<int, std::string>* map_tostr);
    //void print();
};

struct Value:public Token {
    int value_;
    Value(int value, const std::unordered_map<int, std::string>* map_tostr);
    //void print();
};

std::vector<Token*> lexer(char* buf);

Token* parse_lexem(const std::string lexem, const std::unordered_map<std::string, int>* map);

std::vector<std::string> extract_lexems(char* buf);

std::unordered_map<std::string, int> set_token_types();

std::unordered_map<int, std::string> set_token_strings();

void unit_test_1();

void unit_test_2();

void unit_test_3();

void unit_test_4();

void unit_test_5();

void unit_test_6();

void unit_test_7();


