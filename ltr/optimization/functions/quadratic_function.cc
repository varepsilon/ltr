// Copyright 2011 Yandex

#include "ltr/optimization/functions/quadratic_function.h"

namespace optimization {
  // f(X) = X^T * a * X

QuadraticFunction::QuadraticFunction(Matrix a)
  : TwiceDifferentiableFunction(a.rows()) {
  assert(a.rows() == a.cols());
  a_ = a;
}

double QuadraticFunction::value(const Point& point) const {
  assert(point.size() == dimension());
  return point.transpose() * (a_ * point);
}
Vector QuadraticFunction::gradient(const Point& point) const {
  assert(point.size() == dimension());
  return 2.0 * a_ * point;
}
Matrix QuadraticFunction::hessian(const Point& point) const {
  assert(point.size() == dimension());
  return 2.0 * a_;
}
}
