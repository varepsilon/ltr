// Copyright 2011 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_QUADRATIC_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_QUADRATIC_FUNCTION_H_
#include "ltr/optimization/functions/twice_differentiable_function.h"
#include "ltr/utility/macros.h"

namespace optimization {
// f(x_1...x_n) = a_ij*x_i*x_j

class QuadraticFunction : public TwiceDifferentiableFunction {
 public:
  explicit QuadraticFunction(int dimension)
    : TwiceDifferentiableFunction(dimension) { }
  explicit QuadraticFunction(Matrix a);
  ~QuadraticFunction() { }

  double value(const Point& point) const;
  Matrix hessian(const Point& point) const;
  Vector gradient(const Point& point) const;

  GET_SET(Matrix, a);
 private:
  Matrix a_;
};
}


#endif  // LTR_OPTIMIZATION_FUNCTIONS_QUADRATIC_FUNCTION_H_
