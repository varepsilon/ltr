// Copyright 2012 Yandex

#include "ltr/optimization/sets/ball_set.h"
#include "ltr/utility/macros.h"
#include "ltr/utility/random_number_generator.h"

namespace optimization {
bool BallSet::isInside(const Point& point) const {
  CHECK(point.size() == dimension());
  return !(point.norm() > radius());
}

Point BallSet::project(const Point& point) const {
  CHECK(point.size() == dimension());
  if (isInside(point))
    return point;
  return point.normalized() * radius();
}

void BallSet::getBoundaries(Point* top, Point* bottom) const {
  CHECK(top->size() == dimension());
  CHECK(bottom->size() == dimension());

  top->setConstant(radius());
  bottom->setConstant(-radius());
}

// this distribution is not uniform, but will do for now
Point BallSet::sampleRandomPointInside() const {
  Point random_point = getRandomPoint();
  return random_point.normalized() * ltr::utility::randomizer.doubleRand(0, radius());
}

BallSet::BallSet(double radius, int dimension): Set(dimension),
  radius_(radius) { }

double BallSet::radius() const {
  return radius_;
}
}
