#include "gtest/gtest.h"

extern "C" {
  #include "lox/token.h"
  #include "lox/core.h"
  #include "lox/object.h"
  #include "lox/fileloc.h"
  #include "tools/utils.h"
  #include "string.h"
}



// TEST(TestLoxObject, ParseToDouble) {
//   EXPECT_EQ(1234,    encode_double(1234).data.number);
//   EXPECT_EQ(3.1415,  encode_double(3.1415).data.number);
// }
//
// TEST(TestLoxObject, ParseToBool) {
//   EXPECT_EQ(true,    encode_bool(true).data.boolean);
//   EXPECT_EQ(false,  encode_bool(false).data.boolean);
// }
//
// TEST(TestLoxObject, ParseToString) {
//   EXPECT_EQ("hi",    encode_string("hi").data.string);
//   EXPECT_EQ("hello",  encode_string("hello").data.string);
// }
//
//
//
//
