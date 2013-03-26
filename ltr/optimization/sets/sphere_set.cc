// Copyright 2013 Yandex

#include "ltr/optimization/sets/sphere_set.h"

namespace optimization {
bool SphereSet::isInside(const Point& point) const {
  CHECK(point.size() == dimension());
  return ltr::utility::DoubleEqual((point - center()).norm(), radius());
}
void SphereSet::computeProjection(const Point& point,
                                  Point* projection) const {
  CHECK(point.size() == dimension());
  if (isInside(point)) {
    *projection = point;
  } else if (point == center()) {
    sampleRandomPointInside(projection);
  } else {
    *projection = (point - center()).normalized() * radius() + center();
  }
}

void SphereSet::getBoundaries(Point* top, Point* bottom) const {
  CHECK(top->size() == dimension());
  CHECK(bottom->size() == dimension());

  top->setConstant(radius());
  *top = *top + center();
  bottom->setConstant(-radius());
  *bottom = *bottom + center();
}

void SphereSet::sampleRandomPointInside(Point* random_point) const {
  getRandomPoint(random_point);
  *random_point = (*random_point - center()).normalized() * radius() +
    center();
}

SphereSet::SphereSet(double radius, const Point& center)
    : Set(center.rows()),
      radius_(radius),
      center_(center) {
}
}
