// Copyright 2013 Yandex

#include "ltr/utility/random_number_generator.h"
#include "ltr/optimization/sets/ball_set.h"
using ltr::utility::randomizer;

namespace optimization {
BallSet::BallSet(double radius, const Point& center)
    : Set(center.rows()),
      radius_(radius),
      center_(center) {
}

bool BallSet::isInside(const Point& point) const {
  CHECK(point.size() == dimension());
  return !((point - center()).norm() > radius());
}

void BallSet::computeProjection(const Point& point, Point* projection) const {
  CHECK(point.size() == dimension());
  if (isInside(point)) {
    *projection = point;
  } else {
    *projection = (point - center()).normalized() * radius() + center();
  }
}

void BallSet::getBoundaries(Point* top, Point* bottom) const {
  CHECK(top->size() == dimension());
  CHECK(bottom->size() == dimension());

  top->setConstant(radius());
  *top = *top + center();
  bottom->setConstant(-radius());
  *bottom = *bottom + center();
}

void BallSet::sampleRandomPointInside(Point* random_point) const {
  getRandomPoint(random_point);
  *random_point = (*random_point - center()).normalized() *
    randomizer.doubleRand(0, radius()) +
      center();
}
}
