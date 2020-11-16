#pragma once

#include <vector>
#include <string>
#include <unordered_map>

//------------------- Debug mode switch

#ifdef DEBUG
#undef DEBUG
#endif

#define DEBUG false

#define DBG(expr) if (DEBUG) {expr;}

//------------------- All lexems present in grammar
enum lexems_t { ID, VALUE,

ASSIGN, ADD, SUB, MUL, DIV, LESS, GREATER, LESSEQ, GREQ, NOTEQUAL, EQUAL,

WHILE, IF, PRINT, IN,

LBRAC, RBRAC, RSBRAC, LSBRAC, SEMICOL, END, STMT,

};


struct Token {
public:
    int token_kind;
    std::string token_str;

    Token(int kind, const std::unordered_map<int, std::string>* map_tostr);
    Token(Token* token);
    Token() = default;
    //Token(int kind, std::string* word); // constructors for creating such identifiers values
    virtual void print() const;
};


struct Word:public Token {
    std::string m_word;

    Word(Token* token);
    Word(std::string word, int token_kind, const std::unordered_map<int, std::string>* map_tostr);
    //void print();
};

struct Value:public Token {
    int m_value;
    Value(int value, const std::unordered_map<int, std::string>* map_tostr);

    Value(Token* token);
    //void print();
};

std::vector<Token*> lexer(char* buf);

Token* parse_lexem(const std::string lexem, const std::unordered_map<std::string, int>* map);

std::vector<std::string> extract_lexems(char* buf);

std::unordered_map<std::string, int> set_token_types();

std::unordered_map<int, std::string> set_token_strings();


