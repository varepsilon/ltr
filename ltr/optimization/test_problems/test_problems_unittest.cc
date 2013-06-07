// Copyright 2013 Yandex

#include <gtest/gtest.h>
#include "ltr/optimization/test_problems/constrained_functions.h"
#include "ltr/optimization/test_problems/unconstrained_functions.h"

namespace optimization {
const double PI = 3.141592653589797;

TEST(G1FunctionTest, G1FunctionValueTest) {
  G1Function g1_function;
  Point point(13);
  point << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;

  EXPECT_DOUBLE_EQ(g1_function.computeValue(point), 0.0);

  point << 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12;

  EXPECT_DOUBLE_EQ(g1_function.computeValue(point), -112.0);
}
TEST(G1FunctionTest, G1FunctionGradientTest) {
  G1Function g1_function;
  Point point(13);
  Vector gradient(13);
  Vector true_gradient(13);

  point << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
  true_gradient << 5, 5, 5, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1;
  g1_function.computeGradient(point, &gradient);
  EXPECT_TRUE(gradient.isApprox(true_gradient, 0.01));

  point << 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12;
  true_gradient << 5, -5, -15, -25, -1, -1, -1, -1, -1, -1, -1, -1, -1;
  g1_function.computeGradient(point, &gradient);
  EXPECT_TRUE(gradient.isApprox(true_gradient, 0.01));
}
TEST(G1FunctionTest, G1FunctionHessianTest) {
  G1Function g1_function;
  Point point(13);
  Matrix hessian(13, 13);
  Matrix true_hessian(13, 13);
  Vector diag(13);
  diag << -10, -10, -10, -10, 0, 0, 0, 0, 0, 0, 0, 0, 0;

  point << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;

  true_hessian.setZero();
  for (int coordinate = 0; coordinate < 13; ++coordinate) {
    true_hessian(coordinate, coordinate) = diag(coordinate);
  }

  g1_function.computeHessian(point, &hessian);
  EXPECT_TRUE(hessian.isApprox(true_hessian, 0.01));
}

TEST(G3FunctionTest, G3FunctionValueTest) {
  G3Function g3_function(5);
  Point point(5);
  double coefficient = pow(5.0, 1.0 / 5.0);

  point << 1, 1, 1, 1, 1;
  EXPECT_DOUBLE_EQ(g3_function.computeValue(point), coefficient);

  point << 1, 2, 3, 4, 5;
  EXPECT_DOUBLE_EQ(g3_function.computeValue(point), coefficient* 120.0);
}
TEST(G3FunctionTest, G3FunctionGradientTest) {
  G3Function g3_function(5);
  Point point(5);
  Vector true_gradient(5);
  Vector gradient;
  double coefficient = pow(5.0, 1.0 / 5.0);

  point << 1, 2, 3, 4, 5;
  true_gradient << 120, 60, 40, 30, 24;
  true_gradient *= coefficient;

  g3_function.computeGradient(point, &gradient);
  EXPECT_TRUE(gradient.isApprox(true_gradient, 0.01));

  point << 1, 0, 1, 0, 1;
  true_gradient << 0, 0, 0, 0, 0;
  true_gradient *= coefficient;

  g3_function.computeGradient(point, &gradient);
  EXPECT_TRUE(gradient.isApprox(true_gradient, 0.01));
}
TEST(G3FunctionTest, G3FunctionHessianTest) {
  G3Function g3_function(5);
  Point point(5);
  Matrix true_hessian(5, 5);
  Matrix hessian;
  double coefficient = pow(5.0, 1.0 / 5.0);

  point << 1, 2, 3, 4, 5;
  true_hessian <<
    0, 60, 40, 30, 24,
    60, 0, 20, 15, 12,
    40, 20, 0, 10, 8,
    30, 15, 10, 0, 6,
    24, 12, 8, 6, 0;

  true_hessian *= coefficient;
  g3_function.computeHessian(point, &hessian);
  EXPECT_TRUE(hessian.isApprox(true_hessian, 0.01));
}

TEST(SumSquaresFunctionTest, SumSquaresFunctionValueTest) {
  SumSquaresFunction sum_squares_function(4);

  Point point(4);
  point << 1, 1, 1, 1;

  EXPECT_EQ(10.0, sum_squares_function.computeValue(point));

  SumSquaresFunction sum_squares_function1(2);

  Point point1(2);
  point1 << -1, 3;

  EXPECT_EQ(19.0, sum_squares_function1(point1));
}
TEST(SumSquaresFunctionTest, SumSquaresFunctionGradientTest) {
  SumSquaresFunction sum_squares_function(4);

  Point point(4);
  Vector gradient(4);
  Vector true_gradient(4);

  point << 1, 1, 1, 1;
  true_gradient << 2, 4, 6, 8;
  sum_squares_function.computeGradient(point, &gradient);

  EXPECT_TRUE(gradient.isApprox(true_gradient, 0.01));

  SumSquaresFunction sum_squares_function1(2);

  Point point1(2);
  Vector gradient1(2);
  Vector true_gradient1(2);

  point1 << -1, 3;
  true_gradient1 << -2, 12;
  sum_squares_function1.computeGradient(point1, &gradient1);

  EXPECT_TRUE(gradient1.isApprox(true_gradient1, 0.01));
}
TEST(SumSquaresFunctionTest, SumSquaresFunctionHessianTest) {
  SumSquaresFunction sum_squares_function(4);

  Point point(4);
  Matrix hessian(4, 4);
  Matrix true_hessian(4, 4);

  point << 1, 1, 1, 1;
  true_hessian.setZero();
  true_hessian(0, 0) = 2.0;
  true_hessian(1, 1) = 4.0;
  true_hessian(2, 2) = 6.0;
  true_hessian(3, 3) = 8.0;

  sum_squares_function.computeHessian(point, &hessian);

  EXPECT_TRUE(hessian.isApprox(true_hessian, 0.01));
}
TEST(SumSquaresFunctionTest, SumSquaresExceptionTest) {
  EXPECT_ANY_THROW(SumSquaresFunction sum_squares_function(0));
  EXPECT_NO_THROW(SumSquaresFunction sum_squares_function(2));
}

TEST(EasomFunctionTest, EasomFunctionValueTest) {
  EasomFunction easom_function;
  Point point(2);
  point << PI, PI;

  EXPECT_EQ(-1.0, easom_function.computeValue(point));
}
TEST(EasomFunctionTest, EasomFunctionGradientTest) {
  EasomFunction easom_function;
  Point point(2);
  Vector gradient(2);
  Vector true_gradient(2);

  point << PI, PI;

  easom_function.computeGradient(point, &gradient);

  EXPECT_TRUE(gradient.isZero());
}
TEST(EasomFunctionTest, EasomFunctionHessianTest) {
  EasomFunction easom_function;
  Point point(2);
  Matrix hessian(2, 2);
  Matrix true_hessian(2, 2);

  point << PI, PI;
  true_hessian << 3, 0,
                  0, 3;
  easom_function.computeHessian(point, &hessian);

  EXPECT_TRUE(hessian.isApprox(true_hessian, 0.01));
}

TEST(RosenbrockFunctionTest, RosenbrockFunctionValueTest) {
  RosenbrockFunction rosenbrock_function(4);
  Point point(4);
  point.setZero();

  EXPECT_EQ(3.0, rosenbrock_function.computeValue(point));
}
TEST(RosenbrockFunctionTest, RosenbrockFunctionGradientTest) {
  RosenbrockFunction rosenbrock_function(4);
  Point point(4);
  Vector gradient(4);

  point.setOnes();
  rosenbrock_function.computeGradient(point, &gradient);
  EXPECT_TRUE(gradient.isZero());
}
TEST(RosenbrockFunctionTest, RosenbrockFunctionHessianTest) {
  RosenbrockFunction rosenbrock_function(2);
  Point point(2);
  Matrix hessian(2, 2);
  Matrix true_hessian(2, 2);

  point.setOnes();
  true_hessian << 802, -400,
                  -400, 200;

  rosenbrock_function.computeHessian(point, &hessian);
  EXPECT_TRUE(hessian.isApprox(true_hessian));

  RosenbrockFunction rosenbrock_function1(3);

  Point point1(3);
  Matrix true_hessian1(3, 3);
  Matrix hessian1(3, 3);

  point1.setOnes();
  true_hessian1.setZero();

  true_hessian1 << 802, -400,    0,
                  -400, 1002, -400,
                     0, -400,  200;

  rosenbrock_function1.computeHessian(point1, &hessian1);
  EXPECT_TRUE(hessian1.isApprox(true_hessian1, 0.001));
}

TEST(GriewankFunctionTest, GriewankFunctionValueTest) {
  GriewankFunction griewank_function(5);
  Point point(5);
  point.setZero();

  EXPECT_DOUBLE_EQ(0.0, griewank_function.computeValue(point));

  GriewankFunction griewank_function1(2);
  Point point1(2);
  point1 << 0.0, PI * pow(2, 0.5);

  double value = pow(PI, 2)/2000.0 + 2.0;

  EXPECT_DOUBLE_EQ(value, griewank_function1.computeValue(point1));
}
TEST(GriewankFunctionTest, GriewankFunctionGradientTest) {
  GriewankFunction griewank_function(5);
  Point point(5);
  Vector true_gradient(5);
  Vector gradient(5);

  point.setZero();
  true_gradient.setZero();
  griewank_function.computeGradient(point, &gradient);
  EXPECT_TRUE(gradient.isApprox(true_gradient, 0.01));
}
TEST(GriewankFunctionTest, GriewankFunctionHessianTest) {
  GriewankFunction griewank_function(2);
  Point point(2);
  Matrix true_hessian(2, 2);
  Matrix hessian(2, 2);

  point.setZero();
  true_hessian.setZero();
  true_hessian(0, 0) = 1.0/2000.0 + 1.0;
  true_hessian(1, 1) = true_hessian(0, 0)/2.0;

  griewank_function.computeHessian(point, &hessian);

  EXPECT_TRUE(hessian.isApprox(true_hessian, 0.01));
}

TEST(TridFunctionTest, TridFunctionValueTest) {
  TridFunction trid_function(4);
  Point point(4);

  point << 0, 1, 2, 3;

  EXPECT_EQ(-2.0, trid_function.computeValue(point));
}
TEST(TridFunctionTest, TridFunctionGradientTest) {
  TridFunction trid_function(4);
  Point point(4);
  Vector gradient(4);
  Vector true_gradient(4);

  point << 0, 1, 2, 3;
  true_gradient <<-3, -2, -2, 2;
  trid_function.computeGradient(point, &gradient);
  EXPECT_TRUE(gradient.isApprox(true_gradient, 0.01));

  TridFunction trid_function1(4);
  Point point1(4);
  Vector gradient1(4);

  point1 << 4, 6, 6, 4;
  trid_function1.computeGradient(point1, &gradient1);
  EXPECT_TRUE(gradient1.isZero(0.01));
}
TEST(TridFunctionTest, TridFunctionHessianTest) {
  TridFunction trid_function(4);
  Point point(4);
  Matrix hessian(4, 4);
  Matrix true_hessian(4, 4);

  point << 123, 23, 8237, 3;

  true_hessian << 2, -1, 0, 0,
                  -1, 2, -1, 0,
                  0, -1, 2, -1,
                  0, 0, -1, 2;
  trid_function.computeHessian(point, &hessian);
  EXPECT_TRUE(hessian.isApprox(true_hessian, 0.01));
}
TEST(TridFunctionTest, TridFunctionExceptionTest) {
  EXPECT_ANY_THROW(TridFunction trid_function(1));
  EXPECT_NO_THROW(TridFunction trid_function(2));
}
}
