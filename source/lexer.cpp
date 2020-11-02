#include "../include/lexer.h"
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

KeyWord::KeyWord(int kind):Lexem(KEYWORD) {
    keyword_kind = kind;
}

Brack::Brack(int kind):Lexem(BRAC) {
    brack_kind = kind;
}

Decl::Decl(std::string* decl):Lexem(ID) {
    decl_ = decl;
}

void BinOP::print() const {
    fprintf(stdout, "\nBinOP lexem: %p\n\ttype: %d\n", this, binop_kind); // fix
}

void KeyWord::print() const {
    fprintf(stdout, "\nKeyWord lexem: %p\n\ttype: %d\n", this, keyword_kind); // fix
}

void Brack::print() const {
    fprintf(stdout, "\nBrack lexem: %p\n\ttype: %d\n", this, brack_kind); // fix
}

void Decl::print() const {
    fprintf(stdout, "\nDecl lexem: %p\n\ttype: %d\n", this, ID); // fix
    std::cout << "Decl is: " << *decl_ << "\n";
}

 //--------------- get tokens in vector
std::vector<std::string> extract_tokens(char* buf) {
    std::vector<std::string> tokens;

    //--------------- get single token
    char* beginning = strtok(buf, " \n");
    int size = strlen(beginning);
    std::string token(beginning, size);

    DBG(std::cout << "Token parsed: " << token << std::endl);
    tokens.push_back(token);
    
     //--------------- get remaining tokens
    while ((beginning = strtok(NULL, " \n")) != NULL) {
        int size = strlen(beginning);
        token = std::string(beginning, size);
        char s = token.find(';');   // check for semicolon at the end of a token
        char lbr = token.find('('); // check for round brackets
        char rbr = token.find(')');

        if (s != -1 && token.size() > 1) {
            token = token.substr(0, token.size() - 1);
            std::string semicolon = std::string(";");
            DBG(std::cout << "Token parsed: " << token << std::endl);
            DBG(std::cout << "Token parsed: " << semicolon << std::endl);
            tokens.push_back(token);
            tokens.push_back(semicolon);
        } else if (lbr != -1 && token.size() > 1) {
            token = token.substr(1, token.size() - 1);
            std::string lbrac = std::string("(");
            DBG(std::cout << "Token parsed: " << lbrac << std::endl);
            DBG(std::cout << "Token parsed: " << token << std::endl);
            tokens.push_back(lbrac);
            tokens.push_back(token);
        } else if (rbr != -1 && token.size() > 1) {
            token = token.substr(0, token.size() - 1);
            std::string rbrac = std::string(")");
            DBG(std::cout << "Token parsed: " << token << std::endl);
            DBG(std::cout << "Token parsed: " << rbrac << std::endl);
            tokens.push_back(token);
            tokens.push_back(rbrac);
        } else {
            DBG(std::cout << "Token parsed: " << token << std::endl);
            tokens.push_back(token);
        }
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
    operations.insert({"!=", GREQ});

    operations.insert({"while", WHILE});
    operations.insert({"if", IF});
    operations.insert({"print", PRINT});
    operations.insert({"?", IN});
    operations.insert({";", SEMICOL});

    operations.insert({"(", LBRAC});
    operations.insert({")", RBRAC});

    operations.insert({"{", SCOPE_BEG});
    operations.insert({"}", SCOPE_END});

    return operations;
}

 //--------------- parse token and return Lexem*
Lexem* parse_token(const std::string token, const std::unordered_map<std::string, int>& map) {
    Lexem* opt = nullptr;
    int op_kind = -1;

    auto check = map.find(token);
    if (check == map.end()) {
        fprintf(stdout, "Token not recognized!\n");
        op_kind = ID; // possible ID
        //exit(-1);
    }

    if (op_kind == -1) {
        op_kind = map.at(token); // here we are sure that key exists, or it is a possible ID
    }
    if (op_kind >= ASSIGN && op_kind <= EQUAL) {
        BINOP_CREATE(opt, op_kind);
    } else if (op_kind >= WHILE && op_kind <= SEMICOL) {
        KWORD_CREATE(opt, op_kind);
    } else if (op_kind >= SCOPE_BEG && op_kind <= RBRAC) {
        BRAC_CREATE(opt, op_kind);
    } else {
        std::string* decl = new std::string(token);
        opt = new Decl(decl);
    }

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

