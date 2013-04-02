// Copyright 2013 Yandex

#include "ltr/utility/random_number_generator.h"
#include "ltr/optimization/sets/set.h"
using ltr::utility::randomizer;

namespace optimization {
Set::Set(int dimension) : dimension_(dimension) { }

Set::~Set() { }

void Set::getRandomPoint(Point* random_point) const {
  Point bottom(dimension());
  Point top(dimension());
  getBoundaries(&top, &bottom);

  random_point->resize(dimension());
  Point result_point(dimension());
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    (*random_point)(coordinate) =
      randomizer.doubleRand(bottom(coordinate), top(coordinate));
  }
}

void Set::sampleRandomPointInside(Point* random_point) const {
  getRandomPoint(random_point);
  Point projected_random_point;
  computeProjection((*random_point), &projected_random_point);
  (*random_point) = projected_random_point;
}
}
