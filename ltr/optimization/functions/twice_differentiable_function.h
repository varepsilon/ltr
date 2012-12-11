// Copyright 2012 Yandexs

#ifndef LTR_OPTIMIZATION_FUNCTIONS_TWICE_DIFFERENTIABLE_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_TWICE_DIFFERENTIABLE_FUNCTION_H_

#include "ltr/optimization/functions/differentiable_function.h"

namespace optimization {

class TwiceDifferentiableFunction : public DifferentiableFunction {
 public:
  typedef ltr::utility::shared_ptr<TwiceDifferentiableFunction> Ptr;

  explicit TwiceDifferentiableFunction(int dimension)
    : DifferentiableFunction(dimension) { }
  virtual ~TwiceDifferentiableFunction() { }

  virtual double value(const Point& point) const = 0;
  virtual Point gradient(const Point& point) const = 0;
  virtual Matrix hessian(const Point& point) const = 0;
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_TWICE_DIFFERENTIABLE_FUNCTION_H_
