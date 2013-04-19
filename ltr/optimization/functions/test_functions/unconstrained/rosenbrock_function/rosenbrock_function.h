// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_ROSENBROCK_FUNCTION_ROSENBROCK_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_ROSENBROCK_FUNCTION_ROSENBROCK_FUNCTION_H_

#include <string>
#include "ltr/optimization/functions/twice_differentiable_function.h"

namespace optimization {
/**
 * \brief Class for Rosenbrock function in \f$\mathbb{R}^n\f$.
 * 
 * \f$f(x) = \sum_{i = 1}^{n - 1} (100 * (x_i^2 - x_{i + 1}) + (x_i - 1)^2)\f$,
 * minimum at \f$ x = (1, \dots, 1), \quad f(x) = 0 \f$.
 *
 * Get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page2537.htm
 */
class RosenbrockFunction : public TwiceDifferentiableFunction {
 public:
  explicit RosenbrockFunction(int dimension);
  ~RosenbrockFunction() { }

  double value(const Point& point) const;
  Vector gradient(const Point& point) const;
  Matrix hessian(const Point& point) const;
  string getDefaultAlias() const { return "RosenbrockFunction"; }
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_ROSENBROCK_FUNCTION_ROSENBROCK_FUNCTION_H_
