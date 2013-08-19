// Copyright 2013 Yandex

#include "ltr/utility/random_number_generator.h"
#include "ltr/optimization/sets/ball_set.h"
#include "random/ExponentialDistribution.hpp"
#include "random/NormalDistribution.hpp"

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
  random_point->resize(dimension());
  RandomLib::Random& random = randomizer.generator_;
  RandomLib::ExponentialDistribution<double> exponential_distribution;
  RandomLib::NormalDistribution<double> normal_distribution;
  double square_sum = exponential_distribution(random, 1.0);
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    double& coordinate_value = (*random_point)[coordinate];
    coordinate_value = normal_distribution(random, 0.0, 1.0 / sqrt(2.0));
    square_sum += pow(coordinate_value, 2);
  }
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    (*random_point)[coordinate] /= sqrt(square_sum);
  }
  *random_point = *random_point * radius() + center();
}
}
