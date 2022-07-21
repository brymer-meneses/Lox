#include "gtest/gtest.h"
#include <cstdlib>

extern "C" {
  #include "tools/hashmap.h"
  #include "lox/token.h"
  #include "string.h"
}


void hashmap__setstr(Hashmap* hmap, const char* key, const char* value) {
  void* data = strdup(value);

  hashmap__insert(hmap, strdup(key), data);
}

char* hashmap__getstr(Hashmap* hmap, const char* key) {
  void* data = hashmap__retrieve(hmap, strdup(key));
  return (char*) data;
}

TEST(TestUtils, HashMap) {
  Hashmap* hm = hashmap__init();

  const char* keys[] = {"one", "two", "three", "four", "five", "six"};
  const char* vals[] = {"1", "2", "3", "4", "5", "6"};

  const unsigned int length = sizeof(keys)/sizeof(char*);

  for (unsigned int i=0; i<length; i++) {
    hashmap__setstr(hm, keys[i], vals[i]);
  }

  for (unsigned int i=0; i<length; i++) {
    EXPECT_STREQ(vals[i], hashmap__getstr(hm, keys[i]));
  }

  hashmap__free(hm);
}
