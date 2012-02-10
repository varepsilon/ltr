// Copyright 2011 Yandex

#ifndef LTR_MEASURES_RECIPROCAL_RANK_H_
#define LTR_MEASURES_RECIPROCAL_RANK_H_

#include "ltr/measures/measure.h"
#include "ltr/data/object_list.h"

using ltr::ObjectList;

namespace ltr {
  class ReciprocalRank : public MoreIsBetterMeasure<ObjectList> {
  public:
    ReciprocalRank(const ParametersContainer&
        parameters = ParametersContainer())
        :MoreIsBetterMeasure<ObjectList>("Reciprocal Rank") {
      this->setDefaultParameters();
      this->parameters().copyParameters(parameters);
    }

    void setDefaultParameters() {
      this->parameters().setDouble("SCORE_FOR_RELEVANT", 3.0);
    }

  private:
    double get_measure(const ObjectList& objects) const;
    /**
     * Some decreasing function, usually f(x) = 1/x
     * @param pos Position of the first RELEVANT object
     */
    double RRFormula(double pos) const;
  };
};

#endif  // LTR_MEASURES_RECIPROCAL_RANK_H_
