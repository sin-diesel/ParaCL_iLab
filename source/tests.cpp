#include "lexer.h"
#include <string>
#include <assert.h>
#include <string.h>
#include <iostream>

#define TEST_BEGIN(num) fprintf(stdout, "\n\n\n UNIT TEST" #num  "\n");
#define TEST_END(num) fprintf(stdout, "\n\n\n TEST"  #num  "PASSED\n");

void unit_test_1() {
    TEST_BEGIN(1)
    char buf[] = "+ - = <= > <";
    std::vector<std::string> lexems = extract_lexems(buf);

    DBG(for (int i = 0; i < lexems.size(); ++i) {
        fprintf(stdout, "Lexem[%d]: ", i);
        std::cout << lexems[i] << std::endl;
    });

    TEST_END(1)
}

void unit_test_2() {
    TEST_BEGIN(2)
    std::unordered_map<std::string, int> token_types = set_token_types();

    std::string add("+");
    std::string leseq("<=");
    std::string invalid("<==");

    auto iter = token_types.find(add);
    assert(iter != token_types.end());
    iter = token_types.find(leseq);
    assert(iter != token_types.end());
    iter = token_types.find(invalid);
    assert(iter == token_types.end());

    TEST_END(2)
}

void unit_test_3() {
    TEST_BEGIN(3)
    char buf[] = "+ - = <= > <";
    fprintf(stdout, "Running lexer...\n");
    std::vector<Token*> tokens = lexer(buf);
    for (int i = 0; i < tokens.size(); ++i) {
        tokens[i]->print();
    }
    TEST_END(3)
}

void unit_test_4() {
    TEST_BEGIN(4)
    char buf[] = "+ - == == = <= > < while if ?  =";
    //std::vector<std::string> tokens = extract_tokens(buf);

    fprintf(stdout, "Running lexer...\n");
    std::vector<Token*> tokens = lexer(buf);
    fprintf(stdout, "Lexer is done.\n");
    for (int i = 0; i < tokens.size(); ++i) {
        tokens[i]->print();
    }

    TEST_END(4)

}

void unit_test_5() {
    TEST_BEGIN(5)
    char buf[] = "if ( );\n{ \n};";

    fprintf(stdout, "Running lexer...\n");
    std::vector<Token*> tokens = lexer(buf);
    fprintf(stdout, "Lexer is done.\n");
    assert(tokens[1]->token_kind == LBRAC);
    assert(tokens[2]->token_kind == RBRAC);
    for (int i = 0; i < tokens.size(); ++i) {
        tokens[i]->print();
    }

    TEST_END(5)

}

void unit_test_6() {
    TEST_BEGIN(6)
    char buf[] = "if ( (index * 2 + 3) != other_index) {\n index = other_index;\n other_index = ?; }; ";

    fprintf(stdout, "Running lexer...\n");
    std::vector<Token*> tokens = lexer(buf);
    fprintf(stdout, "Lexer is done.\n");
    for (int i = 0; i < tokens.size(); ++i) {
      tokens[i]->print();
    }

    TEST_END(6)

}

void unit_test_7() {
    char buf[] = "if (((a != b))) {\n a = 0;\nb = a + 1 + ((  (c * 2204) + 1));\n }\n";

    fprintf(stdout, "Running lexer...\n");
    std::vector<Token*> tokens = lexer(buf);
    fprintf(stdout, "Lexer is done.\n");
    for (int i = 0; i < tokens.size(); ++i) {
      tokens[i]->print();
    }

    std::cout << "TEST 7 PASSED" << std::endl;

}

void unit_test_8() {
    
}

void test() {

    FILE* input = fopen("./tests/001.dat", "rb");
    assert(input != NULL);

    

    int end = fseek(input, 0L, SEEK_END);
    int size = ftell(input);
    rewind(input);
    fprintf(stdout, "File size: %d\n", size);

    char* buf = (char*) calloc(size, sizeof(char));
    int read = fread(buf, sizeof(char), size, input);
    fprintf(stdout, "Read:%d\n", read);
    assert(read == size);
    fprintf(stdout, "Buf:%s\n", buf);

    std::cout << "Testing lexer..." << std::endl;
    std::vector<Token*> tokens = lexer(buf);

    for (int i = 0; i < tokens.size(); ++i) {
        tokens[i]->print();
    }

    std::cout << "Testing complete.\n\n\n";

    //Parse_tree_t tree(lexems);

    // FILE* dot_file = fopen("dot_file.dot", "w");
    // assert(dot_file != NULL);

    //fprintf(dot_file, "digraph {\n\t node[shape = \"octagon\", color = \"#191970\", fontsize = 12, style = \"filled\", fillcolor = \"#87CEFA\"]; \n\t edge[color = \"#191970\", fontsize = 14];\n\t");

    //tree.head->print_dot(dot_file);

    //fclose(dot_file);
    fclose(input);
    free(buf);

}
