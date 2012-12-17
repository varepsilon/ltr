// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include "ltr/optimization/functions/linear_function.h"

using optimization::Point;
using optimization::LinearFunction;

TEST(LinearFunctionTest, LinearFunctionValueTest) {
  Point a(2);
  a(0) = 1.0;
  a(1) = 2.0;

  double b = 3.0;

  LinearFunction testfunc(a, b);

  Point testpoint(2);
  testpoint(0) = 1.0;
  testpoint(1) = 2.0;

  EXPECT_EQ(8.0, testfunc.value(testpoint));
}

TEST(LinearFunctionTest, LinearFunctionGradientTest) {
  Point a(2);
  a(0) = 1.0;
  a(1) = 2.0;

  double b = 3.0;

  LinearFunction testfunc(a, b);

  Point testpoint(2);
  testpoint(0) = 1.0;
  testpoint(1) = 2.0;

  EXPECT_EQ(a, testfunc.gradient(testpoint));
}
