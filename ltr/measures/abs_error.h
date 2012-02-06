// Copyright 2011 Yandex

#ifndef LTR_MEASURES_ABS_ERROR_H_
#define LTR_MEASURES_ABS_ERROR_H_

#include <cmath>

#include "ltr/measures/measure.h"
#include "ltr/data/object.h"

using ltr::Object;
using ltr::LessIsBetterMeasure;

namespace ltr {

class AbsError : public LessIsBetterMeasure<Object> {
  public:
  AbsError(const ParametersContainer& parameters = ParametersContainer())
      :LessIsBetterMeasure<Object>("Absolute error") {
    this->setDefaultParameters();
    this->parameters().copyParameters(parameters);
  }

  private:
  double get_measure(const Object& element) const {
    return std::fabs(element.actualLabel() -
          element.predictedLabel());
  }
};
}
#endif  // LTR_MEASURES_ABS_ERROR_H_
