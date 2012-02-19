// Copyright 2011 Yandex

#ifndef LTR_MEASURES_GMRR_H_
#define LTR_MEASURES_GMRR_H_

#include "ltr/measures/measure.h"

namespace ltr {
  /**
   * Graded Mean Reciprocal Rank
   * See http://romip.ru/romip2010/20_appendix_a_metrics.pdf pp 185
   */
  class GMRR: public ListwiseMeasure {
  public:
    GMRR(const ParametersContainer& parameters = ParametersContainer())
        :ListwiseMeasure("GMRR") {
      this->setDefaultParameters();
      this->parameters().copyParameters(parameters);
      this->checkParameters();
    }
    /**
    * MAX_LABEL Maximal possible relevance of document
    * NUMBER_OF_OBJECTS_TO_CONSIDER Number of top documents to consider
    */
    void setDefaultParameters() {
      this->parameters().clear();
      this->parameters().setDouble("MAX_LABEL", 5.0);
      this->parameters().setInt("NUMBER_OF_OBJECTS_TO_CONSIDER", 0);
    }
    void checkParameters() const {
      if (this->parameters().getInt("NUMBER_OF_OBJECTS_TO_CONSIDER") < 0) {
        throw logic_error(alias() + " NUMBER_OF_OBJECTS_TO_CONSIDER < 0");
      }
      if (this->parameters().getDouble("MAX_LABEL") < 0.0) {
        throw logic_error(alias() + " MAX_LABEL < 0");
      }
    }
  private:
    double get_measure(const ObjectList& objects) const;
    /**
     * @param pos Current position in the sorted list
     * @param label Label of the current object in list
     * @param p_look The probability of that user have reached the current position
     * without meeting some good enough document
     */
    double GMRRFormula(double pos, double label, double p_look) const;
  };
};

#endif  // LTR_MEASURES_GMRR_H_
