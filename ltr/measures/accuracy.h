// Copyright 2011 Yandex

#ifndef LTR_MEASURES_ACCURACY_H_
#define LTR_MEASURES_ACCURACY_H_

#include "ltr/measures/measure.h"
#include "ltr/utility/numerical.h"

namespace ltr {

template <typename TElement>
class Accuracy : public LessIsBetterMeasure<TElement> {
  public:
  Accuracy() : Measure<TElement>("Accuracy") {}

  private:
  double get_measure(const TElement& object) const;
};

template <typename TElement>
double Accuracy<TElement>::get_measure(const TElement& element) const {
  double errorNum = 0.0;
  for (size_t objIdx = 0; objIdx < element.size(); ++objIdx) {
    errorNum += utility::DoubleEqual(element[objIdx].predictedLabel(),
        element[objIdx].actualLabel()) ? 1 : 0;
  }

  return errorNum;
}
}
#endif  // LTR_MEASURES_ACCURACY_H_
