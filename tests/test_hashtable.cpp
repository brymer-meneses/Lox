#include "gtest/gtest.h"

extern "C" {
  #include "tools/hashtable.h"
}

// TEST(TestUtils, HashTable) {
//   HashTable ht = ht_init();
//   
//   ht_insert(&ht, "zero",  "0");
//   ht_insert(&ht, "one",  "1");
//   ht_insert(&ht, "two",  "2");
//   ht_insert(&ht, "three",  "3");
//   ht_insert(&ht, "four",  "4");
//   ht_insert(&ht, "five",  "5");
//   ht_insert(&ht, "six",  "6");
//   ht_insert(&ht, "seven",  "7");
//   ht_insert(&ht, "eight",  "8");
//   ht_insert(&ht, "nine",  "9");
//   ht_insert(&ht, "ten",  "10");
//
//   EXPECT_TRUE(strcmp("0", ht_retrieve(ht, "zero"))==0);
//   EXPECT_TRUE(strcmp("1", ht_retrieve(ht, "two"))==0);
//   EXPECT_EQ("one", ht_retrieve(&ht, "1"));
//   EXPECT_EQ("two", ht_retrieve(&ht, "2"));
//   EXPECT_EQ("three", ht_retrieve(&ht, "3"));
//   EXPECT_EQ("four", ht_retrieve(&ht, "4"));
// }
