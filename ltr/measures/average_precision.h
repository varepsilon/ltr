// Copyright 2011 Yandex

#ifndef LTR_MEASURES_AVERAGE_PRECISION_H_
#define LTR_MEASURES_AVERAGE_PRECISION_H_

#include <string>
#include "ltr/measures/measure.h"
#include "ltr/data/object_list.h"

using std::string;

namespace ltr {
/**
 * Average precision listwise measure.
 * See http://en.wikipedia.org/wiki/Information_retrieval#Average_precision
 */
class AveragePrecision: public ListwiseMeasure {
 public:
  explicit AveragePrecision(const ParametersContainer&
      parameters) {
    this->setParameters(parameters);
  }

  explicit AveragePrecision(double score_for_relevant = 3.0) {
    score_for_relevant_ = score_for_relevant;
  }
  /**
   * Clears parameters container and sets default values:
   * SCORE_FOR_RELEVANT = 3.0 - if object's score is more or equal to SCORE_FOR_RELEVANT,
   * the object is considered to be relevant
   */

  GET_SET(double, score_for_relevant);

  virtual void setDefaultParameters() {
    score_for_relevant_ = 3.0;
  }
  double best() const {
    return 1.0;
  }
  double worst() const {
    return 0.0;
  }
  string toString() const;
 private:
  virtual void setParametersImpl(const ParametersContainer& parameters) {
    score_for_relevant_ = parameters.Get<double>("SCORE_FOR_RELEVANT");
  }
  double score_for_relevant_;
  double get_measure(const ObjectList& objects) const;
  virtual string getDefaultAlias() const {return "AveragePrecision";}
};
};

#endif  // LTR_MEASURES_AVERAGE_PRECISION_H_
