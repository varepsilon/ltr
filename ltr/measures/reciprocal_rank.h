// Copyright 2011 Yandex

#ifndef LTR_MEASURES_RECIPROCAL_RANK_H_
#define LTR_MEASURES_RECIPROCAL_RANK_H_

#include "ltr/measures/measure.h"
#include "ltr/data/object_list.h"

using ltr::ObjectList;

namespace ltr {
  /**
   * Reciprocal rank listwise measure.
   * Returns f(x), where x is position of first relevant object.
   * Usually f(x) = 1.0 / x
   */
  class ReciprocalRank : public MoreIsBetterMeasure<ObjectList> {
  public:
    ReciprocalRank(const ParametersContainer&
        parameters = ParametersContainer())
        :MoreIsBetterMeasure<ObjectList>("Reciprocal Rank") {
      this->setDefaultParameters();
      this->parameters().copyParameters(parameters);
      this->checkParameters();
    }
    /** 
     * Clears parameters container and sets default values:
     * SCORE_FOR_RELEVANT = 3.0, if object's score is more or equal to SCORE_FOR_RELEVANT,
     * the object is considered to be relevant
     */
    void setDefaultParameters() {
      this->parameters().clear();
      this->parameters().setDouble("SCORE_FOR_RELEVANT", 3.0);
    }

  private:
    double get_measure(const ObjectList& objects) const;
    /**
     * Some decreasing function, usually f(x) = 1/x
     * @param pos - position of the first relevant object
     */
    static double RRFormula(double pos);
  };
};

#endif  // LTR_MEASURES_RECIPROCAL_RANK_H_
