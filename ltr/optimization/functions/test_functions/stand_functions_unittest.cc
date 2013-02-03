// Copyright 2012 Yandex

#include <gtest/gtest.h>
#include "ltr/optimization/functions/test_functions/unconstrained/sum_squares_function/sum_squares_function.h"
#include "ltr/optimization/functions/test_functions/unconstrained/easom_function/easom_function.h"
#include "ltr/optimization/functions/test_functions/unconstrained/rosenbrock_function/rosenbrock_function.h"
#include "ltr/optimization/functions/test_functions/unconstrained/griewank_function/griewank_function.h"
#include "ltr/optimization/functions/test_functions/unconstrained/trid_function/trid_function.h"

using optimization::Point;
using optimization::Vector;
using optimization::Matrix;
using optimization::SumSquaresFunction;
using optimization::EasomFunction;
using optimization::RosenbrockFunction;
using optimization::GriewankFunction;
using optimization::TridFunction;

TEST(SumSquaresFunctionTest, SumSquaresFunctionValueTest) {
  SumSquaresFunction test(4);

  Point point(4);
  point(0) = 1.0;
  point(1) = 1.0;
  point(2) = 1.0;
  point(3) = 1.0;

  EXPECT_EQ(10.0, test.value(point));

  SumSquaresFunction test1(2);

  Point point1(2);
  point1(0) = -1.0;
  point1(1) = 3.0;

  EXPECT_EQ(19.0, test1(point1));
}
TEST(SumSquaresFunctionTest, SumSquaresFunctionGradientTest) {
  SumSquaresFunction test(4);

  Point point(4);
  point(0) = 1.0;
  point(1) = 1.0;
  point(2) = 1.0;
  point(3) = 1.0;

  Point gradient(4);
  gradient(0) = 2.0;
  gradient(1) = 4.0;
  gradient(2) = 6.0;
  gradient(3) = 8.0;

  EXPECT_TRUE(gradient.isApprox(test.gradient(point), 0.001));

  SumSquaresFunction test1(2);

  Point point1(2);
  point1(0) = -1.0;
  point1(1) = 3.0;

  Point gradient1(2);
  gradient1(0) = -2.0;
  gradient1(1) = 12.0;

  EXPECT_TRUE(gradient1.isApprox(test1.gradient(point1), 0.001));
}
TEST(SumSquaresFunctionTest, SumSquaresFunctionHessianTest) {
  SumSquaresFunction test(4);

  Point point(4);
  point(0) = 1.0;
  point(1) = 1.0;
  point(2) = 1.0;
  point(3) = 1.0;

  Matrix hessian(4, 4);
  hessian.setZero();
  hessian(0, 0) = 2.0;
  hessian(1, 1) = 4.0;
  hessian(2, 2) = 6.0;
  hessian(3, 3) = 8.0;

  EXPECT_TRUE(hessian.isApprox(test.hessian(point), 0.001));
}
TEST(SumSquaresFunctionTest, SumSquaresExceptionTest) {
  EXPECT_ANY_THROW(SumSquaresFunction test(0));
  EXPECT_NO_THROW(SumSquaresFunction test(2));
}

TEST(EasomFunctionTest, EasomFunctionValueTest) {
  EasomFunction test;
  Point point(2);
  point(0) = 3.141592653589797;
  point(1) = 3.141592653589797;

  EXPECT_EQ(-1.0, test.value(point));
}
TEST(EasomFunctionTest, EasomFunctionGradientTest) {
  EasomFunction test;
  Point point(2);
  const double PI = 3.141592653589797;
  point(0) = PI;
  point(1) = PI;

  EXPECT_TRUE(test.gradient(point).isZero(0.001));
}
TEST(EasomFunctionTest, EasomFunctionHessianTest) {
  EasomFunction test;
  Point point(2);
  const double PI = 3.141592653589797;
  point(0) = PI;
  point(1) = PI;
  Matrix hessian(2, 2);
  hessian.setZero();
  hessian(0, 0) = 3.0;
  hessian(1, 1) = 3.0;

  EXPECT_TRUE(hessian.isApprox(test.hessian(point), 0.001));
}

