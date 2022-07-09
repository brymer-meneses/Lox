#include "gtest/gtest.h"

extern "C" {
  #include "tools/hashtable.h"
  #include "string.h"
}

static HTValue cast(const char* val) {
  HTValue retval;
  retval.value.string = (char*) val;
  retval.isnull = false;
  return retval;
}

TEST(TestUtils, HashTable) {
  HashTable ht = ht_init();
 
  ht_insert(&ht, "zero",  cast("0"));
  ht_insert(&ht, "one",   cast("1"));
  ht_insert(&ht, "two",   cast("2"));
  ht_insert(&ht, "three", cast("3"));
  ht_insert(&ht, "four",  cast("4"));
  ht_insert(&ht, "five",  cast("5"));
  ht_insert(&ht, "six",   cast("6"));
  ht_insert(&ht, "seven", cast("7"));
  ht_insert(&ht, "eight", cast("8"));
  ht_insert(&ht, "nine",  cast("9"));
  ht_insert(&ht, "ten",   cast("10"));

  EXPECT_TRUE(strcmp("0",  ht_retrieve(&ht, "zero" ).value.string)  ==0);
  EXPECT_TRUE(strcmp("1",  ht_retrieve(&ht, "one"  ).value.string)  ==0);
  EXPECT_TRUE(strcmp("2",  ht_retrieve(&ht, "two"  ).value.string)  ==0);
  EXPECT_TRUE(strcmp("3",  ht_retrieve(&ht, "three").value.string)  ==0);
  EXPECT_TRUE(strcmp("4",  ht_retrieve(&ht, "four" ).value.string)  ==0);
  EXPECT_TRUE(strcmp("5",  ht_retrieve(&ht, "five" ).value.string)  ==0);
  EXPECT_TRUE(strcmp("6",  ht_retrieve(&ht, "six"  ).value.string)  ==0);
  EXPECT_TRUE(strcmp("7",  ht_retrieve(&ht, "seven").value.string)  ==0);
  EXPECT_TRUE(strcmp("8",  ht_retrieve(&ht, "eight").value.string)  ==0);
  EXPECT_TRUE(strcmp("9",  ht_retrieve(&ht, "nine" ).value.string)  ==0);
  EXPECT_TRUE(strcmp("10", ht_retrieve(&ht, "ten"  ).value.string)  ==0);
}
