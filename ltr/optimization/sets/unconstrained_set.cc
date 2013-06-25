// Copyright 2013 Yandex

#include <float.h>
#include "ltr/utility/random_number_generator.h"
#include "ltr/optimization/sets/unconstrained_set.h"
using ltr::utility::randomizer;

namespace optimization {
UnconstrainedSet::UnconstrainedSet(int dimension) : Set(dimension) { }

UnconstrainedSet::~UnconstrainedSet() { }

bool UnconstrainedSet::isInside(const Point& point) const {
  CHECK(point.size() == dimension());
  return true;
}

void UnconstrainedSet::computeProjection(
    const Point& point,
    Point* projection) const {
  CHECK(point.size() == dimension());
  *projection = point;
}

void UnconstrainedSet::getBoundaries(Point* top, Point* bottom) const {
  CHECK(top->size() == dimension());
  CHECK(bottom->size() == dimension());

  top->setConstant(DBL_MAX);
  bottom->setConstant(DBL_MIN);
}

void UnconstrainedSet::sampleRandomPointInside(Point* random_point) const {
  getRandomPoint(random_point);
}
}
