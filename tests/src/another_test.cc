#include "another_test.h"

#include <gtest/gtest.h>

TEST(AnotherTest, BasicAssertions) {
  EXPECT_STREQ("another", "another");
  EXPECT_NE(SIX, SEVEN);
}