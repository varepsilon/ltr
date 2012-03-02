// Copyright 2011 Yandex

#ifndef LTR_MEASURES_SQUARED_ERROR_H_
#define LTR_MEASURES_SQUARED_ERROR_H_

#include <cmath>

#include "ltr/utility/numerical.h"
#include "ltr/measures/measure.h"
#include "ltr/data/object.h"

using ltr::Object;
using ltr::utility::INFINITY;

namespace ltr {
  /**
   * Squared error pointwise measure
   */
  class SquaredError : public PointwiseMeasure {
    public:
    SquaredError(const ParametersContainer& parameters = ParametersContainer())
        : PointwiseMeasure("Squared error") {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }

    double best() const {
      return 0.0;
    }
    double worst() const {
      return INFINITY;
    }
    private:
    double get_measure(const Object& object) const {
      return std::pow(object.actualLabel() - object.predictedLabel(), 2);
    }
  };
};

#endif  // LTR_MEASURES_SQUARED_ERROR_H_
