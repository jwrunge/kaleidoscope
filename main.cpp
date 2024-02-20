#include <iostream>
#include <vector>

enum Token {
    tok_eof = -1,

    //commands
    tok_def = -2,
    tok_extern = -3,

    //primary
    tok_identifier = -4,
    tok_number = -5,
};

class ExprAST {
    public:
        virtual ~ExprAST() = default;
};

class NumberExprAST : public ExprAST {
    double Val;

    public:
        NumberExprAST(double Val) : Val(Val) {}
};

class VariableExprAST : public ExprAST {
    std::string Name;

    public:
        VariableExprAST(const std::string &Name) : Name(Name) {}
};

class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
            : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

    public:
        CallExprAST(
            const std::string &Callee, 
            std::vector<std::unique_ptr<ExprAST>> Args
        )
        : Callee(Callee), Args(std::move(Args)) {}
};

class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

    public:
        PrototypeAST(const std::string &Name, std::vector<std::string> Args)
            : Name(Name), Args(std::move(Args)) {}
        
        const std::string &getName() const { return Name; }
};

class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body)
            : Proto(std::move(Proto)), Body(std::move(Body)) {}
};


static std::string IdentifierStr;   // Filled in if tok_identifier
static double NumVal;               // Filled in if tok_number

static int gettok() {
    static int LastChar = ' ';

    // Skip any whitespace
    while(isspace(LastChar))
        LastChar = getchar();

    // Handle identifiers and literals
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

    // Handle numbers
    if(isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    // Handle comments
    if(LastChar == '#') {
        do 
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if(LastChar != EOF)
            return gettok();
    }

    // Check for end of file. Don't eat the EOF
    if(LastChar == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

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
    auto Result = std::make_unique<ExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}

static std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken(); // eat (
    auto V = ParseExpression();
    if(!V)
        return nullptr;

    if(CurTok != ')')
        return LogError("expected ')'");
    getNextToken(); // eat )
    return V;
}