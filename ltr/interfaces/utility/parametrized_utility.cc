// Copyright 2011 Yandex

#include <stdexcept>

#include "ltr/interfaces/utility/parametrized_utility.h"

namespace ltr {

void checkIntParameterGreaterZero(const Parameterized& parametrized,
    const string& parameterName) {
  checkIntParameterGreaterThan(parametrized, parameterName, 0);
}

void checkIntParameterGreaterThan(const Parameterized& parametrized,
    const string& parameterName,
    int value) {
  if (parametrized.getIntParameter(parameterName) <= value) {
    throw logic_error(parameterName + " is not correct.");
  }
}

void checkDoubleParameterGreaterInsegment(const Parameterized& parametrized,
    const string& parameterName,
    double segmentBegins,
    double segmentEnds) {
  if ((parametrized.getDoubleParameter(parameterName) <= segmentBegins) ||
      (parametrized.getDoubleParameter(parameterName) >= segmentEnds)) {
    throw logic_error(parameterName + " is not correct.");
  }
}

void checkDoubleParameterGreaterOrEqualZeroLessOrEqualOne(
    const Parameterized& parametrized,
    const string& parameterName) {
  checkDoubleParameterGreaterInsegment(parametrized, parameterName, 0.0, 1.0);
}
}
