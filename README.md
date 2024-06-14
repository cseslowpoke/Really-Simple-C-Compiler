# really-simple-c-compiler

This project was developed specifically for a friend’s course project. It is a simple C compiler that generates AT&T syntax x86-64 assembly code.

## Features

- Lexical analysis using Flex
- Parsing using Bison
- Abstract Syntax Tree (AST) generation
- Simple code generation for x86-64 architecture using AT&T syntax
- Supports basic arithmetic operations, variable declarations, assignments, and basic I/O operations

## Getting Started

### Prerequisites

Make sure you have the following installed:

- GCC
- Flex
- Bison
- NASM (Netwide Assembler)

### Directory Structure

- `src/` - Contains all the source code files (`.c`, `.h`, `.l`, `.y`)
- `build/` - All the build artifacts (`.o`, `.out`, `.asm`, etc.) will be placed here

### Building the Compiler

To build the compiler, simply run the following command:

```sh
make