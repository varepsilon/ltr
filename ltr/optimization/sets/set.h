// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SETS_SET_H_
#define LTR_OPTIMIZATION_SETS_SET_H_

#include "ltr/optimization/functions/function.h"

namespace optimization {
/** \class\brief Interface class for sets in \f$\mathbb{R}^n$\f.
 */
class Set {
 public:
  explicit Set(int dimension);
  virtual ~Set();

  /** checks, whether given point is inside this set
   */
  virtual bool isInside(const Point& point) const = 0;
  /** projects given point on this set
   */
  virtual Point project(const Point& point) const = 0;
  /** give a cubic boundaries which contain this set
   */
  virtual void getBoundaries(Point* top, Point* bottom) const = 0;
  /** get arbitrary point in given set (by default, uniformly distributed)
   */
  virtual Point sampleRandomPointInside() const;

  /** number of dimensions of this set
   */
  int dimension() const;
 protected:
  Point getRandomPoint() const;
 private:
  int dimension_;
};
}

#endif  // LTR_OPTIMIZATION_SETS_SET_H_

