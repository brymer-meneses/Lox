#include "gtest/gtest.h"

extern "C" {
  #include "lox/token.h"
  #include "lox/declarations.h"
  #include "lox/filelocation.h"
  #include "tools/utils.h"
  #include "string.h"
}



TEST(TestToken, ParseTokenToDouble) {
  const FileLoc fl = fileloc_init(1, 1, 1);
  EXPECT_EQ(1234,   token_parse_double( token_init(NUMBER, "1234", fl) ));
  EXPECT_EQ(3.1415, token_parse_double(  token_init(NUMBER, "3.1415", fl) ));
}

TEST(TestToken, ParseTokenToBool) {
  const FileLoc fl = fileloc_init(1, 1, 1);
  EXPECT_EQ(true,  token_parse_bool( token_init(TRUE,  "true",   fl) ));
  EXPECT_EQ(false, token_parse_bool( token_init(FALSE, "false", fl) ));
}




