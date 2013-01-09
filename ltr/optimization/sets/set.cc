// Copyright 2012 Yandex

#include "ltr/optimization/sets/set.h"
#include "ltr/utility/random_number_generator.h"

namespace optimization {
Set::Set(int dimension):dimension_(dimension) { }

Set::~Set() { }

int Set::dimension() const {
  return dimension_;
}

Point Set::getRandomPoint() const {
  Point bottom(dimension());
  Point top(dimension());
  getBoundaries(&top, &bottom);
  Point random_point(dimension());
  for (int i = 0; i < dimension(); ++i) {
    random_point[i] = ltr::utility::randomizer.doubleRand(bottom[i], top[i]);
  }
  return random_point;
}

Point Set::getRandomPointInSet() const {
  Point random_point;
  do {
    random_point = getRandomPoint();
  } while (!isInside(random_point));
  return random_point;
}
}
