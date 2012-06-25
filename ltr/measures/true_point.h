// Copyright 2011 Yandex

#ifndef LTR_MEASURES_TRUE_POINT_H_
#define LTR_MEASURES_TRUE_POINT_H_

#include <cmath>
#include <string>

#include "ltr/measures/measure.h"
#include "ltr/data/object.h"

using std::fabs;
using std::string;
using ltr::Object;


namespace ltr {
  /**
   * TruePoint pointwise measure. Is 1.0 / (1.0 + AbsError)
   */
  class TruePoint : public PointwiseMeasure {
  public:
    TruePoint(const ParametersContainer& parameters = ParametersContainer()) {
      setDefaultParameters();
      copyParameters(parameters);
    }

    double best() const {
      return 1.0;
    }
    double worst() const {
      return 0.0;
    }
    string toString() const {
      return "True point measure";
    }
  private:
    double get_measure(const Object& object) const {
      return 1.0/(fabs(object.actual_label() - object.predicted_label()) + 1.0);
    }
    virtual string getDefaultAlias() const {return "TruePoint";}
  };
};

#endif  // LTR_MEASURES_TRUE_POINT_H_
