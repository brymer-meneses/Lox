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

  scanner__init(input);
  Token** tokens = scanner__scan();

  parser__init(tokens);
  Stmt** statements = parser__parse();

  EXPECT_EQ(statements[0]->type, STMT_VAR);
  EXPECT_EQ(statements[0]->as.var.initializer->as.literal.value->type, LOX_NUMBER);
  EXPECT_EQ(statements[0]->as.var.initializer->as.literal.value->as.number, 5);
}



