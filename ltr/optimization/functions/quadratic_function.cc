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

double QuadraticFunction::computeValueImpl(const Point& point) const {
  double first_term = point.dot(quadratic_ * point);
  double second_term = linear_.dot(point);
  return first_term + second_term + shift_;
}

void QuadraticFunction::computeGradientImpl(const Point& point,
                                        Vector* gradient) const {
  *gradient = 2.0 * quadratic_ * point + linear_;
}

void QuadraticFunction::computeHessianImpl(const Point& point,
                                       Matrix* hessian) const {
  *hessian = 2.0 * quadratic_;
}
}
