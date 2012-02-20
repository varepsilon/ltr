// Copyright 2011 Yandex

#ifndef LTR_MEASURES_PFOUND_H_
#define LTR_MEASURES_PFOUND_H_

#include <stdexcept>

#include "ltr/measures/measure.h"
#include "ltr/data/object_list.h"

using std::logic_error;
using ltr::ObjectList;

namespace ltr {
  class PFound: public MoreIsBetterMeasure<ObjectList> {
  public:
    /**
     * Yandex pFound listwise measure. See http://romip.ru/romip2010/20_appendix_a_metrics.pdf pp 186.
     * Here pRel = object's score / MAX_LABEL
     */
    PFound(const ParametersContainer& parameters = ParametersContainer())
        :MoreIsBetterMeasure<ObjectList>("PFound") {
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
      if (this->getDoubleParameter("P_BREAK") > 1.0) {
        throw logic_error(alias() + " P_BREAK > 1");
      }
      if (this->getDoubleParameter("P_BREAK") < 0.0) {
        throw logic_error(alias() + " P_BREAK < 0");
      }
      if (this->getIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER") < 0) {
        throw logic_error(alias() + " NUMBER_OF_OBJECTS_TO_CONSIDER < 0");
      }
      if (this->getDoubleParameter("MAX_LABEL") < 0.0) {
        throw logic_error(alias() + " MAX_LABEL < 0");
      }
    }

  private:
    double get_measure(const ObjectList& objects) const;
  };
};

#endif  // LTR_MEASURES_PFOUND_H_
