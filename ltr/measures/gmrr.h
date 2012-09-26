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
  explicit GMRR(const ParametersContainer& parameters) {
    this->setParameters(parameters);
  }

  GET_SET(double, max_label);
  GET_SET(int, number_of_objects_to_consider);

  explicit GMRR(double max_label = 5.0,
                int number_of_objects_to_consider = 0) {
     max_label_ = max_label;
     number_of_objects_to_consider_ = number_of_objects_to_consider;
  }
  /**
  * MAX_LABEL Maximal possible relevance of document
  * NUMBER_OF_OBJECTS_TO_CONSIDER Number of top documents to consider
  */
  virtual void setDefaultParameters() {
    set_max_label(5.0);
    set_number_of_objects_to_consider(0);
  }
  virtual void checkParameters() const {
    CHECK(number_of_objects_to_consider_ >= 0);
    CHECK(max_label_ >= 0);
  }

  double best() const {
    return 1.0;
  }
  double worst() const {
    return 0.0;
  }
  string toString() const;

 private:
  double max_label_;
  int number_of_objects_to_consider_;
  double get_measure(const ObjectList& objects) const;
  /**
   * @param label - label of the current object in list
   */
  double GMRRFormula(double label) const;
  virtual string getDefaultAlias() const {return "GMRR";}
  virtual void setParametersImpl(const ParametersContainer& parameters) {
    max_label_ = parameters.Get<double>("MAX_LABEL");
    number_of_objects_to_consider_ =
      parameters.Get<int>("NUMBER_OF_OBJECTS_TO_CONSIDER");
  }
};
};

#endif  // LTR_MEASURES_GMRR_H_
