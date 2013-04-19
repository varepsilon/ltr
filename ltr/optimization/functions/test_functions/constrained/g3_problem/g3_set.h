// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_TEST_CONSTRAINED_G3_PROBLEM_G3_SET_H_
#define LTR_OPTIMIZATION_FUNCTIONS_TEST_CONSTRAINED_G3_PROBLEM_G3_SET_H_

#include "ltr/optimization/sets/sphere_set.h"

namespace optimization {
/** \brief Class for G1 set in \f$\mathbb{R}^n\f$.
 *
 * \f$h(x) = \sum_{i = 1}^n x_i^2 - 1 = 0\f$
 *
 * Get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page2613.htm
 */
class G3Set : public SphereSet {
 public:
  explicit G3Set(int dimension) : SphereSet(1.0, 0.001, dimension) { }
  ~G3Set() { }
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_TEST_CONSTRAINED_G3_PROBLEM_G3_SET_H_

