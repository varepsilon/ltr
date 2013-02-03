// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_TRID_FUNCTION_TRID_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_TRID_FUNCTION_TRID_FUNCTION_H_

#include <string>
#include "ltr/optimization/functions/twice_differentiable_function.h"

namespace optimization {
/** \class\brief TridFunction
 * Class for Trid function in \f$\mathbb{R}^n$\f.
 * \f$f(x) = \sum_{i = 1}^{n} (x_i - 1)^2 - \sum_{i = 2}^n x_i*x_{i-1}$\f
 * get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page2904.htm
 */
class TridFunction : public TwiceDifferentiableFunction {
 public:
  explicit TridFunction(int dimension);
  ~TridFunction() { }

  double value(const Point& point) const;
  Vector gradient(const Point& point) const;
  Matrix hessian(const Point& point) const;
  string getDefaultAlias() const { return "TridFunction"; }
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_TRID_FUNCTION_TRID_FUNCTION_H_
