// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_DIFFERENTIABLE_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_DIFFERENTIABLE_FUNCTION_H_

#include "ltr/optimization/functions/function.h"

namespace optimization {

class DifferentiableFunction : public Function {
 public:
  typedef ltr::utility::shared_ptr<DifferentiableFunction> Ptr;

  explicit DifferentiableFunction(int dimension)
    : Function(dimension) { }
  virtual ~DifferentiableFunction() { }

  virtual double value(const Point& point) const = 0;
  virtual Vector gradient(const Point& point) const = 0;
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_DIFFERENTIABLE_FUNCTION_H_
