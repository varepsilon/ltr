// Copyright 2012 Yandex

#include "ltr/optimization/functions/test_functions/unconstrained/sum_squares_function/sum_squares_function.h"
#include <math.h>
#include "ltr/utility/macros.h"

namespace optimization {
double SumSquaresFunction::value(const Point& point) const {
  CHECK(point.size() == dimension());

  double value = 0.0;

  for (int i = 0; i < dimension(); ++i) {
    value += (i + 1.0) * pow(point(i), 2);
  }

  return value;
}

Vector SumSquaresFunction::gradient(const Point& point) const {
  CHECK(point.size() == dimension());

  Point gradient(dimension());
  for (int i = 0; i < dimension(); ++i) {
    gradient(i) = 2.0 * (i + 1.0) * point(i);
  }

  return gradient;
}

Matrix SumSquaresFunction::hessian(const Point& point) const {
  CHECK(point.size() == dimension());

  Matrix hessian(dimension(), dimension());
  hessian.setZero();

  for (int i = 0; i < dimension(); ++i) {
    hessian(i, i) = 2.0 * (i + 1.0);
  }

  return hessian;
}
}
