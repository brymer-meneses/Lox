#include "gtest/gtest.h"

extern "C" {
  #include "lox/environment.h"
  #include "tools/hashmap.h"
  #include "lox/object.h"
  #include "lox/token.h"
  #include "lox/lox.h"
}

TEST(TestEnvionment, Define) {

  environment_init();
  environment_define((char*) "test", loxobject_number(5, NULL));

  void* retval = hashmap_retrieve(lox.environment.values, (char*) "test");
  LoxObject* obj = (LoxObject*) retval;

  EXPECT_EQ(obj->data.number, 5);

  environment_dump();

}
