// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SETS_SET_H_
#define LTR_OPTIMIZATION_SETS_SET_H_

#include "ltr/optimization/functions/function.h"

namespace optimization {
class Set {
 public:
  explicit Set(int dimension);
  virtual ~Set();

  virtual bool isInside(const Point& point) const = 0;
  virtual Point project(const Point& point) const = 0;
  virtual void getBoundaries(Point* top, Point* bottom) const = 0;

  virtual Point getRandomPointInSet() const;

  int dimension() const;
 protected:
  Point getRandomPoint() const;
 private:
  int dimension_;
};
}

#endif  // LTR_OPTIMIZATION_SETS_SET_H_

