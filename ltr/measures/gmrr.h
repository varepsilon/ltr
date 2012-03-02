// Copyright 2011 Yandex

#ifndef LTR_MEASURES_GMRR_H_
#define LTR_MEASURES_GMRR_H_

#include "ltr/measures/measure.h"
#include "ltr/data/object_list.h"

using ltr::ObjectList;

namespace ltr {
  /**
   * Graded Mean Reciprocal Rank
   * See http://romip.ru/romip2010/20_appendix_a_metrics.pdf pp 185
   */
  class GMRR: public ListwiseMeasure {
  public:
    GMRR(const ParametersContainer& parameters = ParametersContainer())
        : ListwiseMeasure("GMRR") {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }
    /**
    * MAX_LABEL Maximal possible relevance of document
    * NUMBER_OF_OBJECTS_TO_CONSIDER Number of top documents to consider
    */
    void setDefaultParameters() {
      this->clearParameters();
      this->addDoubleParameter("MAX_LABEL", 5.0);
      this->addIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 0);
    }
    void checkParameters() const {
      CHECK_INT_PARAMETER("NUMBER_OF_OBJECTS_TO_CONSIDER", X >= 0);
      CHECK_DOUBLE_PARAMETER("MAX_LABEL", X >= 0);
    }

    double best() const {
      return 1.0;
    }
    double worst() const {
      return 0.0;
    }
  private:
    double get_measure(const ObjectList& objects) const;
    /**
     * @param label - label of the current object in list
     */
    double GMRRFormula(double label) const;
  };
};

#endif  // LTR_MEASURES_GMRR_H_
