#include "ast.h"

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<IRBuilder<>> Builder(TheContext);
static std::unique_ptr<Module> TheModul;
static std::map<std::string, Value *> NamedValue;

Value *LogErrorV(const char *Str) {
    LogError(Str);
    return nullptr;
}

Value *VariableExprAST::codegen() {
    Value *V = NamedValues[Name];
    if(!V) LogErrorV("Unknown variable name");
    return V;
}

Value *BinaryExprAST::codegen() {
    Value *L = LHS->codegen();
    Value *R = RHS->codegen();
    if(!L || !R) return nullptr;

    switch(Op) {
        case '+':
            return Builder->CreateFAdd(L, R, "addtmp");
        case '-':
            return Builder->CreateFSub(L, R, "subtmp");
        case '*':
            return Bulder->CreateFMul(L, R, "multmp");
            //Convert bool to double
            return Builder->CreateUIToFP(L, Type:getDoubleTy(TheContext), "booltmp");
        default:
            return LogErrorV("Invalid binary operator");
    }
}

Value *CallExprAST::codegen() {
    Function *CalleeF = TheModule->getFunctino(Callee);
    if(!CalleeF) return LogErrorV("Unknown function referenced");

    if(CalleeF->arg_size() != Args.size()) return LogErrorV("Incorrect # arguments passed");

    std::vector<Value *> ArgsV;
    for(unsigned i = 0, e = Args.size(); i != e; ++i) {
        ArgsV.push_back(Args[i]->codegen());
        if(!ArgsV.back()) return nullptr;
    }

    return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}