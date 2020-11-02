#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <assert.h>
#include <fstream>

int main () {

    FILE* input = fopen("./test_files/001.dat", "rb");
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

    std::vector<Lexem*> lexems = lexer(buf);

    for (int i = 0; i < lexems.size(); ++i) {
        lexems[i]->print();
    }

    Parse_tree_t tree(lexems);

    fclose(input);
    free(buf);
    return 0;
}
