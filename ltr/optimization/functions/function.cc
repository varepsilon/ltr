// Copyright 2012 Yandex

#include "ltr/optimization/functions/function.h"
#include "ltr/utility/macros.h"

namespace optimization {
Function::Function(int dimension):dimension_(dimension) 
{
  CHECK(dimension > 0)
}

int Function::dimension() const {
  return dimension_;
}
}
