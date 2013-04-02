// Copyright 2013 Yandex

#include "ltr/utility/macros.h"
#include "ltr/optimization/functions/function.h"

namespace optimization {
Function::Function(int dimension) : dimension_(dimension) {
  CHECK(dimension > 0)
}

int Function::dimension() const {
  return dimension_;
}

double Function::computeValue(const Point& point) const {
  CHECK(point.size() == dimension());
  return computeValueImpl(point);
}
}
