// Copyright 2013 Yandex

#include "ltr/optimization/sets/sphere_set.h"
#include "random/NormalDistribution.hpp"
#include "ltr/utility/random_number_generator.h"

using ltr::utility::randomizer;

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
  top->resize(dimension());
  bottom->resize(dimension());

  top->setConstant(radius());
  *top = *top + center();
  bottom->setConstant(-radius());
  *bottom = *bottom + center();
}

void SphereSet::sampleRandomPointInside(Point* random_point) const {
  random_point->resize(dimension());
  RandomLib::Random& random = randomizer.generator_;
  RandomLib::NormalDistribution<double> normal_distibution;
  double square_sum = 0;
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    (*random_point)[coordinate] = normal_distibution(random, 0.0, 1.0);
    square_sum += pow((*random_point)[coordinate], 2);
  }
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    (*random_point)[coordinate] /= sqrt(square_sum);
  }
  *random_point = *random_point * radius() + center();
}

SphereSet::SphereSet(double radius, const Point& center)
    : Set(center.rows()),
      radius_(radius),
      center_(center) {
}
}
