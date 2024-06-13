# Makefile for Flex and Bison

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Files
LEXER = scanner.l
PARSER = parser.y
OBJS = lex.yy.o parser.tab.o ast.o

# Targets
all: SimpleC

SimpleC: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) 

# Generate the lexer and parser
lex.yy.c: $(LEXER)
	flex $(LEXER)

parser.tab.c parser.tab.h: $(PARSER)
	bison -d $(PARSER) -Wall -Wcounterexamples

# Compile the source files
lex.yy.o: lex.yy.c parser.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c

parser.tab.o: parser.tab.c
	$(CC) $(CFLAGS) -c parser.tab.c

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c
# Clean up
clean:
	rm -f my_program $(OBJS) lex.yy.c parser.tab.c parser.tab.h

.PHONY: all clean