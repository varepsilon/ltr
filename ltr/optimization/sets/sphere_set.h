// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SETS_SPHERE_SET_H_
#define LTR_OPTIMIZATION_SETS_SPHERE_SET_H_

#include "ltr/optimization/sets/set.h"

namespace optimization {
class SphereSet : public Set {
 public:
  SphereSet(double radius, double precision, int dimension);

  virtual bool isInside(const Point& point) const;
  virtual Point project(const Point& point) const;
  virtual void getBoundaries(Point* top, Point* bottom) const;

  virtual Point sampleRandomPointInside() const;

  double radius() const;
  double precision() const;
 private:
  double radius_;
  double precision_;
};
}

#endif  // LTR_OPTIMIZATION_SETS_SPHERE_SET_H_
