// Copyright 2011 Yandex

#include "ltr/optimization/functions/linear_function.h"

namespace optimization {

double LinearFunction::value(const Point& point) const {
  CHECK(point.size() == dimension());
  return factor_.transpose() * point + shift_;
}

Vector LinearFunction::gradient(const Point& point) const {
  CHECK(point.size() == dimension());
  return factor_;
}

Matrix LinearFunction::hessian(const Point& point) const {
  int dim = dimension();
  CHECK(point.size() == dim);
  Matrix result(dim, dim);
  result.setZero();
  return result;
}
}
