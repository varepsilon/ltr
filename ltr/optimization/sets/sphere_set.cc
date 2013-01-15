// Copyright 2012 Yandex

#include <cmath>

#include "ltr/optimization/sets/sphere_set.h"
#include "ltr/utility/macros.h"

namespace optimization {
bool SphereSet::isInside(const Point& point) const {
  CHECK(point.size() == dimension());
  return fabs(point.norm() - radius()) < precision();
}

Point SphereSet::project(const Point& point) const {
  CHECK(point.size() == dimension());
  if (isInside(point))
    return point;
  return point.normalized() * radius();
}

void SphereSet::getBoundaries(Point* top, Point* bottom) const {
  CHECK(top->size() == dimension());
  CHECK(bottom->size() == dimension());

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
