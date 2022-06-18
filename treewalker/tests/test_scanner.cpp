#include "gtest/gtest.h"

extern "C" {
  #include "scanner.h"
  #include "token.h"
  #include "stdio.h"
  #include "string.h"
}


TEST(TestScanner, TestRegisterToken) {
  char input[15] = "var test = 10";
  Scanner scanner = scanner_create(input);

  char lexeme_a[15] = "var";
  char lexeme_b[15] = "10";

  Token token_a = token_create(IDENTIFIER, lexeme_a, NULL);
  Token token_b = token_create(NUMBER, lexeme_b, (void*) 10);

  scanner_register_token(&scanner, token_a);
  scanner_register_token(&scanner, token_b);

  ASSERT_EQ(token_a.lexeme, scanner.tokens[0].lexeme);
  ASSERT_EQ(token_a.type, scanner.tokens[0].type);
  ASSERT_EQ(token_a.literal, scanner.tokens[0].literal);

  ASSERT_EQ(token_b.lexeme, scanner.tokens[1].lexeme);
  ASSERT_EQ(token_b.type, scanner.tokens[1].type);
  ASSERT_EQ(token_b.literal, scanner.tokens[1].literal);
}

TEST(TestScanner, Advance) {
  char input[14] = "var test = 10";
  Scanner scanner = scanner_create(input);

  for (int i=0; i<strlen(input); i++) {
    ASSERT_EQ(input[i], scanner_advance(&scanner));
  }
}

TEST(TestScanner, ScanSingleChar) {
  char input[12] = "{}()+-*,.;";
  TokenType correct_types[] = { 
    LEFT_BRACE, RIGHT_BRACE, LEFT_PAREN, RIGHT_PAREN, PLUS,
    MINUS, STAR, COMMA, DOT, SEMICOLON, FILE_EOF
  };

  Scanner scanner = scanner_create(input);
  Token* tokens = scanner_scan(&scanner);

  for (int i=0; i<scanner.parsed; i++) {
    ASSERT_EQ(correct_types[i], tokens[i].type);
  }

  for (int i=0; i<strlen(input); i++) {
    ASSERT_EQ(input[i], *tokens[i].lexeme);
  }
}

TEST(TestScanner, Peek) {
  char input[] = "The quick brown fox jumped over the lazy cat.";
  Scanner scanner = scanner_create(input);

  for (int i=0; i<strlen(input); i++) {
    char next_char = _peek(&scanner);
    scanner_advance(&scanner);
    EXPECT_EQ(input[i], next_char);
  }

}

TEST(TestScanner, Match) {
  char input[] = "abcdefghijklmnop";
  Scanner scanner = scanner_create(input);

  for (int i=0; i<strlen(input); i++) {
      EXPECT_TRUE(_match(&scanner, input[i+1]));
      scanner_advance(&scanner);
  }

}

TEST(TestScanner, ScanDoubleChar) {
  char input[] = "== != >= <=";

  TokenType correct_types[] = {EQUAL_EQUAL, BANG_EQUAL, GREATER_EQUAL, LESS_EQUAL, FILE_EOF};

  Scanner scanner = scanner_create(input);
  Token* tokens = scanner_scan(&scanner);

  for (int i=0; i<scanner.parsed; i++) {
    EXPECT_EQ(correct_types[i], tokens[i].type);
  }
}

