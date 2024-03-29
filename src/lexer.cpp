#include <iostream>
#include <vector>
#include "lexer.h"

std::string IdentifierStr;   // Filled in if tok_identifier
double NumVal;               // Filled in if tok_number
std::map<char, int> BinopPrecedence; //Binary expression parsing

int gettok() {
    static int LastChar = ' ';

    // Skip any whitespace
    while(isspace(LastChar)) LastChar = getchar();

    // Handle identifiers and literals
    if(isalpha(LastChar)) {
        IdentifierStr = LastChar;

        while(isalnum((LastChar = getchar()))) IdentifierStr += LastChar;
        
        if(IdentifierStr == "def") return tok_def;
        if(IdentifierStr == "extern")  return tok_extern;
        return tok_identifier;
    }

    // Handle numbers
    if(isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    // Handle comments
    if(LastChar == '#') {
        do LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if(LastChar != EOF) return gettok();
    }

    // Check for end of file. Don't eat the EOF
    if(LastChar == EOF) return tok_eof;

    // Otherwise, just return the character as its ascii value
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}
