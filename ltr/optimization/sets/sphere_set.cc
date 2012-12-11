// Copyright 2012 Yandex

#include "ltr/optimization/sets/sphere_set.h"
#include <cmath>

namespace optimization {
bool SphereSet::isInside(const Point& point) const {
  assert(point.size() == dimension());
  return abs(point.norm() - radius()) < precision();
}

Point SphereSet::project(const Point& point) const {
  assert(point.size() == dimension());
  if (isInside(point))
    return point;
  return point.normalized() * radius();
}

void SphereSet::getBoundaries(Point* top, Point* bottom) const {
  assert(top->size() == dimension());
  assert(bottom->size() == dimension());

  for (int i = 0; i < dimension(); ++i) {
    (*top)[i] = radius();
    (*bottom)[i] = -radius();
  }
}

Point SphereSet::getRandomPointInSet() const {
  return (getRandomPoint().normalized()) * radius();
}

double SphereSet::radius() const {
  return radius_;
}

double SphereSet::precision() const {
  return precision_;
}

SphereSet::SphereSet(double radius, double precision, int dimension)
  : Set(dimension), radius_(radius), precision_(precision) { }
}
