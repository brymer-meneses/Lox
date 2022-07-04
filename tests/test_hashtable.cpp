#include "gtest/gtest.h"

extern "C" {
  #include "tools/hashtable.h"
}

TEST(TestUtils, HashTable) {
  HashTable ht = ht_init();

  char key[] = "hello";
  char value[] = "world";

  ht_insert(&ht, key, value);

  EXPECT_EQ(value, ht_retrieve(&ht, key));
  EXPECT_EQ(NULL,  ht_retrieve(&ht, CAST("NONE")));
}
