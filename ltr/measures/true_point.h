// Copyright 2011 Yandex

#ifndef LTR_MEASURES_TRUE_POINT_H_
#define LTR_MEASURES_TRUE_POINT_H_

#include <cmath>

#include "ltr/measures/measure.h"
#include "ltr/data/object.h"

using std::fabs;

using ltr::Object;


namespace ltr {
  /**
   * TruePoint pointwise measure. Is 1.0 / (1.0 + AbsError)
   */
  class TruePoint : public PointwiseMeasure {
  public:
    TruePoint(const ParametersContainer& parameters = ParametersContainer())
        : PointwiseMeasure("TruePoint") {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }
    
    double best() const {
      return 1.0;
    }
    double worst() const {
      return 0.0;
    }
  private:
    double get_measure(const Object& object) const {
      return 1/(fabs(object.actualLabel() - object.predictedLabel()) + 1.0);
    }
  };
};

#endif  // LTR_MEASURES_TRUE_POINT_H_
