// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_GRIEWANK_FUNCTION_GRIEWANK_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_GRIEWANK_FUNCTION_GRIEWANK_FUNCTION_H_

#include <string>
#include "ltr/optimization/functions/twice_differentiable_function.h"

namespace optimization {
/** \class\brief GriewankFunction
 * Class for Griewank function in \f$\mathbb{R}^n$\f.
 * \f$f(x) = \sum_{i = 1}^n x_i^2/4000 - \prod_{i = 1} ^ n \cos(x_i / \sqrt(i))$\f
 * minimum at \f$ x = (0, \dots, 0), \quad f(x) = 0 $\f
 * get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page1905.htm
 */
class GriewankFunction : public TwiceDifferentiableFunction {
 public:
  explicit GriewankFunction(int dimension)
    : TwiceDifferentiableFunction(dimension) { }
  ~GriewankFunction() { }

  double value(const Point& point) const;
  Vector gradient(const Point& point) const;
  Matrix hessian(const Point& point) const;
  string getDefaultAlias() const { return "GriewankFunction"; }
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_GRIEWANK_FUNCTION_GRIEWANK_FUNCTION_H_
