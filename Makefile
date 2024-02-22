srcdir = "./src"
distdir = "./dist"
intdir = "./intermediate"

lexer = $(srcdir)/lexer.cpp
parser = $(srcdir)/parser.cpp
codegen = $(srcdir)/codegen.cpp
main = $(srcdir)/main.cpp

files = $(lexer) $(parser) $(codegen) $(main)

.DEFAULT_GOAL := main

main:
	@echo "Building main..."
	@g++ `llvm-config-18 --cxxflags --ldflags --system-libs --libs core` $(files) -o $(distdir)/compiler
	@echo "Done."

clean:
	@echo "Cleaning..."
	@rm -f $(distdir)/* 
	@echo "Done."