TEST(RosenbrockFunctionTest, RosenbrockFunctionValueTest) {
  RosenbrockFunction test(4);
  Point point(4);
  point.setZero();

  EXPECT_EQ(3.0, test.value(point));
}
TEST(RosenbrockFunctionTest, RosenbrockFunctionGradientTest) {
  RosenbrockFunction test(4);
  Point point(4);
  point.setOnes();
  Vector gradient(4);
  gradient.setZero();

  EXPECT_TRUE(gradient.isApprox(test.gradient(point), 0.001));
}
TEST(RosenbrockFunctionTest, RosenbrockFunctionHessianTest) {
  RosenbrockFunction test(2);

  Point point(2);
  point.setOnes();

  Matrix hessian(2, 2);
  hessian(0, 0) = 802.0;
  hessian(1, 0) = -400.0;
  hessian(0, 1) = -400.0;
  hessian(1, 1) = 200.0;

  EXPECT_TRUE(hessian.isApprox(test.hessian(point), 0.001));

  RosenbrockFunction test1(3);

  Point point1(3);
  point1.setOnes();

  Matrix hessian1(3, 3);
  hessian1.setZero();

  hessian1(0, 0) = 802.0;
  hessian1(1, 1) = 1002.0;
  hessian1(2, 2) = 200.0;
  hessian1(1, 0) = -400.0;
  hessian1(0, 1) = -400.0;
  hessian1(2, 1) = -400.0;
  hessian1(1, 2) = -400.0;

  EXPECT_TRUE(hessian1.isApprox(test1.hessian(point1), 0.001));
}

TEST(GriewankFunctionTest, GriewankFunctionValueTest) {
  GriewankFunction test(5);
  Point point(5);
  point.setZero();

  EXPECT_EQ(0.0, test.value(point));

  const double PI = 3.141592653589797;
  GriewankFunction test1(2);
  Point point1(2);
  point1(0) = 0.0;
  point1(1) = PI * pow(2, 0.5);
  double value = pow(PI, 2)/2000.0 + 2.0;

  EXPECT_EQ(value, test1.value(point1));
}
TEST(GriewankFunctionTest, GriewankFunctionGradientTest) {
  GriewankFunction test(5);
  Point point(5);
  point.setZero();
  Vector gradient(5);
  gradient.setZero();

  EXPECT_TRUE(gradient.isApprox(test.gradient(point), 0.001));
}
TEST(GriewankFunctionTest, GriewankFunctionHessianTest) {
  GriewankFunction test(2);
  Point point(2);
  point.setZero();
  Matrix hessian(2, 2);
  hessian.setZero();
  hessian(0, 0) = 1.0/2000.0 + 1.0;
  hessian(1, 1) = hessian(0, 0)/2.0;
  hessian(1, 0) = 0.0;
  hessian(0, 1) = 0.0;

  EXPECT_TRUE(hessian.isApprox(test.hessian(point), 0.001));
}

TEST(TridFunctionTest, TridFunctionValueTest) {
  TridFunction test(4);

  Point point(4);
  point(0) = 0.0;
  point(1) = 1.0;
  point(2) = 2.0;
  point(3) = 3.0;

  EXPECT_EQ(-2.0, test.value(point));
}
TEST(TridFunctionTest, TridFunctionGradientTest) {
  TridFunction test(4);

  Point point(4);
  point(0) = 0.0;
  point(1) = 1.0;
  point(2) = 2.0;
  point(3) = 3.0;

  Vector gradient(4);
  gradient(0) = -3.0;
  gradient(1) = -2.0;
  gradient(2) = -2.0;
  gradient(3) = 2.0;

  EXPECT_TRUE(gradient.isApprox(test.gradient(point), 0.001));

  TridFunction test1(4);

  Point point1(4);
  point1(0) = 4.0;
  point1(1) = 6.0;
  point1(2) = 6.0;
  point1(3) = 4.0;

  Vector gradient1(4);
  gradient1(0) = 0.0;
  gradient1(1) = 0.0;
  gradient1(2) = 0.0;
  gradient1(3) = 0.0;

  EXPECT_TRUE(gradient1.isApprox(test1.gradient(point1), 0.001));
}
TEST(TridFunctionTest, TridFunctionHessianTest) {
  TridFunction test(4);

  Point point(4);
  point(0) = 123.0;
  point(1) = 23.0;
  point(2) = 8237.0;
  point(3) = 3.0;

  Matrix hessian(4, 4);
  hessian.setZero();
  hessian(0, 0) = 2.0;
  hessian(1, 1) = 2.0;
  hessian(2, 2) = 2.0;
  hessian(3, 3) = 2.0;
  hessian(0, 1) = -1.0;
  hessian(1, 0) = -1.0;
  hessian(1, 2) = -1.0;
  hessian(2, 1) = -1.0;
  hessian(2, 3) = -1.0;
  hessian(3, 2) = -1.0;

  EXPECT_TRUE(hessian.isApprox(test.hessian(point), 0.001));
}
TEST(TridFunctionTest, TridFunctionExceptionTest) {
  EXPECT_ANY_THROW(TridFunction test(1));
  EXPECT_NO_THROW(TridFunction test(2));
}
