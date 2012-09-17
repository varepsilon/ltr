#include <gtest/gtest.h> 

// the name of the class here is the name of a test set
class ObviousTests : public ::testing::Test {
 protected:
  virtual void SetUp() { // doing something before a test run, e.g. setting variables
    a = 1; // initializing variables
    b = -2;
  }
  virtual void TearDown() { // doing something after a test run, e.g. free memory
  }

  int a, b; // variables used in all tests of the test set
};

TEST_F(ObviousTests, ComparisonTest) {
  EXPECT_TRUE(a > 0);  // variables a and b are already initialized in the ObviousTests class
  EXPECT_FALSE(b > 0); // and one can use them here without defining
  EXPECT_GT(a, b);
}

TEST_F(ObviousTests, SumTest) {
  EXPECT_EQ(-1, a + b);
}

TEST_F(ObviousTests, DivisionThrowTest) {
  EXPECT_NO_THROW(a / b);
}