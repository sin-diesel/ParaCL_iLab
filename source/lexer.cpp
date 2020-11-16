#include "lexer.h"
#include "tests.h"
#include <string>
#include <string.h>
#include <iostream>
#include <assert.h>
#include <errno.h>
#include <vector>
#include <unordered_map>


/* TODO: 1) implement error checking in lexer
        2) fix unordered map implementation*/

Token::Token(int kind, const std::unordered_map<int, std::string>* map) {
    token_kind = kind;

    if (kind != ID && kind != VALUE) {
        auto check = map->find(kind);

        std::string str = map->at(kind); // here we are sure key exists
       // std::string* id = new std::string(str);
        token_str = str;
    } else if (kind == ID) {
       // std::string* id = new std::string("Identifier");
        token_str = "Identifier";
    } else if (kind == VALUE) {
        //std::string* value = new std::string("Value");
        token_str = "Value";
    }
}

void Token::print() const {
    std::cout << "Token: " << token_str << std::endl;
}

Token::Token(Token* token) {
    token_kind = token->token_kind;
    token_str = token->token_str;
}

Word::Word(std::string word, int token_kind, const std::unordered_map<int, std::string>* map):Token(token_kind, map) {
    m_word = word;
}

Word::Word(Token* token) {
    token_kind = token->token_kind;
    token_str = token->token_str;
    Word* word = static_cast<Word*>(token);
    m_word = word->m_word;
}

Value::Value(int value, const std::unordered_map<int, std::string>* map):Token(VALUE, map) {
    m_value = value;
}

Value::Value(Token* token) {
    token_kind = token->token_kind;
    token_str = token->token_str;
    Value* value = static_cast<Value*>(token);
    m_value = value->m_value;
}

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
        //std::string* id = new std::string(lexem); // here we identifited it is an ID
        token_kind = ID; 
        token = new Word(lexem, ID, map_tostr);
        }
    }

    assert(token != nullptr);
    return token;
}



//--------------- return vector of lexems
std::vector<Token*> lexer(char* buf) {
    
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

