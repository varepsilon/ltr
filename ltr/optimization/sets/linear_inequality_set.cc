// Copyright 2012 Yandex

#include <limits>

#include "ltr/optimization/sets/linear_inequality_set.h"

namespace optimization {
LinearInequalitySet::LinearInequalitySet(Vector normal,
  Point point, int dimension):Set(dimension), positive_normal_(normal),
  initial_point_(point) { }

bool LinearInequalitySet::isInside(const Point& point) const {
  assert(point.size() == dimension());
  return !(positive_normal().dot(point - initial_point()) < 0);
}

Point LinearInequalitySet::project(const Point& point) const {
  assert(point.size() == dimension());
  if (!isInside(point)) {
    Vector projecting_vector = point - initial_point();
    Vector projected_vector;
    if (positive_normal().isZero())
      projected_vector = projecting_vector;
    else
      projected_vector = projecting_vector -
        (projecting_vector.dot(positive_normal()) / positive_normal().norm()) *
        positive_normal();
    return initial_point() + projected_vector;
  } else {
    return point;
  }
}

void LinearInequalitySet::getBoundaries(Point* top, Point* bottom) const {
  assert(top->size() == dimension());
  assert(bottom->size() == dimension());

  for (int i = 0; i < dimension(); ++i) {
    (*top)[i] = std::numeric_limits<double>::max();
    (*bottom)[i] = -std::numeric_limits<double>::max();
  }
}

Vector LinearInequalitySet::positive_normal() const {
  return positive_normal_;
}

Point LinearInequalitySet::initial_point() const {
  return initial_point_;
}
}
