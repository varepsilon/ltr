// Copyright 2011 Yandex

#ifndef LTR_MEASURES_AVERAGE_PRECISION_H_
#define LTR_MEASURES_AVERAGE_PRECISION_H_

#include "ltr/measures/measure.h"
#include "ltr/data/object_list.h"

namespace ltr {
  /**
   * Average precision listwise measure.
   * See http://en.wikipedia.org/wiki/Information_retrieval#Average_precision
   */
  class AveragePrecision: public MoreIsBetterMeasure<ObjectList> {
  public:
    AveragePrecision(const ParametersContainer&
        parameters = ParametersContainer())
        :MoreIsBetterMeasure<ObjectList>("Average precision") {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }
  /** 
   * Clears parameters container and sets default values:
   * SCORE_FOR_RELEVANT = 3.0 - if object's score is more or equal to SCORE_FOR_RELEVANT,
   * the object is considered to be relevant
   */
  void setDefaultParameters() {
    this->clearParameters();
    this->addDoubleParameter("SCORE_FOR_RELEVANT", 3.0);
  }
  private:
    double get_measure(const ObjectList& objects) const;
  };
};

#endif  // LTR_MEASURES_AVERAGE_PRECISION_H_
