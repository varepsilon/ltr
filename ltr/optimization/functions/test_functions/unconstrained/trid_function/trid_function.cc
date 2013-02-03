// Copyright 2012 Yandex

#include "ltr/optimization/functions/test_functions/unconstrained/trid_function/trid_function.h"
#include <math.h>
#include "ltr/utility/macros.h"

namespace optimization {
TridFunction::TridFunction(int dimension)
  : TwiceDifferentiableFunction(dimension) { CHECK(dimension > 1); }

double TridFunction::value(const Point& point) const {
  CHECK(point.size() == dimension());

  double sum_squares = 0.0;
  double sum_prod = 0.0;

  for (int i = 0; i < dimension(); ++i) {
    sum_squares += pow(point(i) - 1.0, 2);
  }

  for (int i = 1; i < dimension(); ++i) {
    sum_prod += point(i) * point(i - 1);
  }

  double value = sum_squares - sum_prod;

  return value;
}

Vector TridFunction::gradient(const Point& point) const {
  CHECK(point.size() == dimension());

  Vector gradient(dimension());

  gradient(0) = 2.0 * point(0) - 2.0 - point(1);
  gradient(dimension() - 1) = 2.0 * point(dimension() - 1) -
    2.0 - point(dimension() - 2);

  for (int i = 1; i < dimension() - 1; ++i) {
    gradient(i) = 2.0 * point(i) - 2.0 - point(i - 1) - point(i + 1);
  }

  return gradient;
}

Matrix TridFunction::hessian(const Point& point) const {
  CHECK(point.size() == dimension());

  Matrix hessian(dimension(), dimension());
  hessian.setZero();

  for (int i = 0; i < dimension(); ++i) {
    hessian(i, i) = 2.0;
  }

  for (int i = 0; i < dimension() - 1; ++i) {
    hessian(i, i + 1) = -1.0;
    hessian(i + 1, i) = -1.0;
  }

  return hessian;
}
}
