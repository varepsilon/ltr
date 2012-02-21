// Copyright 2011 Yandex

#ifndef LTR_MEASURES_SQUARED_ERROR_H_
#define LTR_MEASURES_SQUARED_ERROR_H_

#include <cmath>

#include "ltr/measures/measure.h"
#include "ltr/data/object.h"

using ltr::Object;
using ltr::LessIsBetterMeasure;

namespace ltr {
  /**
   * Squared error pointwise measure
   */
  class SquaredError : public LessIsBetterMeasure<Object> {
    public:
    SquaredError(const ParametersContainer& parameters = ParametersContainer())
        :LessIsBetterMeasure<Object>("Squared error") {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }

    private:
    double get_measure(const Object& object) const {
      return std::pow(object.actualLabel() - object.predictedLabel(), 2);
    }
  };
};

#endif  // LTR_MEASURES_SQUARED_ERROR_H_
