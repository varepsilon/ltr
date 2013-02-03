// Copyright 2012 Yandex

#include "ltr/optimization/functions/test_functions/unconstrained/rosenbrock_function/rosenbrock_function.h"
#include <math.h>
#include "ltr/utility/macros.h"

namespace optimization {
RosenbrockFunction::RosenbrockFunction(int dimension)
  : TwiceDifferentiableFunction(dimension) {CHECK(dimension > 1); }

double RosenbrockFunction::value(const Point& point) const {
  CHECK(point.size() == dimension());

  double value = 0.0;

  for (int i = 1; i < dimension(); ++i) {
    value += (100.0 * pow(pow(point(i - 1), 2) - point(i), 2) +
      pow(point(i - 1) - 1, 2));
  }

  return value;
}

Vector RosenbrockFunction::gradient(const Point& point) const {
  CHECK(point.size() == dimension());

  Point gradient(dimension());

  gradient(0) = 400.0 * (pow(point(0), 2) - point(1)) * point(0) +
    2.0 * (point(0) - 1.0);
  gradient(dimension() - 1) = -200.0 * (pow(point(dimension() - 2), 2) -
    point(dimension() - 1));

  for (int i = 1; i < dimension() - 1; ++i) {
    gradient(i) =  -200.0 * (pow(point(i - 1), 2) - point(i)) +
      400.0 * (pow(point(i), 2) - point(i + 1)) *
      point(i) + 2.0 * (point(i) - 1.0);
  }

  return gradient;
}

Matrix RosenbrockFunction::hessian(const Point& point) const {
  CHECK(point.size() == dimension());

  Matrix hessian(dimension(), dimension());
  hessian.setZero();

  hessian(0, 0) = 1200.0 * pow(point(0), 2) - 400.0 * point(1) + 2.0;
  hessian(0, 1) = -400.0 * point(0);
  hessian(1, 0) = hessian(0, 1);

  hessian(dimension() - 1, dimension() - 1) = 200.0;

  for (int i = 1; i < dimension() - 1; ++i) {
    hessian(i, i) = 200.0 + 1200.0 * pow(point(i), 2) -
      400.0 * point(i + 1) + 2.0;
    hessian(i, i + 1) = -400.0 * point(i);
    hessian(i + 1, i) = hessian(i, i + 1);
  }

  return hessian;
}
}
