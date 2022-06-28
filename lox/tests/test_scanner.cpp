#include "gtest/gtest.h"

extern "C" {
  #include "stdio.h"
  #include "string.h"

  #include "lox/scanner.h"
  #include "lox/token.h"
}

TEST(TestScanner, TestRegisterToken) {
  char input[15] = "var test = 10";
  Scanner scanner = scanner_init(input);

  char lexeme_a[15] = "var";
  char lexeme_b[15] = "10";

  Token token_a = token_init(IDENTIFIER, lexeme_a, 1);
  Token token_b = token_init(NUMBER, lexeme_b, 1);

  scanner_register_token(&scanner, token_a);
  scanner_register_token(&scanner, token_b);

  ASSERT_EQ(token_a.lexeme, scanner.tokens[0].lexeme);
  ASSERT_EQ(token_a.type, scanner.tokens[0].type);

  ASSERT_EQ(token_b.lexeme, scanner.tokens[1].lexeme);
  ASSERT_EQ(token_b.type, scanner.tokens[1].type);
}

TEST(TestScanner, scanner_advance) {
  char input[14] = "var test = 10";
  Scanner scanner = scanner_init(input);

  for (int i=0; i<strlen(input); i++) {
    ASSERT_EQ(input[i], scanner_advance(&scanner));
  }
}

TEST(TestScanner, ScanSingleChar) {
  char input[12] = "{}()+-*,.;";
  TokenType correct_types[] = { 
    LEFT_BRACE, RIGHT_BRACE, LEFT_PAREN, RIGHT_PAREN, PLUS,
    MINUS, STAR, COMMA, DOT, SEMICOLON, SOURCE_END
  };

  Scanner scanner = scanner_init(input);
  Token* tokens = scanner_scan(&scanner);

  for (int i=0; i<scanner.parsed; i++) {
    ASSERT_EQ(correct_types[i], tokens[i].type);
  }

  for (int i=0; i<strlen(input); i++) {
    ASSERT_EQ(input[i], *tokens[i].lexeme);
  }
}

TEST(TestScanner, scanner_peek) {
  char input[] = "The quick brown fox jumped over the lazy cat.";
  Scanner scanner = scanner_init(input);

  for (int i=0; i<strlen(input); i++) {
    char next_char = scanner_peek(&scanner);
    scanner_advance(&scanner);
    EXPECT_EQ(input[i], next_char);
  }

}

TEST(TestScanner, scanner_match) {
  char input[] = "abcdefghijklmnop";
  Scanner scanner = scanner_init(input);

  for (int i=0; i<strlen(input); i++) {
      EXPECT_TRUE(scanner_match(&scanner, input[i]));
  }

}

TEST(TestScanner, ScanDoubleChar) {
  char input[] = "== != >= <=";

  TokenType correct_types[] = {EQUAL_EQUAL, BANG_EQUAL, GREATER_EQUAL, LESS_EQUAL, SOURCE_END};

  Scanner scanner = scanner_init(input);
  Token* tokens = scanner_scan(&scanner);

  for (int i=0; i<scanner.parsed; i++) {
    EXPECT_EQ(correct_types[i], tokens[i].type);
  }
}

