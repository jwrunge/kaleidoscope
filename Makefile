srcdir = "./src"
distdir = "./dist"
intdir = "./intermediate"

lexer = $(srcdir)/lexer.cpp
parser = $(srcdir)/parser.cpp
main = $(srcdir)/main.cpp

#cpath = #-I/usr/include/llvm-18 -I/usr/include/llvm-c-18
objects = $(intdir)/$(lexer).o $(intdir)/$(parser).o
files = $(lexer) $(parser) $(main)

.DEFAULT_GOAL := main

main:
	@echo "Building main..."
	@echo g++ $(llvm-config --ldflags --libs) $(files) -o $(distdir)/compiler
	@g++ $(cpath) $(files) -o $(distdir)/compiler
	@echo "Done."

clean:
	@echo "Cleaning..."
	@rm -f $(distdir)/* 
	@echo "Done."