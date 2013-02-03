// Copyright 2012 Yandex

#include "ltr/optimization/functions/test_functions/constrained/g3_problem/g3_function.h"
#include <math.h>
#include "ltr/utility/macros.h"

namespace optimization {
double G3Function::value(const Point& point) const {
  CHECK(point.size() == dimension());

  double value_ = 1.0;
  for (int i = 0; i < dimension(); ++i) {
    value_ *= point(i);
  }
  value_ *= pow(dimension(), 1.0/dimension());
  return value_;
}
Vector G3Function::gradient(const Point& point) const {
  CHECK(point.size() == dimension());

  Vector gradient_(dimension());
  gradient_.setOnes();

  for (int i = 0; i < dimension(); ++i) {
    for (int j = 0; j < dimension(); ++j) {
      if (i != j)
        gradient_(i) *= point(j);
    }
  }
  gradient_ *= pow(dimension(), 1.0/dimension());
  return gradient_;
}
Matrix G3Function::hessian(const Point& point) const {
  CHECK(point.size() == dimension());

  Matrix hessian_(dimension(), dimension());
  hessian_.setOnes();

  for (int i = 0; i < dimension(); ++i) {
    hessian_(i, i) = 0.0;
    for (int j = 0; j < dimension(); ++j) {
      if (i != j)
      for (int k = 0; k < dimension(); ++k) {
        if ((k != i) && (k != j))
          hessian_(i) *= point(k);
      }
    }
  }
  hessian_ *=  pow(dimension(), 1.0/dimension());
  return hessian_;
}
}
