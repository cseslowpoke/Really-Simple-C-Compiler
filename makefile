# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -I$(SRC_DIR) -I$(BUILD_DIR) 

# Directories
SRC_DIR = src
BUILD_DIR = build

# Files
LEXER = $(SRC_DIR)/scanner.l
PARSER = $(SRC_DIR)/parser.y
OBJS = $(BUILD_DIR)/lex.yy.o $(BUILD_DIR)/parser.tab.o $(BUILD_DIR)/ast.o $(BUILD_DIR)/io.o

# Targets
all: $(BUILD_DIR) SimpleC

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

SimpleC: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Generate the lexer and parser
$(BUILD_DIR)/lex.yy.c: $(LEXER)
	flex -o $(BUILD_DIR)/lex.yy.c $(LEXER)

$(BUILD_DIR)/parser.tab.c $(BUILD_DIR)/parser.tab.h: $(PARSER)
	bison -o $(BUILD_DIR)/parser.tab.c -d $(PARSER) -Wall -Wcounterexamples

# Compile the source files
$(BUILD_DIR)/lex.yy.o: $(BUILD_DIR)/lex.yy.c $(BUILD_DIR)/parser.tab.h
	$(CC) $(CFLAGS) -c $(BUILD_DIR)/lex.yy.c -o $@

$(BUILD_DIR)/parser.tab.o: $(BUILD_DIR)/parser.tab.c
	$(CC) $(CFLAGS) -c $(BUILD_DIR)/parser.tab.c -o $@

$(BUILD_DIR)/ast.o: $(SRC_DIR)/ast.c $(SRC_DIR)/ast.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/ast.c -o $@

$(BUILD_DIR)/io.o: $(SRC_DIR)/io.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/io.c -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR) SimpleC

.PHONY: all clean