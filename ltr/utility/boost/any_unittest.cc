// Copyright 2012 Yandex

#include "gtest/gtest.h"
#include <vector>

#include "ltr/utility/boost/any.h"

using ltr::utility::Any;
using ltr::utility::any_cast;
using std::vector;


TEST(AnyTest, TestingScalars) {
  Any a = 5;
  Any b = 1.1;
  EXPECT_EQ(any_cast<int>(a), 5);
  EXPECT_EQ(any_cast<double>(b), 1.1);

  EXPECT_ANY_THROW(any_cast<int>(b));
  EXPECT_ANY_THROW(any_cast<double>(a));
  EXPECT_ANY_THROW(any_cast<vector<int> >(a));
  EXPECT_ANY_THROW(any_cast<vector<Any> >(a));
}

TEST(AnyTest, TestingVectors) {
  Any a = vector<int>(2, 1);
  Any b = vector<Any>(2, Any(1));

  EXPECT_EQ(any_cast<vector<int> >(b), vector<int>(2, 1));
  EXPECT_EQ(any_cast<vector<int> >(a), any_cast<vector<int> >(b));

  EXPECT_ANY_THROW(any_cast<vector<double> >(a));
  EXPECT_ANY_THROW(any_cast<vector<double> >(b));
  EXPECT_ANY_THROW(any_cast<int>(a));
  EXPECT_ANY_THROW(any_cast<int>(b));
}
