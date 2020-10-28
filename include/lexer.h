#include <vector>
#include <unordered_map>

#define DEBUG true

#define BINOP_CREATE(pointer, type) pointer = new BinOP(type); \

#define DBG(expr) if (DEBUG) {expr;}


enum token_kind_t { BINOP, KEYWORD, ID, VALUE, CMP, LITERAL, SCOPE, BRAC, SEMICOL};
enum binop_t { ASSIGN, ADD, SUB, MUL, DIV, LESS, GREATER, LESSEQ, GREQ, EQUAL };
enum keywords_t { WHILE, IF, PRINT, IN };
enum braces_t { SCOPE_BEG, SCOPE_END, LBRAC, RBRAC };

struct Lexem {
public:
    int token_kind;

    Lexem() = delete;
    Lexem(int kind);
    virtual void print() const = 0;
    virtual ~Lexem() {};
};

struct BinOP: public Lexem {
    int binop_kind;

    BinOP() = delete;
    BinOP(int binop);
    void print() const override;
};

struct KeyWord:public Lexem {
    int keyword_kind;

    KeyWord(int keyword_kind);
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
