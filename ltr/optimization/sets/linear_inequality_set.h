// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SETS_LINEAR_INEQUALITY_SET_H_
#define LTR_OPTIMIZATION_SETS_LINEAR_INEQUALITY_SET_H_

#include "ltr/optimization/sets/set.h"
#include "ltr/optimization/functions/function.h"
#include "ltr/interfaces/parameterized.h"

namespace optimization {
/**\class\brief implements semi-space
 */
class LinearInequalitySet : public Set {
 public:
  LinearInequalitySet(Vector normal, Point point, int dimension);

  virtual bool isInside(const Point& point) const;
  virtual Point project(const Point& point) const;
  virtual void getBoundaries(Point* top, Point* bottom) const;

  virtual Point sampleRandomPointInside() const;
  /** vector, defining border hyperplane and positive direction
   */
  Vector positive_normal() const;
  /** some point on border
   */
  Point initial_point() const;
 private:
  Vector positive_normal_;
  Point initial_point_;
};
}

#endif  // LTR_OPTIMIZATION_SETS_LINEAR_INEQUALITY_SET_H_
