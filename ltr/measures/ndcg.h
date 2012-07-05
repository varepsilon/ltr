// Copyright 2011 Yandex

#ifndef LTR_MEASURES_NDCG_H_
#define LTR_MEASURES_NDCG_H_

#include <stdexcept>
#include <algorithm>
#include <vector>
#include <string>

#include "ltr/data/object_list.h"
#include "ltr/measures/measure.h"
#include "ltr/measures/dcg.h"
#include "ltr/measures/utils/measure_utility.h"
#include "ltr/utility/numerical.h"

using std::sort;
using std::logic_error;
using std::vector;
using ltr::utility::PredictedAndActualLabels;
using ltr::utility::ExtractLabels;
using ltr::utility::PredictedDecreasingActualIncreasing;
using ltr::utility::ActualDecreasing;
using ltr::utility::DoubleEqual;
using ltr::ObjectList;

using ltr::DCGFormula;

namespace ltr {
/**
 * NDCG (Normalized DCG) listwise measure, which is parametrized by formula used in it
 */
template<class TDCGFormula>
class BaseNDCG : public ListwiseMeasure {
 public:
  /**
   * @param parameters Standart LTR parameter container with int parameter
   * NUMBER_OF_OBJECTS_TO_CONSIDER (where 0 means consider all docs),
   * by default NUMBER_OF_OBJECTS_TO_CONSIDER = 0
   */
  explicit BaseNDCG(const ParametersContainer& parameters) {
    this->setParameters(parameters);
  }
  
  explicit BaseNDCG(int number_of_objects_to_consider = 0) {
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
  int number_of_objects_to_consider_;
  virtual void setParametersImpl(const ParametersContainer& parameters);
  double get_measure(const ObjectList& objects) const;
  virtual string getDefaultAlias() const {return "NDCG";}
};

typedef BaseNDCG<DCGFormula> NDCG;
typedef BaseNDCG<YandexDCGFormula> YandexNDCG;

// template realizations
template<class TDCGFormula>
double BaseNDCG<TDCGFormula>::best() const {
  return 1.0;
}

template<class TDCGFormula>
double BaseNDCG<TDCGFormula>::worst() const {
  return 0.0;
}

template<class TDCGFormula>
string BaseNDCG<TDCGFormula>::toString() const {
  std::stringstream str;
  str << this->alias();
  str << " measure with parameter NUMBER_OF_OBJECTS_TO_CONSIDER = ";
  str << this->number_of_objects_to_consider_;
  return str.str();
}

template<class TDCGFormula>
void BaseNDCG<TDCGFormula>::setParametersImpl(const ParametersContainer& parameters) {
  number_of_objects_to_consider_ = parameters.template Get<int>("NUMBER_OF_OBJECTS_TO_CONSIDER");
}

template<class TDCGFormula>
double BaseNDCG<TDCGFormula>::get_measure(const ObjectList& objects) const {
  vector<PredictedAndActualLabels> labels = ExtractLabels(objects);
  sort(labels.begin(), labels.end(), ActualDecreasing);

  size_t n = this->number_of_objects_to_consider_;
  if ((n == 0) || (n > labels.size())) {
    n = labels.size();
  }

  double best_possible_DCG = 0.0;
  for (int labels_index = 0; labels_index < n; ++labels_index) {
    best_possible_DCG +=
      TDCGFormula::count(labels[labels_index].actual, labels_index);
  }

  sort(labels.begin(), labels.end(), PredictedDecreasingActualIncreasing);

  double DCG_value = 0.0;
  for (int labels_index = 0; labels_index < n; ++labels_index) {
    DCG_value += TDCGFormula::count(
      labels[labels_index].actual, labels_index);
  }

  if (!utility::DoubleEqual(best_possible_DCG, 0.0)) {
    return DCG_value / best_possible_DCG;
  } else {
    return 0.0;
  }
}

template<class TDCGFormula>
void BaseNDCG<TDCGFormula>::setDefaultParameters() {
  this->number_of_objects_to_consider_ = 0;
}
template<class TDCGFormula>
void BaseNDCG<TDCGFormula>::checkParameters() const {
  if (this->number_of_objects_to_consider_ < 0) {
    throw logic_error(alias() + " NUMBER_OF_OBJECTS_TO_CONSIDER < 0");
  }
}
}
#endif  // LTR_MEASURES_NDCG_H_
