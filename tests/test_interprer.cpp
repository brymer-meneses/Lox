#include "gtest/gtest.h"

extern "C" {
  #include "lox/interpreter.h"
  #include "lox/parser.h"
  #include "lox/scanner.h"
  #include "lox/environment.h"
  #include "lox/object.h"
}


TEST(TestInterpreter, BinaryOperation) {

  Environment* env = environment_init(NULL);

  const char* input = "var ten = 5 + 5;";

  Scanner* scanner = scanner_init((char*) input);
  Parser*  parser = parser_init(scanner_scan());
  Stmt** statements = parser_parse();

  LoxObject* result = execute(env, statements[0]);


  EXPECT_EQ(result->as.number, 10);
}

