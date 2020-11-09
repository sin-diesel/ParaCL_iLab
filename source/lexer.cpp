#include "lexer.h"
//#include "parser.h"
#include <string>
#include <string.h>
#include <iostream>
#include <assert.h>
#include <errno.h>
#include <vector>
#include <unordered_map>

Token::Token(int kind, const std::unordered_map<int, std::string>* map) {
    token_kind = kind;

    auto check = map->find(kind);
    if (check == map->end()) {
        fprintf(stdout, "lexem not recognized!\n");
    }

    std::string str = map->at(kind); // here we are sure key exists
    std::string* id = new std::string(str);
    token_str = id;
}

void Token::print() {
    std::cout << "Token: " << *token_str << std::endl;
}

// Lexem::Lexem(int kind) {
//     lexem_kind = kind;
// }

// BinOP::BinOP(int kind):Lexem(BINOP) {
//     binop_kind = kind;
// }

// KeyWord::KeyWord(int kind):Lexem(KEYWORD) {
//     keyword_kind = kind;
// }

// Brack::Brack(int kind):Lexem(BRAC) {
//     brack_kind = kind;
// }

Word::Word(std::string* word, int token_kind, const std::unordered_map<int, std::string>* map):Token(token_kind, map) {
    word_ = word;
}

Value::Value(int value, const std::unordered_map<int, std::string>* map):Token(VALUE, map) {
    value_ = value;
}

// void BinOP::print() const {
//     fprintf(stdout, "\nBinOP lexem: %p\n\ttype: %d\n", this, binop_kind); // fix
// }

// int BinOP::get_type() const {
//     return binop_kind;
// }

// void BinOP::print_dot(FILE* dot_file) {

//     CAST(BinOP, this, lex)

//     if (lex->lhs != nullptr) {
//     fprintf (dot_file, "\n\t\t\"%d\"[shape = \"ellipse\", \
//                 color=\"#191970\", style=\"filled\", fillcolor = \"#E0FFFF\"];\n" \
//                 "\t\t\"%d\"->\"%d\"\n", lex->token_kind, lex->token_kind, (lex->lhs)->token_kind);

//     (lex->lhs)->print_dot(dot_file);

//     }
    
//     if (lex->rhs != nullptr) {
//         fprintf (dot_file, "\n\t\t\"%d\"[shape = \"ellipse\", \
//                 color=\"#191970\", style=\"filled\", fillcolor = \"#E0FFFF\"];\n" \
//                 "\t\t\"%d\"->\"%d\"\n", lex->token_kind, lex->token_kind, (lex->rhs)->token_kind);

//         (lex->rhs)->print_dot(dot_file);
//     }
// }

// void KeyWord::print() const {
//     fprintf(stdout, "\nKeyWord lexem: %p\n\ttype: %d\n", this, keyword_kind); // fix
// }

// void KeyWord::print_dot(FILE* dot_file) {

//     CAST(KeyWord, this, lex)

//     if (lex->lhs != nullptr) {
//     fprintf (dot_file, "\n\t\t\"%d\"[shape = \"ellipse\", \
//                 color=\"#191970\", style=\"filled\", fillcolor = \"#E0FFFF\"];\n" \
//                 "\t\t\"%d\"->\"%d\"\n", lex->token_kind, lex->token_kind, (lex->lhs)->token_kind);

//     (lex->lhs)->print_dot(dot_file);

//     }
    
//     if (lex->rhs != nullptr) {
//         fprintf (dot_file, "\n\t\t\"%d\"[shape = \"ellipse\", \
//                 color=\"#191970\", style=\"filled\", fillcolor = \"#E0FFFF\"];\n" \
//                 "\t\t\"%d\"->\"%d\"\n", lex->token_kind, lex->token_kind, (lex->rhs)->token_kind);

//         (lex->rhs)->print_dot(dot_file);
//     }
// }

// int KeyWord::get_type() const {
//     return keyword_kind;
// }


// void Brack::print() const {
//     fprintf(stdout, "\nBrack lexem: %p\n\ttype: %d\n", this, brack_kind); // fix
// }

// void Brack::print_dot(FILE* dot_file) {
//     return;
// }

// int Brack::get_type() const {
//     return brack_kind;
// }

// void Decl::print() const {
//     fprintf(stdout, "\nDecl lexem: %p\n\ttype: %d\n", this, ID); // fix
//     std::cout << "Decl is: " << *decl_ << "\n";
// }

// void Decl::print_dot(FILE* dot_file) {
//     return;
// }

// int Decl::get_type() const {
//     return ID;
// }

// void Value::print() const {
//     fprintf(stdout, "\nValue lexem: %p\n\ttype: %d\n", this, ID); // fix
//     std::cout << "Value is: " << value << "\n";
// }

// void Value::print_dot(FILE* dot_file) {
//     return;
// }

// int Value::get_type() const {
//     return value;
// }

 //--------------- get lexems in vector
