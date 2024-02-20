srcdir = "./src"
distdir = "./dist"
intdir = "./intermediate"

lexer = lexer
parser = parser
main = main

objects = $(intdir)/$(lexer).o $(intdir)/$(parser).o

.DEFAULT_GOAL := all

lexer:
	@echo "Building lexer..."
	@g++ -c $(srcdir)/$(lexer).cpp -o $(intdir)/$(lexer).o
	@echo "Done."

parser:
	@echo "Building parser..."
	@g++ -c $(srcdir)/$(parser).cpp -o $(intdir)/$(parser).o
	@echo "Done."

main:
	@echo "Building main..."
	@g++ $(objects) $(srcdir)/$(main).cpp -o $(distdir)/compiler
	@echo "Done."

all: lexer parser main

clean:
	@echo "Cleaning..."
	@rm -f $(distdir)/* 
	@rm -f $(intdir)/* 
	@echo "Done."