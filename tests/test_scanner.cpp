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
  char input[] = "print var class return varname";

  TokenType correct_types[] = {PRINT, VAR, CLASS,  RETURN, IDENTIFIER, SOURCE_END};

  scanner_init(input);
  Token** tokens = scanner_scan();

  for (unsigned int i=0; i<lox.scanner.parsed; i++) {
    EXPECT_EQ(correct_types[i], tokens[i]->type);
  }

  free(tokens);
}

TEST(TestScanner, ScanVariableNames) {
  char input[] = "test test1 test2 test_underscore _test_underscore";

  TokenType correct_types[] = {IDENTIFIER, IDENTIFIER, IDENTIFIER, IDENTIFIER, IDENTIFIER, SOURCE_END};
  char** correct_literals = str_split(input, " ");

  scanner_init(input);
  Token** tokens = scanner_scan();

  for (unsigned int i=0; i<lox.scanner.parsed-1; i++) {
    EXPECT_EQ(correct_types[i], tokens[i]->type);
    EXPECT_TRUE(strcmp(correct_literals[i], tokens[i]->lexeme) == 0);
  }

}

TEST(TestScanner, ScanString) {
  char input[] = "\"hello world\"";
  char* correct = substring(input, 1, 11);

  scanner_init(input);
  Token** tokens = scanner_scan();

  EXPECT_TRUE(strcmp(tokens[0]->literal->data.string, correct) == 0);
}

TEST(TestScanner, ScanVariableDeclaration) {
  char input[] = "var input = 5;";
  TokenType correct_types[] = {VAR, IDENTIFIER, EQUAL, NUMBER, SEMICOLON, SOURCE_END};

  scanner_init(input);
  Token** tokens = scanner_scan();

  for (unsigned int i=0; i<lox.scanner.parsed-1; i++) {
    EXPECT_EQ(correct_types[i], tokens[i]->type);
  }
  
}
