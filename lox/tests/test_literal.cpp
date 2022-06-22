#include "gtest/gtest.h"

extern "C" {
  #include "lox/token.h"
  #include "lox/declarations.h"
  #include "lox/literal.h"
  #include "string.h"
}


TEST(TestLiteral, ParseLiteralToDouble) {
  const double expected_result = 123;
  const double parsed_result = literal_parse_double("123");

  EXPECT_EQ(expected_result, parsed_result);


}

