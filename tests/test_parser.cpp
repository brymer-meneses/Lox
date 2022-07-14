#include "gtest/gtest.h"

extern "C" {
  #include "lox/scanner.h"
  #include "lox/parser.h"
  #include "lox/token.h"
  #include "lox/stmt.h"
  #include "lox/interpreter.h"
}

TEST(TestParser, VariableDeclaration) {

  char input[] = "var name = 5;";

  scanner_init(input);
  Token** tokens = scanner_scan();

  parser_init(tokens);
  // Stmt** statements = parser_parse();

  // interpret(statements);
}

