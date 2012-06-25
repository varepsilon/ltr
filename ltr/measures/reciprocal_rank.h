// Copyright 2011 Yandex

#ifndef LTR_MEASURES_RECIPROCAL_RANK_H_
#define LTR_MEASURES_RECIPROCAL_RANK_H_

#include <string>

#include "ltr/measures/measure.h"
#include "ltr/data/object_list.h"

using std::string;
using ltr::ObjectList;

namespace ltr {
/**
 * Reciprocal rank listwise measure.
 * Returns f(x), where x is position of first relevant object.
 * Usually f(x) = 1.0 / x
 */
class ReciprocalRank : public ListwiseMeasure {
 public:
  /**
   * @param parameters Standart LTR parameter container with int parameter
   * SCORE_FOR_RELEVANT, by default SCORE_FOR_RELEVANT = 3.0
   */
  ReciprocalRank(const ParametersContainer&
      parameters = ParametersContainer()) {
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
    this->addNewParam("SCORE_FOR_RELEVANT", 3.0);
  }

  double best() const {
    return 1.0;
  }
  double worst() const {
    return 0.0;
  }
  string toString() const;

 private:
  double get_measure(const ObjectList& objects) const;
  /**
   * Some decreasing function, usually f(x) = 1/x
   * @param pos - position of the first relevant object
   */
  static double RRFormula(int pos);
  virtual string getDefaultAlias() const {return "ReciprocalRank";}
};
};

#endif  // LTR_MEASURES_RECIPROCAL_RANK_H_
