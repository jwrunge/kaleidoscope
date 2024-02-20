#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "lexer.h"
#include "parser.h"

int CurTok;
int getNextToken() {
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
std::unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}

std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken(); // eat (
    auto V = ParseExpression();
    if(!V)
        return nullptr;

    if(CurTok != ')')
        return LogError("expected ')'");
    getNextToken(); // eat )
    return V;
}

std::unique_ptr<ExprAST> ParseIdentifierExpr() {
    std::string IdName = IdentifierStr;

    getNextToken(); // eat identifier

    if(CurTok != '(')
        return std::make_unique<VariableExprAST>(IdName);

    getNextToken(); // eat (
    std::vector<std::unique_ptr<ExprAST>> Args;
    if(CurTok != ')') {
        while(1) {
            if(auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if(CurTok == ')')
                break;

            if(CurTok != ',')
                return LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }

    getNextToken(); // eat )
    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

std::unique_ptr<ExprAST> ParsePrimary() {
    switch(CurTok) {
        default:
            return LogError("unknown token when expecting an expression");
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
    }
}

static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                              std::unique_ptr<ExprAST> LHS) {
  while (true) {
    int TokPrec = GetTokPrecedence();

    if (TokPrec < ExprPrec)
      return LHS;

    int BinOp = CurTok;
    getNextToken(); // eat binop

    auto RHS = ParsePrimary();
    if (!RHS)
      return nullptr;

    int NextPrec = GetTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS)
        return nullptr;
    }

    LHS =
        std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
  }
}

int GetTokPrecedence() {
    if(!isascii(CurTok))
        return -1;

    int TokPrec = BinopPrecedence[CurTok];
    if(TokPrec <= 0)
        return -1;
    return TokPrec;
}

std::unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();
    if(!LHS)
        return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

//Prototype parsing
std::unique_ptr<PrototypeAST> ParsePrototype() {
    if(CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype");

    std::string FnName = IdentifierStr;
    getNextToken();

    if(CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    std::vector<std::string> ArgNames;
    while(getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    if(CurTok != ')')
        return LogErrorP("Expected ')' in prototype");

    getNextToken(); // eat )
    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

//Parse definition
std::unique_ptr<FunctionAST> ParseDefinition() {
    getNextToken(); // eat def
    auto Proto = ParsePrototype();
    if(!Proto)
        return nullptr;

    if(auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

//Parse extern
std::unique_ptr<PrototypeAST> ParseExtern() {
    getNextToken(); // eat extern
    return ParsePrototype();
}

//Top level expression parsing
std::unique_ptr<FunctionAST> ParseTopLevelExpr() {
    if(auto E = ParseExpression()) {
        auto Proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

//Top level parsing

void HandleDefinition() {
    if(ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        getNextToken();
    }
}

void HandleExtern() {
    if(ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        getNextToken();
    }
}

void HandleTopLevelExpression() {
    if(ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        getNextToken();
    }
}

void MainLoop() {
    while(1) {
        fprintf(stderr, "ready> ");
        switch(CurTok) {
            case tok_eof:
                return;
            case ';':
                getNextToken();
                break;
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}