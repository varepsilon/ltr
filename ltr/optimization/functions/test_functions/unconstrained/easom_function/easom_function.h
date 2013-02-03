// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_EASOM_FUNCTION_EASOM_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_EASOM_FUNCTION_EASOM_FUNCTION_H_

#include <string>
#include "ltr/optimization/functions/twice_differentiable_function.h"

namespace optimization {
/** \class\brief EasomFunction
 * Class for Easom function in \f$\mathbb{R}^2$\f.
 * \f$f(x, y) = - e^{-(x - \pi)^2} * e^{-(y - \pi)^2} * \cos(x) * \cos(y)$\f
 * \f$f(x, y) = - g(x) * g(y), \text{where } g(x) = e^{-(x - \pi)^2} * cos(x)$\f
 * minimum at \f$ (x, y) = (\pi, \pi), \quad f(x, y) = -1$\f
 * get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page1361.htm
 */
class EasomFunction : public TwiceDifferentiableFunction {
 public:
  explicit EasomFunction()
    : TwiceDifferentiableFunction(2) { }
  ~EasomFunction() { }

  double value(const Point& point) const;
  Vector gradient(const Point& point) const;
  Matrix hessian(const Point& point) const;
  string getDefaultAlias() const { return "EasomFunction"; }
 private:
  /** find \f$e^{-(x - \pi)^2}$\f
  */
  double squareExp(double variable) const;
  /** find \f$g(x)$\f
  */
  double partValue(double variable) const;
  /** find \f$\frac{dg(x)}{dx}$\f
  */
  double partGradient(double variable) const;
  /** find \f$\frac{d^2g(x)}{dx^2}$\f
  */
  double partHessian(double variable) const;
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_TEST_UNCONSTRAINED_EASOM_FUNCTION_EASOM_FUNCTION_H_
