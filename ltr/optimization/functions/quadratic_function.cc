// Copyright 2013 Yandex

#include "ltr/optimization/functions/quadratic_function.h"

namespace optimization {

QuadraticFunction::QuadraticFunction(const Matrix& quadratic,
                                     const Vector& linear,
                                     double shift)
    : TwiceDifferentiableFunction(quadratic.rows()) {
  CHECK(quadratic.rows() == quadratic.cols());
  CHECK(quadratic.rows() == linear.rows());
  quadratic_ = quadratic;
  linear_ = linear;
  shift_ = shift;
}

double QuadraticFunction::computeValue(const Point& point) const {
  CHECK(point.size() == dimension());
  double first_term = point.dot(quadratic_ * point);
  double second_term = linear_.dot(point);
  return first_term + second_term + shift_;
}

void QuadraticFunction::computeGradient(const Point& point,
                                        Vector* gradient) const {
  CHECK(point.size() == dimension());
  *gradient = 2.0 * quadratic_ * point + linear_;
}

void QuadraticFunction::computeHessian(const Point& point,
                                       Matrix* hessian) const {
  CHECK(point.size() == dimension());
  *hessian = 2.0 * quadratic_;
}
}
