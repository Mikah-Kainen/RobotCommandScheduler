#include "another_test.h"

#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(AnotherTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STREQ("another", "another");
  // Expect equality.
  EXPECT_NE(SIX, SEVEN);
}