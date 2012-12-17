// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include "ltr/optimization/functions/quadratic_function.h"

using optimization::Point;
using optimization::Matrix;
using optimization::QuadraticFunction;

TEST(QuadraticFunctionTest, QuadraticFunctionValueTest) {
  Matrix a(2, 2);

  a(0, 0) = 1.0;
  a(1, 0) = 2.0;
  a(0, 1) = 2.0;
  a(1, 1) = 3.0;

  QuadraticFunction testfunc(a);

  Point testpoint(2);
  testpoint(0) = 1.0;
  testpoint(1) = 2.0;

  EXPECT_EQ(21.0, testfunc.value(testpoint));
}

TEST(QuadraticFunctionTest, QuadraticFunctionGradientTest) {
  Matrix a(2, 2);

  a(0, 0) = 1.0;
  a(1, 0) = 2.0;
  a(0, 1) = 2.0;
  a(1, 1) = 3.0;

  QuadraticFunction testfunc(a);

  Point testpoint(2);
  testpoint[0] = 1.0;
  testpoint[1] = 2.0;

  Point result(2);
  result(0) = 10.0;
  result(1) = 16.0;

  EXPECT_EQ(result, testfunc.gradient(testpoint));
}

TEST(QuadraticFunctionTest, QuadraticFunctionHessianTest) {
  Matrix a(2, 2);

  a(0, 0) = 1.0;
  a(1, 0) = 2.0;
  a(0, 1) = 2.0;
  a(1, 1) = 3.0;

  QuadraticFunction testfunc(a);

  Point testpoint(2);
  testpoint(0) = 1.0;
  testpoint(1) = 2.0;


  EXPECT_EQ(a * 2.0, testfunc.hessian(testpoint));
}
