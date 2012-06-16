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
    BaseNDCG(const ParametersContainer& parameters = ParametersContainer()) {
      this->setDefaultParameters();
      this->copyParameters(parameters);
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

    double best() const;
    double worst() const;

    string toString() const;

    private:
    double get_measure(const ObjectList& objects) const;
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
    str << this->parameters().template Get<int>(
               "NUMBER_OF_OBJECTS_TO_CONSIDER");
    return str.str();
  }

  template<class TDCGFormula>
  double BaseNDCG<TDCGFormula>::get_measure(const ObjectList& objects) const {
    vector<PredictedAndActualLabels> labels = ExtractLabels(objects);
    sort(labels.begin(), labels.end(), ActualDecreasing);

    size_t n = this->parameters().
               template Get<int>("NUMBER_OF_OBJECTS_TO_CONSIDER");
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
    this->clearParameters();
    this->addNewParam("NUMBER_OF_OBJECTS_TO_CONSIDER", 0);
  }
  template<class TDCGFormula>
  void BaseNDCG<TDCGFormula>::checkParameters() const {
    if (this->parameters().
        template Get<int>("NUMBER_OF_OBJECTS_TO_CONSIDER") < 0) {
      throw logic_error(alias() + " NUMBER_OF_OBJECTS_TO_CONSIDER < 0");
    }
  }
}
#endif  // LTR_MEASURES_NDCG_H_
