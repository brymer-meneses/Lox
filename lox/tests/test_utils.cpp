#include "gtest/gtest.h"

extern "C" {
  #include "string.h"
  #include "stdio.h"
  #include "lox/utils.h"
}

TEST(TestUtils, Substring) {
  const char str[46] = "The quick brown fox jumped over the lazy cat.";

  EXPECT_TRUE(strcmp("The",     substring(str, 0, 2))   ==0);
  EXPECT_TRUE(strcmp("quick",   substring(str, 4, 8))   ==0);
  EXPECT_TRUE(strcmp("brown",   substring(str, 10, 14)) ==0);
  EXPECT_TRUE(strcmp("fox",     substring(str, 16, 18)) ==0);
  EXPECT_TRUE(strcmp("jumped",  substring(str, 20, 25)) ==0);
  EXPECT_TRUE(strcmp("over",    substring(str, 27, 30)) ==0);
  EXPECT_TRUE(strcmp("the",     substring(str, 32, 34)) ==0);
  EXPECT_TRUE(strcmp("lazy",    substring(str, 36, 39)) ==0);
  EXPECT_TRUE(strcmp("cat.",    substring(str, 41, 44)) ==0);
}

TEST(TestUtils, IsRealNumber) {
  EXPECT_TRUE(is_real_number("1"));
  EXPECT_TRUE(is_real_number("-2.0"));
  EXPECT_TRUE(is_real_number("-3.1415"));
  EXPECT_TRUE(is_real_number("5"));

  EXPECT_FALSE(is_real_number("true"));
  EXPECT_FALSE(is_real_number("--123"));
  EXPECT_FALSE(is_real_number("2.718.28"));
  EXPECT_FALSE(is_real_number("210-28"));
  EXPECT_FALSE(is_real_number("hello there"));
  EXPECT_FALSE(is_real_number("-1.234 this is a test!"));
}
