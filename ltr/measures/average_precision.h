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
    }

    void setDefaultParameters();
  private:
    double get_measure(ObjectList& objects) const;
  };
};

#endif  // LTR_MEASURES_AVERAGE_PRECISION_H_
