#include "gtest/gtest.h"

extern "C" {
 #include "tools/fileloc.h"
}

TEST(TestUtils, FileLocRange) {
  
  FileLoc* fl = fileloc_range(2, fileloc_init(5, 3, 5), fileloc_init(10, 25, 30));

  EXPECT_EQ(10, fl->line);
  EXPECT_EQ(3, fl->start);
  EXPECT_EQ(30, fl->end);

}


