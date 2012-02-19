// Copyright 2011 Yandex

#ifndef LTR_MEASURES_NDCG_H_
#define LTR_MEASURES_NDCG_H_

#include <stdexcept>
#include <algorithm>
#include <vector>

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
    BaseNDCG(const ParametersContainer& parameters = ParametersContainer())
    :ListwiseMeasure("NDCG with " + TDCGFormula().alias()) {
      this->setDefaultParameters();
      this->parameters().copyParameters(parameters);
      this->checkParameters();
    }

    /** 
     * Clears parameters container and sets default values:
     * NUMBER_OF_OBJECTS_TOCONSIDER = 0
     */
    void setDefaultParameters();
    /**
     * Checks if NUMBER_OF_OBJECTS_TOCONSIDER > 0 (should be true)
     */
    void checkParameters() const;

    bool better(double expected_better, double expected_worse) const;

    private:
    double get_measure(const ObjectList& objects) const;
  };

  typedef BaseNDCG<DCGFormula> NDCG;
  typedef BaseNDCG<YandexDCGFormula> YandexNDCG;

  // template realizations
  template<class TDCGFormula>
  bool BaseNDCG<TDCGFormula>::better(double expected_better, double expected_worse) const {
    TDCGFormula formula;
    return formula.better(expected_better, expected_worse);
  }

  template<class TDCGFormula>
  double BaseNDCG<TDCGFormula>::get_measure(const ObjectList& objects) const {
    vector<PredictedAndActualLabels> labels = ExtractLabels(objects);
    sort(labels.begin(), labels.end(), ActualDecreasing);

    size_t n = this->parameters().getInt("NUMBER_OF_OBJECTS_TO_CONSIDER");
    if ((n == 0) || (n > labels.size())) {
      n = labels.size();
    }

    TDCGFormula formula;

    double best_possible_DCG = 0.0;
    for (int labels_index = 0; labels_index < n; ++labels_index) {
      best_possible_DCG +=
        formula(labels[labels_index].actual, labels_index);
    }

    sort(labels.begin(), labels.end(), PredictedDecreasingActualIncreasing);

    double DCG_value = 0.0;
    for (int labels_index = 0; labels_index < n; ++labels_index) {
      DCG_value += formula(labels[labels_index].actual, labels_index);
    }

    if (!utility::DoubleEqual(best_possible_DCG, 0.0)) {
      double result =  DCG_value / best_possible_DCG;
      if (result > 1.0) {
        throw logic_error(alias() + " calculated > 1");
      } else if (result < 0.0) {
        throw logic_error(alias() + " calculated < 0");
      }
      return result;
    } else {
      return 0.0;
    }
  }

  template<class TDCGFormula>
  void BaseNDCG<TDCGFormula>::setDefaultParameters() {
    this->parameters().clear();
    this->parameters().setInt("NUMBER_OF_OBJECTS_TO_CONSIDER", 0);
  }
  template<class TDCGFormula>
  void BaseNDCG<TDCGFormula>::checkParameters() const {
    if (this->parameters().getInt("NUMBER_OF_OBJECTS_TO_CONSIDER") < 0) {
      throw logic_error(alias() + " NUMBER_OF_OBJECTS_TO_CONSIDER < 0");
    }
  }
}
#endif  // LTR_MEASURES_NDCG_H_
