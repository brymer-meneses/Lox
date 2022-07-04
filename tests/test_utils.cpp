#include "lox/scanner.h"
#include "gtest/gtest.h"

extern "C" {
  #include "string.h"
  #include "stdio.h"
  #include "tools/utils.h"
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

TEST(TestUtils, StringSplit) {
  const char* source = "The quick brown fox jumped over the lazy cat.";
  const char* str_arr[] = {"The", "quick", "brown", "fox", "jumped", "over",
    "the", "lazy", "cat." };

  char** result = str_split(source, " ");

  for (int i = 0; i< 8; i++) {
    EXPECT_TRUE(strcmp(str_arr[i], result[i]) == 0);
  }

  free(result);
}

TEST(TestUtils, StringConcat) {
  const char* hello_world = "hello world";
  char* result = str_concat("hello ", "world");

  EXPECT_TRUE(strcmp(hello_world,  result) == 0);
}


TEST(TestUtils, StringFormat) {
  const char* source = "hello world";
  const char* result = str_format(1, "hello ", "world");

  EXPECT_TRUE(strcmp(source, result) == 0) << result;
}
