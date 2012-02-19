// Copyright 2011 Yandex

#ifndef LTR_MEASURES_AVERAGE_PRECISION_H_
#define LTR_MEASURES_AVERAGE_PRECISION_H_

#include "ltr/measures/measure.h"
#include "ltr/data/object_list.h"

namespace ltr {
  /*
  * http://en.wikipedia.org/wiki/Information_retrieval#Average_precision
  */
  class AveragePrecision: public MoreIsBetterMeasure<ObjectList> {
  public:
    AveragePrecision(const ParametersContainer&
        parameters = ParametersContainer())
        :MoreIsBetterMeasure<ObjectList>("Average precision") {
      this->setDefaultParameters();
      this->parameters().copyParameters(parameters);
      this->checkParameters();
    }

  void setDefaultParameters() {
    this->parameters().clear();
    this->parameters().setDouble("SCORE_FOR_RELEVANT", 3.0);
  }
  private:
    double get_measure(const ObjectList& objects) const;
  };
};

#endif  // LTR_MEASURES_AVERAGE_PRECISION_H_
