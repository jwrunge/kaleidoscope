#include <iostream>
#include <vector>
#include "lexer.h"
#include "parser.h"

static int CurTok;
static int getNextToken() {
    return CurTok = gettok();
}

std::unique_ptr<ExprAST> LogError(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}

//Expression parsing
static std::unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}

// static std::unique_ptr<ExprAST> ParseParenExpr() {
//     getNextToken(); // eat (
//     auto V = ParseExpression();
//     if(!V)
//         return nullptr;

//     if(CurTok != ')')
//         return LogError("expected ')'");
//     getNextToken(); // eat )
//     return V;
// }