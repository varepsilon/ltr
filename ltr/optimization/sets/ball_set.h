// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SETS_BALL_SET_H_
#define LTR_OPTIMIZATION_SETS_BALL_SET_H_

#include "ltr/optimization/sets/set.h"

namespace optimization {
class BallSet : public Set {
 public:
  BallSet(double radius, int dimension);

  virtual bool isInside(const Point& point) const;
  virtual Point project(const Point& point) const;
  virtual void getBoundaries(Point* top, Point* bottom) const;

  double radius() const;
 private:
  double radius_;
};
}

#endif  // LTR_OPTIMIZATION_SETS_BALL_SET_H_
