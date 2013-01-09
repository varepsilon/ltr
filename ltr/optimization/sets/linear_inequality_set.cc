// Copyright 2012 Yandex

#include <limits>

#include "ltr/optimization/sets/linear_inequality_set.h"
#include "ltr/utility/random_number_generator.h"
#include "ltr/utility/macros.h"

namespace optimization {
LinearInequalitySet::LinearInequalitySet(Vector normal,
  Point point, int dimension):Set(dimension), positive_normal_(normal),
  initial_point_(point) { }

bool LinearInequalitySet::isInside(const Point& point) const {
  CHECK(point.size() == dimension());
  return !(positive_normal().dot(point - initial_point()) < 0);
}

Point LinearInequalitySet::project(const Point& point) const {
  CHECK(point.size() == dimension());
  if (!isInside(point)) {
    Vector projecting_vector = point - initial_point();
    Vector projected_vector;
    if (positive_normal().isZero()) {
      projected_vector = projecting_vector;
    } else {
      double scalar = projecting_vector.dot(positive_normal());
      double norm = positive_normal().dot(positive_normal());
      projected_vector = projecting_vector - (scalar/norm) * positive_normal();
    }
    return initial_point() + projected_vector;
  } else {
    return point;
  }
}

void LinearInequalitySet::getBoundaries(Point* top, Point* bottom) const {
  CHECK(top->size() == dimension());
  CHECK(bottom->size() == dimension());

  for (int i = 0; i < dimension(); ++i) {
    (*top)[i] = std::numeric_limits<double>::max();
    (*bottom)[i] = std::numeric_limits<double>::min();
  }
}

Vector LinearInequalitySet::positive_normal() const {
  return positive_normal_;
}

Point LinearInequalitySet::initial_point() const {
  return initial_point_;
}

Point LinearInequalitySet::getRandomPointInSet() const {
  Vector random_vector(dimension());
  double max_rand = std::numeric_limits<double>::max();
  double min_rand = std::numeric_limits<double>::min();

  for (int i = 0; i < dimension(); i++) {
    random_vector[i] = ltr::utility::randomizer.doubleRand(min_rand, max_rand);
  }
  if (random_vector.dot(positive_normal()) < 0)
    random_vector = -random_vector;
  return initial_point() + random_vector;
}
}
