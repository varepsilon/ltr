// Copyright 2011 Yandex

#ifndef LTR_MEASURES_ACCURACY_H_
#define LTR_MEASURES_ACCURACY_H_

#include "ltr/measures/measure.h"

namespace ltr {
  class Accuracy : public PointwiseMeasure {
   public:
     Accuracy()
       : PointwiseMeasure("Accuracy") {
     }
   private:
    double get_measure(const Object& object) const;
  };
};

#endif  // LTR_MEASURES_ACCURACY_H_
