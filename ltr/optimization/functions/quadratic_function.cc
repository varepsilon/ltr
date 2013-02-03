// Copyright 2012 Yandex

#include "ltr/optimization/functions/quadratic_function.h"

namespace optimization {

QuadraticFunction::QuadraticFunction(Matrix a, Vector b, double c)
  : TwiceDifferentiableFunction(a.rows()) {
  CHECK(a.rows() == a.cols());
  CHECK(a.rows() == b.rows());
  a_ = a;
  b_ = b;
  c_ = c;
}

double QuadraticFunction::value(const Point& point) const {
  CHECK(point.size() == dimension());
  double first_term = point.transpose() * (a_ * point);
  double second_term = b_.transpose() * point;
  return first_term + second_term + c_;
}

Vector QuadraticFunction::gradient(const Point& point) const {
  CHECK(point.size() == dimension());
  return 2.0 * a_ * point + b_;
}

Matrix QuadraticFunction::hessian(const Point& point) const {
  CHECK(point.size() == dimension());
  return 2.0 * a_;
}
}
