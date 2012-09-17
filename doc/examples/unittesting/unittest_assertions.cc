#include <gtest/gtest.h>

TEST(ObviousTests, ComparisonTest) {
  int a = 1;
  int b = -2;
  EXPECT_TRUE(a > 0);
  EXPECT_FALSE(b > 0);
  EXPECT_GT(a, b); // checks if a > b
}
// there could be more than one TEST macro in one file
TEST(ObviousTests, SumTest) {
  int a = 1;
  int b = -2;
  EXPECT_EQ(-1, a + b); // checks if parameters are equal
}

TEST(ObviousTests, DivisionThrowTest) {
  int a = 1;
  int b = -2;
  EXPECT_NO_THROW(a / b); // checks if while processing the command in brackets no exceptions were thrown
  // there is also an EXPECT_ANY_THROW macro, which checks the opposite
}