// Copyright 2011 Yandex

#ifndef LTR_MEASURES_ABS_ERROR_H_
#define LTR_MEASURES_ABS_ERROR_H_

#include <cmath>
#include <string>

#include "ltr/utility/numerical.h"
#include "ltr/measures/measure.h"
#include "ltr/data/object.h"

using std::fabs;
using std::string;
using ltr::Object;
using ltr::utility::Inf;

namespace ltr {
  /**
   * Absolute error pointwise measure
   */
  class AbsError : public PointwiseMeasure {
    public:
    AbsError(const ParametersContainer& parameters = ParametersContainer())
        : PointwiseMeasure("Absolute error") {
      this->setDefaultParameters();
      copyParameters(parameters);
    }

    double worst() const {
      return Inf;
    }
    double best() const {
      return 0.0;
    }

    string toString() const {
      return "Absolute error measure";
    }

    private:
    double get_measure(const Object& element) const {
      return fabs(element.actualLabel() -
            element.predictedLabel());
    }
  };
};
#endif  // LTR_MEASURES_ABS_ERROR_H_
