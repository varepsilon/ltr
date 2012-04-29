// Copyright 2011 Yandex

#ifndef LTR_MEASURES_PFOUND_H_
#define LTR_MEASURES_PFOUND_H_

#include <stdexcept>
#include <string>

#include "ltr/measures/measure.h"
#include "ltr/data/object_list.h"

using std::string;
using std::logic_error;
using ltr::ObjectList;

namespace ltr {
  class PFound: public ListwiseMeasure {
  public:
    /**
     * Yandex pFound listwise measure. See http://romip.ru/romip2010/20_appendix_a_metrics.pdf pp 186.
     * Here pRel = object's score / MAX_LABEL
     */
    PFound(const ParametersContainer& parameters = ParametersContainer())
        : ListwiseMeasure("PFound") {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }
    /**
     * Clears parameters container and sets default values:
     * P_BREAK = 0.15 - probability of unexpected user's break
     * MAX_LABEL = 5.0 - the maximal possible relevance of (ideal) document
     * NUMBER_OF_OBJECTS_TO_CONSIDER = 0 - number of top documents to consider
     * (0 means all docs)
     */
    void setDefaultParameters() {
      this->clearParameters();
      this->addDoubleParameter("P_BREAK", 0.15);
      this->addDoubleParameter("MAX_LABEL", 5.0);
      this->addIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 0);
    }
    /**
     * Checks if 0 <= P_BREAK <= 1, NUMBER_OF_OBJECTS_TO_CONSIDER >=0,
     * MAX_LABEL >= 0 (all should be true)
     */
    void checkParameters() const {
      CHECK_DOUBLE_PARAMETER("P_BREAK", X >= 0 && X <= 1);
      CHECK_INT_PARAMETER("NUMBER_OF_OBJECTS_TO_CONSIDER", X >= 0);
      CHECK_DOUBLE_PARAMETER("MAX_LABEL", X >= 0);
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
  };
};

#endif  // LTR_MEASURES_PFOUND_H_
