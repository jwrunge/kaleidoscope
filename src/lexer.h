#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <map>
#include <map>

enum Token {
    tok_eof = -1,

    //commands
    tok_def = -2,
    tok_extern = -3,

    //primary
    tok_identifier = -4,
    tok_number = -5,
};

extern std::string IdentifierStr;   // Filled in if tok_identifier
extern double NumVal;               // Filled in if tok_number
extern std::map<char, int> BinopPrecedence; //Binary expression parsing

int gettok();
int GetTokPrecedence();

#endif