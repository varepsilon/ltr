// Copyright 2013 Yandex

#include "ltr/optimization/functions/linear_function.h"

namespace optimization {

double LinearFunction::computeValueImpl(const Point& point) const {
  return factor_.transpose() * point + shift_;
}

void LinearFunction::computeGradientImpl(const Point& point,
                                     Vector* gradient) const {
  *gradient = factor_;
}

void LinearFunction::computeHessianImpl(const Point& point,
                                        Matrix* hessian) const { }
}
