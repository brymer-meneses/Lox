#include "gtest/gtest.h"

extern "C" {
  #include "stdio.h"
  #include "string.h"

  #include "lox/scanner.h"
  #include "lox/token.h"
  #include "lox/lox.h"

  #include "tools/fileloc.h"
  #include "tools/utils.h"
}

TEST(TestScanner, ScanSingleChar) {
  char input[12] = "{}()+-*,.;";
  TokenType correct_types[] = { 
    LEFT_BRACE, RIGHT_BRACE, LEFT_PAREN, RIGHT_PAREN, PLUS,
    MINUS, STAR, COMMA, DOT, SEMICOLON, SOURCE_END
  };

  scanner_init(input);
  Token** tokens = scanner_scan();

  for (unsigned int i=0; i<lox.scanner.parsed; i++) {
    EXPECT_EQ(correct_types[i], tokens[i]->type);
  }

  for (unsigned int i=0; i<strlen(input); i++) {
    EXPECT_EQ(input[i], *tokens[i]->lexeme);
  }

  free(tokens);
}


TEST(TestScanner, ScanDoubleChar) {
  char input[] = "== != >= <=";

  TokenType correct_types[] = {EQUAL_EQUAL, BANG_EQUAL, GREATER_EQUAL, LESS_EQUAL, SOURCE_END};

  scanner_init(input);
  Token** tokens = scanner_scan();

  for (unsigned int i=0; i<lox.scanner.parsed; i++) {
    EXPECT_EQ(correct_types[i], tokens[i]->type);
  }

  free(tokens);
}

TEST(TestScanner, ScanKeywords) {
  char input[] = "print var class return";

  TokenType correct_types[] = {PRINT, VAR, CLASS,  RETURN, SOURCE_END};

  scanner_init(input);
  Token** tokens = scanner_scan();

  for (unsigned int i=0; i<lox.scanner.parsed; i++) {
    EXPECT_EQ(correct_types[i], tokens[i]->type);
  }

  free(tokens);
}

// TEST(TestScanner, ScanVarDeclaration) {
//   char input[] = "test test1 test2 test_underscore _test_underscore2";
//
//   TokenType correct_types[] = {IDENTIFIER, IDENTIFIER, IDENTIFIER, IDENTIFIER, SOURCE_END};
//   char** correct_literals = str_split(input, " ");
//
//   scanner_init(input);
//   Token* tokens = scanner_scan();
//
//   for (unsigned int i=0; i<lox.scanner.parsed; i++) {
//     EXPECT_EQ(correct_types[i], tokens[i].type);
//     EXPECT_TRUE(strcmp(correct_literals[i], tokens[i].literal.data.string) == 0);
//   }
//
// }
