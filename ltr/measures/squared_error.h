// Copyright 2011 Yandex

#ifndef LTR_MEASURES_SQUARED_ERROR_H_
#define LTR_MEASURES_SQUARED_ERROR_H_

#include <cmath>
#include <string>

#include "ltr/utility/numerical.h"
#include "ltr/measures/measure.h"
#include "ltr/data/object.h"

using std::string;
using ltr::Object;
using ltr::utility::Inf;

namespace ltr {
  /**
   * Squared error pointwise measure
   */
  class SquaredError : public PointwiseMeasure {
    public:
    SquaredError(const ParametersContainer& parameters = ParametersContainer())
        : PointwiseMeasure("Squared error") {
      setDefaultParameters();
      copyParameters(parameters);
    }

    double best() const {
      return 0.0;
    }
    double worst() const {
      return Inf;
    }
    string toString() const {
      return "Squared error measure";
    }
    private:
    double get_measure(const Object& object) const {
      return std::pow(object.actual_label() - object.predicted_label(), 2);
    }
  };
};

#endif  // LTR_MEASURES_SQUARED_ERROR_H_
