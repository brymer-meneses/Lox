#include "gtest/gtest.h"

extern "C" {

  #include "string.h"
  #include "stdio.h"
  #include "strutils.h"
}

TEST(TestUtils, TestSubstring) {
  char str[] = "one two three";

  ASSERT_TRUE(strcmp("one", substring(str, 0, 2)) == 0);
  ASSERT_TRUE(strcmp("one", substring(str, 0, 2)) == 0);
  ASSERT_TRUE(strcmp("two", substring(str,  4, 6)) == 0);
  
}
