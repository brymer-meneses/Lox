#include "gtest/gtest.h"
#include <string.h>

extern "C" {
  #include "scanner.h"
  #include "token.h"
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

TEST(TestScanner, TestAdvance) {
  char input[14] = "var test = 10";
  Scanner scanner = scanner_create(input);

  for (int i=0; i<strlen(input); i++) {
    ASSERT_EQ(input[i], scanner_advance(&scanner));
  }
}
