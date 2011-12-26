// Copyright 2011 Yandex

#ifndef LTR_MEASURES_ABS_ERROR_H_
#define LTR_MEASURES_ABS_ERROR_H_

#include <cmath>

#include "ltr/measures/measure.h"

namespace ltr {

template <typename TElement>
class AbsError : public Measure<TElement> {
  public:
  typedef boost::shared_ptr<AbsError> Ptr;

  AbsError() : Measure<TElement>("Absolute error.") {}

  bool better(double firstVal, double secondVal) const {
    return firstVal < secondVal;
  }

  private:
  double get_measure(const TElement& element) const {
    double absError = 0.0;
    for (size_t objIdx = 0; objIdx < element.size(); ++objIdx) {
      absError += std::fabs(element[objIdx].actualLabel() -
          element[objIdx].predictedLabel());
    }
    return absError;
  }
};
}
#endif  // LTR_MEASURES_ABS_ERROR_H_
