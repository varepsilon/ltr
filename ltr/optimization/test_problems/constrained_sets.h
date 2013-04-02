// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_TEST_PROBLEMS_CONSTRAINED_SETS_H_
#define LTR_OPTIMIZATION_TEST_PROBLEMS_CONSTRAINED_SETS_H_

#include <string>
#include "ltr/optimization/sets/polyhedron_set.h"
#include "ltr/optimization/sets/sphere_set.h"

namespace optimization {
/**
 * \class\brief G1Set
 * Class for G1 polyhedron set in \f$\mathbb{R}^13$\f.
 * get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page506.htm
 */
class G1Set : public PolyhedronSet {
 public:
  explicit G1Set();
  ~G1Set() { }

  string getDefaultAlias() const { return "G1Set"; }
};

/**
 * \class\brief G3Set
 * Class for G3 set in \f$\mathbb{R}^n$\f.
 * \f$h(x) = \sum_{i = 1}^n x_i^2 - 1 = 0$\f
 * get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page2613.htm
 */
class G3Set : public SphereSet {
 public:
  explicit G3Set(int dimension);
  ~G3Set() { }

  string getDefaultAlias() const { return "G3Set"; }
};
}

#endif  // LTR_OPTIMIZATION_TEST_PROBLEMS_CONSTRAINED_SETS_H_

