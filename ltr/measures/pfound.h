// Copyright 2011 Yandex

#ifndef LTR_MEASURES_PFOUND_H_
#define LTR_MEASURES_PFOUND_H_

#include <stdexcept>
#include <functional>
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
  explicit PFound(const ParametersContainer& parameters) {
    this->setParameters(parameters);
  }

  explicit PFound(double p_break = 0.15, 
                  double max_label = 5.0, 
                  int number_of_objects_to_consider = 0) {
    p_break_ = p_break;
    max_label_ = max_label;
    number_of_objects_to_consider_ = number_of_objects_to_consider;
  }


  GET_SET(double, p_break);
  GET_SET(double, max_label);
  GET_SET(int, number_of_objects_to_consider);
  /**
   * Clears parameters container and sets default values:
   * P_BREAK = 0.15 - probability of unexpected user's break
   * MAX_LABEL = 5.0 - the maximal possible relevance of (ideal) document
   * NUMBER_OF_OBJECTS_TO_CONSIDER = 0 - number of top documents to consider
   * (0 means all docs)
   */
  void setDefaultParameters() {
    p_break_ = 0.15;
    max_label_ = 5.0;
    number_of_objects_to_consider_ = 0;
  }
  /**
   * Checks if 0 <= P_BREAK <= 1, NUMBER_OF_OBJECTS_TO_CONSIDER >=0,
   * MAX_LABEL >= 0 (all should be true)
   */
  void checkParameters() const {
    CHECK(p_break_ >= 0);
    CHECK(p_break_ <= 1);
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
  int number_of_objects_to_consider_;
  double p_break_;
  double max_label_;

  virtual void setParametersImpl(const ParametersContainer& parameters) {
    p_break_ = parameters.Get<double>("P_BREAK");
    max_label_ = parameters.Get<double>("MAX_LABEL");
    number_of_objects_to_consider_ = 
      parameters.Get<int>("NUMBER_OF_OBJECTS_TO_CONSIDER");
  }
  
  double get_measure(const ObjectList& objects) const;
  virtual string getDefaultAlias() const {return "PFound";}
};
};

#endif  // LTR_MEASURES_PFOUND_H_
