// Copyright 2011 Yandex

#include "ltr/measures/accuracy.h"
#include "ltr/utility/numerical.h"

namespace ltr {
  double Accuracy::get_measure(const Object& object) const {
    return utility::DoubleEqual(
      object.predictedLabel(), object.actualLabel()) ? 1 : 0;
  }
};