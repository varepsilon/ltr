// Copyright 2012 Yandex

#include "ltr/optimization/sets/ball_set.h"

namespace optimization {
bool BallSet::isInside(const Point& point) const {
  assert(point.size() == dimension());
  return !(point.norm() > radius());
}

Point BallSet::project(const Point& point) const {
  assert(point.size() == dimension());
  if (isInside(point))
    return point;
  return point.normalized() * radius();
}

void BallSet::getBoundaries(Point* top, Point* bottom) const {
  assert(top->size() == dimension());
  assert(bottom->size() == dimension());

  for (int i = 0; i < dimension(); ++i) {
    (*top)[i] = radius();
    (*bottom)[i] = -radius();
  }
}

BallSet::BallSet(double radius, int dimension): Set(dimension),
  radius_(radius) { }

double BallSet::radius() const {
  return radius_;
}
}
