#include "lexer.h"
#include <string>
#include <assert.h>
#include <string.h>
#include <iostream>

void unit_test_1() {
    char buf[] = "+ - = <= > <";
    std::vector<std::string> tokens = extract_tokens(buf);

    DBG(for (int i = 0; i < tokens.size(); ++i) {
        fprintf(stdout, "Tokens[%d]: ", i);
        std::cout << tokens[i] << std::endl;
    });

    std::cout << "TEST 1 PASSED" << std::endl;
}

void unit_test_2() {
    std::unordered_map<std::string, int> ops = set_operations();

    std::string add("+");
    std::string leseq("<=");
    std::string invalid("<==");

    auto iter = ops.find(add);
    assert(iter != ops.end());
    iter = ops.find(leseq);
    assert(iter != ops.end());
    iter = ops.find(invalid);
    assert(iter == ops.end());

    std::cout << "TEST 2 PASSED" << std::endl;
}

void unit_test_3() {
    char buf[] = "+ - = <= > <";
    //std::vector<std::string> tokens = extract_tokens(buf);
    fprintf(stdout, "Running lexer...\n");
    std::vector<Lexem*> lexems = lexer(buf);
    for (int i = 0; i < lexems.size(); ++i) {
        lexems[i]->print();
    }

    
    std::cout << "TEST 3 PASSED" << std::endl;
}

void unit_test_4() {
    char buf[] = "+ - == == = <= > < while if ?  =";
    //std::vector<std::string> tokens = extract_tokens(buf);

    fprintf(stdout, "Running lexer...\n");
    std::vector<Lexem*> lexems = lexer(buf);
    fprintf(stdout, "Lexer is done.\n");
    for (int i = 0; i < lexems.size(); ++i) {
      // lexems[i]->print();
    }

    std::cout << "TEST 4 PASSED" << std::endl;

}

void unit_test_5() {
    char buf[] = "if ( );\n{ \n};";

    fprintf(stdout, "Running lexer...\n");
    std::vector<Lexem*> lexems = lexer(buf);
    fprintf(stdout, "Lexer is done.\n");
    KeyWord* semicol = (KeyWord*) lexems[3];
    assert(semicol->keyword_kind == SEMICOL);
    for (int i = 0; i < lexems.size(); ++i) {
      //lexems[i]->print();
    }

    std::cout << "TEST 5 PASSED" << std::endl;

}

void unit_test_6() {
    char buf[] = "if ( (index * 2 + 3) != other_index) {\n index = other_index;\n other_index = ?; }; ";

    fprintf(stdout, "Running lexer...\n");
    std::vector<Lexem*> lexems = lexer(buf);
    fprintf(stdout, "Lexer is done.\n");
    for (int i = 0; i < lexems.size(); ++i) {
      lexems[i]->print();
    }

    std::cout << "TEST 6 PASSED" << std::endl;

}

void unit_test_7() {
    char buf[] = "if (((a != b))) {\n a = 0;\nb = a + 1 + ((  (c * 2204) + 1));\n }\n";

    fprintf(stdout, "Running lexer...\n");
    std::vector<Lexem*> lexems = lexer(buf);
    fprintf(stdout, "Lexer is done.\n");
    for (int i = 0; i < lexems.size(); ++i) {
      lexems[i]->print();
    }

    std::cout << "TEST 7 PASSED" << std::endl;

}