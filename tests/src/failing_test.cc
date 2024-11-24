#include "another_test.h"

#include <gtest/gtest.h>

TEST(FailingTest, BasicAssertions) {
  EXPECT_STREQ("will", "fail");
}