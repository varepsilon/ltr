// Copyright 2013 Yandex

#include "ltr/utility/random_number_generator.h"
#include "ltr/optimization/sets/cube_set.h"
using ltr::utility::randomizer;

namespace optimization {
CubeSet::CubeSet(double size, int dimension) : Set(dimension), size_(size) {}

bool CubeSet::isInside(const Point& point) const {
  CHECK(point.size() == dimension());
  bool is_inside = true;
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    is_inside &= -size() <= point(coordinate) && point(coordinate) <= size();
  }
  return is_inside;
}

void CubeSet::computeProjection(const Point& point, Point* projection) const {
  CHECK(point.size() == dimension());
  *projection = point;
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    double& coordinate_value = (*projection)(coordinate);
    if (coordinate_value < -size()) {
      coordinate_value = -size();
    } else if (coordinate_value > size()) {
      coordinate_value = size();
    }
  }
}

void CubeSet::getBoundaries(Point* top, Point* bottom) const {
  top->resize(dimension());
  bottom->resize(dimension());
  top->setConstant(size());
  bottom->setConstant(-size());
}

void CubeSet::sampleRandomPointInside(Point* point) const {
  point->resize(dimension());
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    (*point)(coordinate) = randomizer.doubleRand(-size(), size());
  }
}
}
