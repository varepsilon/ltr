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
  PFound(const ParametersContainer& parameters = ParametersContainer()) {
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
    this->addNewParam("P_BREAK", 0.15);
    this->addNewParam("MAX_LABEL", 5.0);
    this->addNewParam("NUMBER_OF_OBJECTS_TO_CONSIDER", 0);
  }
  /**
   * Checks if 0 <= P_BREAK <= 1, NUMBER_OF_OBJECTS_TO_CONSIDER >=0,
   * MAX_LABEL >= 0 (all should be true)
   */
  void checkParameters() const {
    const Belongs<double> G0L1(0, 1);

    this->checkParameter<double>("P_BREAK", G0L1);
    this->checkParameter<int>("NUMBER_OF_OBJECTS_TO_CONSIDER",
                            std::bind2nd(std::greater_equal<int>(), 0) );
    this->checkParameter<double>("MAX_LABEL",
                             std::bind2nd(std::greater_equal<double>(), 0) );
  }

  double best() const {
    return 1.0;
  }
  double worst() const {
    return 0.0;
  }
  string toString() const;

 private:
  template <class T>
  struct Belongs: public std::unary_function<T, bool> {
    Belongs(const T &min, const T &max): min_(min), max_(max) { }
    bool operator()(const T& x) const {
      return x >= min_ && x <= max_;
    }

  private:
    const T &min_;
    const T &max_;
  };

  double get_measure(const ObjectList& objects) const;
  virtual string getDefaultAlias() const {return "PFound";}
};
};

#endif  // LTR_MEASURES_PFOUND_H_
