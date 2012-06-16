// Copyright 2011 Yandex

#ifndef LTR_MEASURES_GMRR_H_
#define LTR_MEASURES_GMRR_H_

#include <functional>
#include <string>

#include "ltr/measures/measure.h"
#include "ltr/data/object_list.h"

using std::string;
using ltr::ObjectList;

namespace ltr {
  /**
   * Graded Mean Reciprocal Rank
   * See http://romip.ru/romip2010/20_appendix_a_metrics.pdf pp 185
   */
  class GMRR: public ListwiseMeasure {
  public:
    GMRR(const ParametersContainer& parameters = ParametersContainer()) {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }
    /**
    * MAX_LABEL Maximal possible relevance of document
    * NUMBER_OF_OBJECTS_TO_CONSIDER Number of top documents to consider
    */
    void setDefaultParameters() {
      this->clearParameters();
      this->addNewParam("MAX_LABEL", 5.0);
      this->addNewParam("NUMBER_OF_OBJECTS_TO_CONSIDER", 0);
    }
    void checkParameters() const {
      this->checkParameter<int>("NUMBER_OF_OBJECTS_TO_CONSIDER",
                                   std::bind2nd(std::greater_equal<int>(), 0));
      this->checkParameter<double>("MAX_LABEL",
                             std::bind2nd(std::greater_equal<double>(), 0));
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
     * @param label - label of the current object in list
     */
    double GMRRFormula(double label) const;
  };
};

#endif  // LTR_MEASURES_GMRR_H_
