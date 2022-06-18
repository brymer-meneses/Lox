#include "gtest/gtest.h"

extern "C" {
  #include "string.h"
  #include "stdio.h"
  #include "strutils.h"
}

TEST(TestUtils, Substring) {
  const char str[46] = "The quick brown fox jumped over the lazy cat.";

  ASSERT_TRUE(strcmp("The",     substring(str, 0, 2))   ==0);
  ASSERT_TRUE(strcmp("quick",   substring(str, 4, 8))   ==0);
  ASSERT_TRUE(strcmp("brown",   substring(str, 10, 14)) ==0);
  ASSERT_TRUE(strcmp("fox",     substring(str, 16, 18)) ==0);
  ASSERT_TRUE(strcmp("jumped",  substring(str, 20, 25)) ==0);
  ASSERT_TRUE(strcmp("over",    substring(str, 27, 30)) ==0);
  ASSERT_TRUE(strcmp("the",     substring(str, 32, 34)) ==0);
  ASSERT_TRUE(strcmp("lazy",    substring(str, 36, 39)) ==0);
  ASSERT_TRUE(strcmp("cat.",    substring(str, 41, 44)) ==0);
}
