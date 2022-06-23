#include "gtest/gtest.h"

extern "C" {
  #include "lox/token.h"
  #include "lox/declarations.h"
  #include "lox/literal.h"
  #include "string.h"
}


TEST(TestLiteral, ParseLiteralToDouble) {
  EXPECT_EQ(1234, literal_parse_double("1234"));
  EXPECT_EQ(3.1415, literal_parse_double("3.1415"));
}

TEST(TestLiteral, ParseLiteralToBool) {
  EXPECT_EQ(true, literal_parse_bool("true"));
  EXPECT_EQ(false, literal_parse_bool("false"));
}

TEST(TestLiteral, GetType) {
  EXPECT_EQ(LOX_BOOLEAN, literal_get_type("true"));
  EXPECT_EQ(LOX_STRING, literal_get_type("hi there"));
  EXPECT_EQ(LOX_NUMBER, literal_get_type("3.1415"));
}

