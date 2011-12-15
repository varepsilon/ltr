// Copyright 2011 Yandex

#include <cmath>

#include "ltr/measures/abs_error.h"

namespace ltr {

  double AbsError::get_measure(Object& object) const {
    return fabs(object.actualLabel() - object.predictedLabel());
  }

};
