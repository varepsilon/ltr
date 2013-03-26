// Copyright 2013 Yandex

#include <math.h>
#include <Eigen/Dense>
#include <limits>
#include "ltr/utility/numerical.h"
#include "ltr/utility/random_number_generator.h"
#include "ltr/utility/macros.h"
#include "ltr/optimization/sets/linear_inequality_set.h"

namespace optimization {
LinearInequalitySet::LinearInequalitySet(const Vector& positive_normal,
                                         double shift)
    : Set(positive_normal.rows()),
      positive_normal_(positive_normal),
      shift_(shift) {
  CHECK(positive_normal.rows() > 0);
  CHECK(positive_normal.lpNorm<2>() != 0.0);
}

bool LinearInequalitySet::isInside(const Point& point) const {
  CHECK(point.size() == dimension());
  return (positive_normal().dot(point) + shift() >= 0);
}

void LinearInequalitySet::computeProjection(const Point& point,
                                            Point* projection) const {
  CHECK(point.size() == dimension());
  if (isInside(point)) {
    *projection = point;
  } else {
    *projection = point -
      (positive_normal() * (shift() + positive_normal().dot(point))) /
        pow(positive_normal().norm(), 2);
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

void LinearInequalitySet::sampleRandomPointInside(Point* random_point) const {
  Vector random_vector(dimension());
  random_vector.setRandom();
  if (random_vector.dot(positive_normal()) < 0) {
    random_vector = -random_vector;
  }
  *random_point = shift() * positive_normal() / positive_normal().lpNorm<2>()
    + random_vector;
}
}
