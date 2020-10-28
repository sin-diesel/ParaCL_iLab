#include "lexer.h"
#include <string>
#include <string.h>
#include <iostream>
#include <assert.h>
#include <vector>
#include <unordered_map>

Lexem::Lexem(int kind) {
    token_kind = kind;
}

BinOP::BinOP(int kind):Lexem(BINOP) {
    binop_kind = kind;
}

void BinOP::print() const {

    fprintf(stdout, "\n\n\nBinOP lexem: %p\n\ttype: %d\n\n\n", this, binop_kind);
}

 //--------------- get tokens in vector
std::vector<std::string> extract_tokens(char* buf) {
    std::vector<std::string> tokens;

    //--------------- get single token
    char* beginning = strtok(buf, " ");
    int size = strlen(beginning);
    std::string token(beginning, size);

    DBG(std::cout << "Token parsed: " << token << std::endl);
    tokens.push_back(token);
    
     //--------------- get remaining tokens
    while ((beginning = strtok(NULL, " ")) != NULL) {
       int size = strlen(beginning);
        token = std::string(beginning, size);
        DBG(std::cout << "Token parsed: " << token << std::endl);
        tokens.push_back(token);
    }
    return tokens;
}

 //--------------- associate enum values with corresponding strings
std::unordered_map<std::string, int> set_operations() {
    std::unordered_map<std::string, int> operations;

    operations.insert({"+", ADD});
    operations.insert({"-", SUB});
    operations.insert({"*", MUL});
    operations.insert({"/", DIV});
    operations.insert({"=", ASSIGN});
    operations.insert({"==", EQUAL});
    operations.insert({"<", LESS});
    operations.insert({">", GREATER});
    operations.insert({"<=", LESSEQ});
    operations.insert({">=", GREQ});

    return operations;
}

 //--------------- parse token and return Lexem*
Lexem* parse_token(const std::string token, const std::unordered_map<std::string, int>& map) {
    BinOP* opt = nullptr;

    auto check = map.find(token);
    if (check == map.end()) {
        fprintf(stdout, "Token not recognized!\n");
        exit(-1);
    }

    int op_kind = map.at(token); // here we are sure that key exists
    BINOP_CREATE(opt, op_kind);

    assert(opt != nullptr);
    return opt;
}

//--------------- return vector of lexems
std::vector<Lexem*> lexer(char* buf) {
    unsigned size = strlen(buf);
    char* current = buf;

    std::vector<Lexem*> lexems;

    std::unordered_map<std::string, int> ops = set_operations();
    std::vector<std::string> tokens = extract_tokens(buf);

    for (int i = 0; i < tokens.size(); ++i) {
        Lexem* lex = parse_token(tokens[i], ops);
        lexems.push_back(lex);
    }

    return lexems;
}

