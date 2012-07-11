// Copyright 2012 Yandex School Practice

#include <stdlib.h>

#include "gtest/gtest.h"

#include "ltr/utility/numerical.h"

using ltr::utility::DOUBLE_EPS;
using ltr::utility::DoubleMore;
using ltr::utility::DoubleLess;
using ltr::utility::DoubleEqual;
using ltr::utility::DoubleMoreOrEqual;
using ltr::utility::DoubleLessOrEqual;
using ltr::utility::DoubleNotEqual;
using ltr::utility::DoubleEqualWithNaN;
using ltr::utility::Inf;
using ltr::utility::isNaN;
using ltr::utility::NaN;

const double precision = 2 * DOUBLE_EPS;

TEST(NumericalTest, isNaNTest) {
  double nan = NaN;
  EXPECT_TRUE(isNaN(nan));
}

TEST(NumericalTest, EqualWithNaNTest) {
  double nan1 = NaN;
  double nan2 = NaN;
  EXPECT_TRUE(DoubleEqualWithNaN(nan1, nan2));

  double number1 = 2;
  double number2 = 10;
  EXPECT_FALSE(DoubleEqualWithNaN(nan1, number1));
  EXPECT_FALSE(DoubleEqualWithNaN(number1, number2));
  EXPECT_TRUE(DoubleEqualWithNaN(number1, number1));
}

TEST(NumericalTest, DoubleMoreTest) {
  double lhs = 1337.1337;
  double rhs = lhs;
  EXPECT_FALSE(DoubleMore(lhs, rhs));

  rhs = lhs - precision * lhs;
  EXPECT_TRUE(DoubleMore(lhs, rhs));

  rhs = lhs + precision * lhs;
  EXPECT_FALSE(DoubleMore(lhs, rhs));

  rhs = lhs - Inf;
  EXPECT_TRUE(DoubleMore(lhs, rhs));

  rhs = lhs + Inf;
  EXPECT_FALSE(DoubleMore(lhs, rhs));
}

TEST(NumericalTest, DoubleLessTest) {
  double lhs = 1337.1337;
  double rhs = lhs;
  EXPECT_FALSE(DoubleLess(lhs, rhs));

  rhs = lhs + precision * lhs;
  EXPECT_TRUE(DoubleLess(lhs, rhs));

   rhs = lhs + Inf;
  EXPECT_TRUE(DoubleLess(lhs, rhs));
}

TEST(NumericalTest, DoubleEqualTest) {
  double lhs = 1337.1337;
  double rhs = lhs;
  EXPECT_TRUE(DoubleEqual(lhs, rhs));

  rhs = lhs + precision * lhs;
  EXPECT_FALSE(DoubleEqual(lhs, rhs));

  rhs = lhs - Inf;
  EXPECT_FALSE(DoubleEqual(lhs, rhs));
}

TEST(NumericalTest, DoubleMoreOrEqualTest) {
  double lhs = 1337.1337;
  double rhs = lhs;
  EXPECT_TRUE(DoubleMoreOrEqual(lhs, rhs));

  rhs = lhs + precision * lhs;
  EXPECT_FALSE(DoubleMoreOrEqual(lhs, rhs));

  rhs = lhs - precision * lhs;
  EXPECT_TRUE(DoubleMoreOrEqual(lhs, rhs));

  rhs = lhs + Inf;
  EXPECT_FALSE(DoubleMoreOrEqual(lhs, rhs));

  rhs = lhs - Inf;
  EXPECT_TRUE(DoubleMoreOrEqual(lhs, rhs));
}

TEST(NumericalTest, DoubleLessOrEqualTest) {
  double lhs = 1337.1337;
  double rhs = lhs;
  EXPECT_TRUE(DoubleLessOrEqual(lhs, rhs));

  rhs = lhs + precision * lhs;
  EXPECT_TRUE(DoubleLessOrEqual(lhs, rhs));

  rhs = lhs - precision * lhs;
  EXPECT_FALSE(DoubleLessOrEqual(lhs, rhs));

  rhs = lhs + Inf;
  EXPECT_TRUE(DoubleLessOrEqual(lhs, rhs));

  rhs = lhs - Inf;
  EXPECT_FALSE(DoubleLessOrEqual(lhs, rhs));
}

TEST(NumericalTest, DoubleNotEqualTest) {
  double lhs = 1337.1337;
  double rhs = lhs;

  EXPECT_FALSE(DoubleNotEqual(lhs, rhs));

  rhs = lhs + precision * lhs;
  EXPECT_TRUE(DoubleNotEqual(lhs, rhs));

  rhs = lhs - precision * lhs;
  EXPECT_TRUE(DoubleNotEqual(lhs, rhs));

  rhs = lhs + Inf;
  EXPECT_TRUE(DoubleNotEqual(lhs, rhs));

  rhs = lhs - Inf;
  EXPECT_TRUE(DoubleNotEqual(lhs, rhs));
}
