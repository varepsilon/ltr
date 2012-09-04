// Copyright 2011 Yandex

#ifndef LTR_MEASURES_DCG_H_
#define LTR_MEASURES_DCG_H_

#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <functional>

#include "ltr/data/object_list.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/measures/measure.h"
#include "ltr/measures/utils/measure_utility.h"

using std::string;
using std::sort;
using std::logic_error;
using std::string;
using std::vector;
using ltr::utility::PredictedAndActualLabels;
using ltr::utility::ExtractLabels;
using ltr::utility::PredictedDecreasingActualIncreasing;
using ltr::ObjectList;

namespace ltr {
/**
 * DCGFormula is formula, which result is added to the DCG measure for each position.
 * Also it contains information if resulted DCG measure is MoreIsBetter of LessIsBetter
 * and what bounds has DCG result (e.g. it must be non-negative)
 */
class DCGFormula {
 public:
  /**
   * The result of this function is added to the DCG for each position.
   * @param relevance - the relevance of the object in position
   * @param position - the position number 0..size() - 1
   */
  static double count(double relevance, size_t position);
  static double best();
  static double worst();
  static string alias() {return "DCG";}
};

/**
 * Yandex DCG formula from http://imat2009.yandex.ru/datasets
 */
class YandexDCGFormula {
 public:
  static double count(double relevance, size_t position);
  static double best();
  static double worst();
  static string alias() {return "YandexDCG";}
};


/**
 * DCG listwise measure, which is parametrized by formula used in it
 */
template<class TDCGFormula>
class BaseDCG : public ListwiseMeasure {
 public:
  /**
   * @param parameters Standart LTR parameter container with int parameter
   * NUMBER_OF_OBJECTS_TO_CONSIDER (where 0 means consider all docs),
   * by default NUMBER_OF_OBJECTS_TO_CONSIDER = 0
   */
  explicit BaseDCG(const ParametersContainer& parameters) {
    this->setParameters(parameters);
  }

  explicit BaseDCG(int number_of_objects_to_consider = 0) {
    number_of_objects_to_consider_ = number_of_objects_to_consider;
  }

  GET_SET(int, number_of_objects_to_consider);

  /**
   * Clears parameters container and sets default values:
   * NUMBER_OF_OBJECTS_TOCONSIDER = 0
   */
  void setDefaultParameters();
  /**
   * Checks if NUMBER_OF_OBJECTS_TOCONSIDER > 0 (should be true)
   */
  void checkParameters() const;

  double best() const;
  double worst() const;

  string toString() const;

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters);
  int number_of_objects_to_consider_;
  double get_measure(const ObjectList& objects) const;
  virtual string getDefaultAlias() const {return TDCGFormula::alias();}
};

typedef BaseDCG<DCGFormula> DCG;
typedef BaseDCG<YandexDCGFormula> YandexDCG;

// template realizations
template<class TDCGFormula>
double BaseDCG<TDCGFormula>::best() const {
  return TDCGFormula::best();
}

template<class TDCGFormula>
double BaseDCG<TDCGFormula>::worst() const {
  return TDCGFormula::worst();
}

template<class TDCGFormula>
string BaseDCG<TDCGFormula>::toString() const {
  std::stringstream str;
  str << this->alias();
  str << " measure with parameter NUMBER_OF_OBJECTS_TO_CONSIDER = ";
  str << this->number_of_objects_to_consider_;
  return str.str();
}

template<class TDCGFormula>
void BaseDCG<TDCGFormula>::
  setParametersImpl(const ParametersContainer& parameters) {
  this->number_of_objects_to_consider_ =
    parameters.template Get<int>("NUMBER_OF_OBJECTS_TO_CONSIDER");
}

template<class TDCGFormula>
double BaseDCG<TDCGFormula>::get_measure(const ObjectList& objects) const {
  vector<PredictedAndActualLabels> labels = ExtractLabels(objects);
  sort(labels.begin(), labels.end(), PredictedDecreasingActualIncreasing);

  int n = this->number_of_objects_to_consider_;
  if ((n == 0) || (n > (int)labels.size())) {
    n = labels.size();
  }

  double result = 0.0;
  for (int labels_index = 0; labels_index < n; ++labels_index) {
    result += TDCGFormula::count(labels[labels_index].actual, labels_index);
  }

  return result;
}

template<class TDCGFormula>
void BaseDCG<TDCGFormula>::setDefaultParameters() {
  this->number_of_objects_to_consider_ = 0;
}
template<class TDCGFormula>
void BaseDCG<TDCGFormula>::checkParameters() const {
  CHECK(this->number_of_objects_to_consider_ >= 0);
}
};
#endif  // LTR_MEASURES_DCG_H_
