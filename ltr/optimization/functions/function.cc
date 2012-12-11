// Copyright 2012 Yandex

#include "ltr/optimization/functions/function.h"

namespace optimization {
Function::Function(int dimension):dimension_(dimension) { }

Function::~Function() { }

int Function::dimension() const {
  return dimension_;
}
}
