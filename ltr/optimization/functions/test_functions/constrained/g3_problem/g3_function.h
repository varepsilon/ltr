// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_TEST_CONSTRAINED_G3_PROBLEM_G3_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_TEST_CONSTRAINED_G3_PROBLEM_G3_FUNCTION_H_

#include "ltr/optimization/functions/twice_differentiable_function.h"

namespace optimization {
/**
 * \brief Class for G1 function in \f$\mathbb{R}^n\f$.
 *
 * \f$f(x) = (\sqrt{n})^n \prod_{i = 1}^n x_i\f$
 *
 * Get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page2613.htm.
 */
class G3Function : public TwiceDifferentiableFunction {
 public:
  explicit G3Function(int dimension)
    : TwiceDifferentiableFunction(dimension) {}
  ~G3Function() {}

  double value(const Point& point) const;
  Vector gradient(const Point& point) const;
  Matrix hessian(const Point& point) const;
};
}
#endif  // LTR_OPTIMIZATION_FUNCTIONS_TEST_CONSTRAINED_G3_PROBLEM_G3_FUNCTION_H_
