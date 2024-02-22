#include "lexer.h"
#include "parser.h"
#include "codegen.h"

int main() {
    // Install standard binary operators.
    // 1 is lowest precedence.
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40; // highest.

    // Prime the first token.
    fprintf(stderr, "ready> ");
    getNextToken();

    // Make the module which holds all the code
    InitializeModule();

    // Run the main "interpreter loop" now.
    MainLoop();

    return 0;
}