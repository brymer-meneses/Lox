#include "lox/scanner.h"
#include "gtest/gtest.h"

extern "C" {
  #include "string.h"
  #include "stdio.h"
  #include "tools/utils.h"
}

TEST(TestUtils, Substring) {
  const char str[46] = "The quick brown fox jumped over the lazy cat.";

  EXPECT_STREQ("The",     substring(str, 0,  2 ));
  EXPECT_STREQ("quick",   substring(str, 4,  8 ));
  EXPECT_STREQ("brown",   substring(str, 10, 14));
  EXPECT_STREQ("fox",     substring(str, 16, 18));
  EXPECT_STREQ("jumped",  substring(str, 20, 25));
  EXPECT_STREQ("over",    substring(str, 27, 30));
  EXPECT_STREQ("the",     substring(str, 32, 34));
  EXPECT_STREQ("lazy",    substring(str, 36, 39));
  EXPECT_STREQ("cat.",    substring(str, 41, 44));
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

TEST(TestUtils, IsBoolean) {
  EXPECT_TRUE(is_boolean("true"));
  EXPECT_TRUE(is_boolean("false"));
  EXPECT_FALSE(is_boolean("1"));
}

TEST(TestUtils, StringSplit) {
  const char* source = "The quick brown fox jumped over the lazy cat.";
  const char* str_arr[] = {"The", "quick", "brown", "fox", "jumped", "over",
    "the", "lazy", "cat." };

  char** result = str_split(source, " ");

  for (int i = 0; i< 8; i++) {
    EXPECT_STREQ(str_arr[i], result[i]);
  }

  free(result);
}

TEST(TestUtils, StringConcat) {
  const char* hello_world = "hello world";
  char* result = str_concat("hello ", "world");

  EXPECT_STREQ(hello_world,  result);
}

