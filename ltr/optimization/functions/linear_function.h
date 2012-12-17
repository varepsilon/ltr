// Copyright 2011 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_LINEAR_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_LINEAR_FUNCTION_H_
#include "ltr/optimization/functions/twice_differentiable_function.h"
#include "ltr/utility/macros.h"

namespace optimization {

// f(x_1...x_n) = a_1*x_1 + ..+ a_n*x_n + b

class LinearFunction : public TwiceDifferentiableFunction {
 public:

  explicit LinearFunction(int dimension)
    : TwiceDifferentiableFunction(dimension) { }
  explicit LinearFunction(Vector factor , double shift = 0.0)
    : factor_(factor),
      shift_(shift),
      TwiceDifferentiableFunction(factor.size()) { }
  ~LinearFunction() { }

  double value(const Point& point) const;
  Matrix hessian(const Point& point) const;
  Vector gradient(const Point& point) const;

  GET_SET(Vector, factor);
  GET_SET(double, shift);
 private:
  Vector factor_;
  double shift_;
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_LINEAR_FUNCTION_H_
