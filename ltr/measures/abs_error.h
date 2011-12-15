// Copyright 2011 Yandex

#ifndef LTR_MEASURES_ABS_ERROR_H_
#define LTR_MEASURES_ABS_ERROR_H_

#include "ltr/measures/measure.h"

namespace ltr {

  class AbsError : public PointwiseMeasure {
  public:
    AbsError()
      : PointwiseMeasure("Absolute error") {
    }

  private:
     double get_measure(Object& object) const;
  };

};

#endif  // LTR_MEASURES_ABS_ERROR_H_
