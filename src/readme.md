# Code Structure

## Scanning
  - files: `scanner.c/scanner.h`
  - Scanning or lexing is the first part of the "frontend" of an interpreter. In this stage the different substrings 
    of the source code gets classified into "tokens," which will then be used by the parser.

  - I made the decision to use a static variable for the scanner, this will increase the readability of the code by not 
    passing each pointer into each function.

## Parsing
  - files: `parser.c/parser.h`
  - Parsing uses the tokens produced by the scanner to produce an [abstract syntax tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree).
    As the name suggests this uses the [tree](https://en.wikipedia.org/wiki/Tree_(data_structure)), to represent the source code.

## Interpreter
  - files: `interpreter.c/interpreter.h`
  - The interpreter, leverages the abstract syntax tree produced by the parser and visits each "node" using the visitor pattern, to interpret 
    each node.

## Lox
  - files: `lox.c/lox.h`
  - Lox is an abstract of each of the components discussed above. It provides a simple API that handles the runtime of the application.

