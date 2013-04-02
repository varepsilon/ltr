// Copyright 2013 Yandex

#include "ltr/optimization/functions/linear_function.h"

namespace optimization {

double LinearFunction::computeValue(const Point& point) const {
  CHECK(point.size() == dimension());
  return factor_.transpose() * point + shift_;
}

void LinearFunction::computeGradient(const Point& point,
                                     Vector* gradient) const {
  CHECK(point.size() == dimension());
  *gradient = factor_;
}

void LinearFunction::computeHessian(const Point& point, Matrix* hessian) const {
  CHECK(point.size() == dimension());
  hessian->resize(dimension(), dimension());
  hessian->setZero();
}
}
