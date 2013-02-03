// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_SUM_SQUARES_FUNCTION_SUM_SQUARES_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_SUM_SQUARES_FUNCTION_SUM_SQUARES_FUNCTION_H_

#include <string>
#include "ltr/optimization/functions/twice_differentiable_function.h"

namespace optimization {
/** \class\brief SumSquaresFunction
 * Class for Sum Squares function in \f$\mathbb{R}^n$\f.
 * \f$f(x) = \sum_{i = 1}^{n} i * x_i^2 $\f
 * minimum at \f$ x = (0, \dots, 0), \quad f(x) = 0 $\f
 * get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page674.htm
 */
class SumSquaresFunction : public TwiceDifferentiableFunction {
 public:
  explicit SumSquaresFunction(int dimension)
    : TwiceDifferentiableFunction(dimension) { }
  ~SumSquaresFunction() { }

  double value(const Point& point) const;
  Matrix hessian(const Point& point) const;
  Vector gradient(const Point& point) const;
  string getDefaultAlias() const { return "SumSquaresFunction"; }
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_SUM_SQUARES_FUNCTION_SUM_SQUARES_FUNCTION_H_
