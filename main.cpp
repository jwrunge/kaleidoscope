#include <iostream>

enum Token {
    tok_eof = -1,

    //commands
    tok_def = -2,
    tok_extern = -3,

    //primary
    tok_identifier = -4,
    tok_number = -5,
};

static std::string IdentifierStr;   // Filled in if tok_identifier
static double NumVal;               // Filled in if tok_number

static int gettok() {
    static int LastChar = ' ';
    while(isspace(LastChar))
        LastChar = getchar();

    if(isalpha(LastChar)) {
        IdentifierStr = LastChar;

        if(IdentifierStr == "def") {
            return tok_def;
        }
        if(IdentifierStr == "extern") {
            return tok_extern;
        }
        return tok_identifier;
    }
}