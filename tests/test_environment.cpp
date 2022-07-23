#include "gtest/gtest.h"

extern "C" {
  #include "lox/environment.h"
  #include "tools/hashmap.h"
  #include "lox/object.h"
  #include "lox/token.h"
  #include "lox/lox.h"
}

TEST(TestEnvionment, Define) {

  Environment* env = environment__init(NULL);
  environment__define(env, (char*) "test", loxobject__number_init(5, NULL));

  void* retval = hashmap__retrieve(env->values, (char*) "test");
  LoxObject* obj = (LoxObject*) retval;

  EXPECT_EQ(obj->as.number, 5);
}