std::vector<std::string> extract_lexems(char* buf) {
    std::vector<std::string> lexems;

    //--------------- get single lexem
    char* beginning = strtok(buf, " \n");
    int size = strlen(beginning);
    std::string lexem(beginning, size);

    DBG(std::cout << "lexem parsed: " << lexem << std::endl);
    lexems.push_back(lexem);
    
     //--------------- get remaining tokens
    while ((beginning = strtok(NULL, " \n")) != NULL) {
        int size = strlen(beginning);
        lexem = std::string(beginning, size);
        
        char sem = lexem.find(';');   // check for semicolon at the end of a token
        char lbr = lexem.find('('); // check for round brackets
        char rbr = lexem.find(')');

        if (sem != -1 && lexem.size() > 1) {

            lexem = lexem.substr(0, lexem.size() - 1);
            std::string semicolon = std::string(";");

            DBG(std::cout << "lexem parsed: " << lexem << std::endl);
            DBG(std::cout << "lexem parsed: " << semicolon << std::endl);

            lexems.push_back(lexem);
            lexems.push_back(semicolon);
        } else if (lbr != -1 && lexem.size() > 1) {

            lexem = lexem.substr(1, lexem.size() - 1);
            std::string lbrac = std::string("(");

            DBG(std::cout << "lexem parsed: " << lbrac << std::endl);
            DBG(std::cout << "lexem parsed: " << lexem << std::endl);

            lexems.push_back(lbrac);
            lexems.push_back(lexem);
        } else if (rbr != -1 && lexem.size() > 1) {
            
            lexem = lexem.substr(0, lexem.size() - 1);
            std::string rbrac = std::string(")");

            DBG(std::cout << "lexem parsed: " << lexem << std::endl);
            DBG(std::cout << "lexem parsed: " << rbrac << std::endl);

            lexems.push_back(lexem);
            lexems.push_back(rbrac);
        } else {
            DBG(std::cout << "lexem parsed: " << lexem << std::endl);
            lexems.push_back(lexem);
        }
    }
    return lexems;
}

 //--------------- associate string values with corresponding enums
std::unordered_map<std::string, int> set_token_types() {
    std::unordered_map<std::string, int> token_types;

    token_types.insert({"+", ADD});
    token_types.insert({"-", SUB});
    token_types.insert({"*", MUL});
    token_types.insert({"/", DIV});
    token_types.insert({"=", ASSIGN});
    token_types.insert({"==", EQUAL});
    token_types.insert({"<", LESS});
    token_types.insert({">", GREATER});
    token_types.insert({"<=", LESSEQ});
    token_types.insert({">=", GREQ});
    token_types.insert({"!=", NOTEQUAL});

    token_types.insert({"while", WHILE});
    token_types.insert({"if", IF});
    token_types.insert({"print", PRINT});
    token_types.insert({"?", IN});
    token_types.insert({";", SEMICOL});

    token_types.insert({"(", LBRAC});
    token_types.insert({")", RBRAC});

    token_types.insert({"{", LSBRAC});
    token_types.insert({"}", RSBRAC});

    return token_types;
}

 //--------------- associate enum values with corresponding strings
std::unordered_map<int, std::string> set_token_strings() {
    std::unordered_map<int, std::string> token_strings;

    token_strings.insert({ADD, "+"});
    token_strings.insert({SUB, "-"});
    token_strings.insert({MUL, "*"});
    token_strings.insert({DIV, "/"});
    token_strings.insert({ASSIGN, "="});
    token_strings.insert({EQUAL, "=="});
    token_strings.insert({LESS, "<"});
    token_strings.insert({GREATER, ">"});
    token_strings.insert({LESSEQ, "<="});
    token_strings.insert({GREQ, ">="});
    token_strings.insert({NOTEQUAL, "!="});

    token_strings.insert({WHILE, "while"});
    token_strings.insert({IF, "if"});
    token_strings.insert({PRINT, "print"});
    token_strings.insert({IN, "?"});
    token_strings.insert({SEMICOL, ";"});

    token_strings.insert({LBRAC, "("});
    token_strings.insert({RBRAC, ")"});

    token_strings.insert({LSBRAC, "{"});
    token_strings.insert({RSBRAC, "}"});

    return token_strings;
}


 //--------------- parse lexem and return Tokens vector
Token* parse_lexem(const std::string lexem, const std::unordered_map<std::string, int>* map_toint, \
                                            const std::unordered_map<int, std::string>* map_tostr) {
    Token* token = nullptr;
    int token_kind = -1;

    auto check = map_toint->find(lexem);
    if (check == map_toint->end()) {
        fprintf(stdout, "lexem not recognized!\n");
        token_kind = ID; // possible id
    }

    if (token_kind == -1) {
        token_kind = map_toint->at(lexem); // here we are sure that key exists, or it is a possible ID
        token = new Token(token_kind, map_tostr);
    } else {

        char* p = nullptr;
        int num = strtol(lexem.c_str(), &p, 10); // checking if a number

        if (num != 0 || *(lexem.c_str()) == '0') {
            token_kind = VALUE; // possible value
            token = new Value(num, map_tostr);
        } else {

        DBG(fprintf(stdout, "Number conversion failed\n"));
        std::string* id = new std::string(lexem); // here we identifited it is an ID
        token_kind = ID; 
        token = new Word(id, ID, map_tostr);
        }
    }

    assert(token != nullptr);
    return token;
}



//--------------- return vector of lexems
std::vector<Token*> lexer(char* buf) {
    
    unsigned size = strlen(buf);
    char* current = buf;

    std::vector<Token*> tokens;

    std::unordered_map<std::string, int> token_types = set_token_types();
    std::unordered_map<int, std::string> token_str = set_token_strings();
    std::vector<std::string> lexems = extract_lexems(buf);

    for (int i = 0; i < lexems.size(); ++i) {
        Token* token = parse_lexem(lexems[i], &token_types, &token_str);
        // DBG(std::cout << i << ": ");
        // DBG(token->print());
        tokens.push_back(token);
    }

    return tokens;
}

