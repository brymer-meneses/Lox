#include "gtest/gtest.h"

extern "C" {
  #include "lox/token.h"
  #include "lox/declarations.h"
  #include "string.h"
}



TEST(TestToken, ParseTokenToDouble) {
  EXPECT_EQ(1234,   token_parse_double( token_init(NUMBER, "1234", 1) ));
  EXPECT_EQ(3.1415, token_parse_double(  token_init(NUMBER, "3.1415", 1) ));
}

TEST(TestToken, ParseTokenToBool) {
  EXPECT_EQ(true,  token_parse_bool( token_init(TRUE,  "true",   1) ));
  EXPECT_EQ(false, token_parse_bool( token_init(FALSE, "false", 1) ));
}




