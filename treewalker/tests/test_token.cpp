#include "gtest/gtest.h"

extern "C" {
  #include "token.h"
  #include "declarations.h"
  #include "utils.h"
}

TEST(Token, testTokenArray) {
  TokenArray token_array = token_array_create();
  Token token = token_create(NUMBER, (char*) "test", (void*) 12);

  token_array_append(&token_array, token);
  ASSERT_EQ(INITIAL_TOKEN_ARRAY_SIZE, token_array.size);
}
