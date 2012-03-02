// Copyright 2011 Yandex

#ifndef LTR_MEASURES_ABS_ERROR_H_
#define LTR_MEASURES_ABS_ERROR_H_

#include <cmath>

#include "ltr/utility/numerical.h"
#include "ltr/measures/measure.h"
#include "ltr/data/object.h"

using std::fabs;
using ltr::Object;
using ltr::utility::INFINITY;

namespace ltr {
  /**
   * Absolute error pointwise measure
   */
  class AbsError : public PointwiseMeasure {
    public:
    AbsError(const ParametersContainer& parameters = ParametersContainer())
        :PointwiseMeasure("Absolute error") {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }

    double worst() const {
      return INFINITY;
    }
    double best() const {
      return 0;
    }

    private:
    double get_measure(const Object& element) const {
      return fabs(element.actualLabel() -
            element.predictedLabel());
    }
  };
};
#endif  // LTR_MEASURES_ABS_ERROR_H_
