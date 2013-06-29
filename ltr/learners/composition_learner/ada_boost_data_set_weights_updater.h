// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_COMPOSITION_LEARNER_ADA_BOOST_DATA_SET_WEIGHTS_UPDATER_H_
#define LTR_LEARNERS_COMPOSITION_LEARNER_ADA_BOOST_DATA_SET_WEIGHTS_UPDATER_H_

#include <rlog/rlog.h>

#include <cmath>
#include <string>
#include <vector>
#include <stdexcept>

#include "ltr/utility/boost/shared_ptr.h"

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/composition_scorers/composition_scorer.h"
#include "ltr/learners/composition_learner/data_set_weights_updater.h"

using std::string;
using std::vector;
using std::exp;
using std::logic_error;

using ltr::Measure;
using ltr::DataSet;

namespace ltr {
namespace composition {
/**
 * DataSetWeightsUpdater which implements AdaBoost algorithm's part
 * concerning updating weights of dataset. About AdaBoost see
 * http://en.wikipedia.org/wiki/AdaBoost
 * http://www.machinelearning.ru/wiki/index.php?title=AdaBoost
 */
template <class TElement>
class AdaBoostDataSetWeightsUpdater : public DataSetWeightsUpdater<TElement> {
  ALLOW_SHARED_PTR_ONLY_CREATION(AdaBoostDataSetWeightsUpdater)
 public:
  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit AdaBoostDataSetWeightsUpdater(
      const ParametersContainer& parameters = ParametersContainer()) {
    this->setDefaultParameters();
  }
  /**
   * @param measure Measure to be used for weights updating
   */
  explicit AdaBoostDataSetWeightsUpdater
      (typename Measure<TElement>::Ptr measure) {
    this->set_measure(measure);
  }

  void updateWeights(const DataSet<TElement>* data,
      const CompositionScorer::Ptr& composition_scorer) const;
 private:
  virtual string getDefaultAlias() const {
    return "AdaBoostDataSetWeightsUpdater";
  }
};

// template realizations
template <class TElement>
void AdaBoostDataSetWeightsUpdater<TElement>::updateWeights(
    const DataSet<TElement>* data,
    const CompositionScorer::Ptr& composition_scorer) const {
  if (composition_scorer->size() == 0) {
    rError("Zero-length scorer as an input");
    throw logic_error("Zero-length scorer for " + this->getDefaultAlias());
  }
  int last_scorer_index = static_cast<int>(composition_scorer->size()) - 1;
  (*composition_scorer)[last_scorer_index].scorer->predict(*data);

  vector<double> measure_exps(data->size());
  double sum_of_exps = 0.0;

  double measure_sign;
  if (this->measure_->best() > this->measure_->worst()) {
    measure_sign = 1.0;
  } else {
    measure_sign = -1.0;
  }

  for (size_t element_index = 0;
      element_index < data->size();
      ++element_index) {
    double measure_value = this->measure_->value(data->at(element_index));

    double measure_value_exp = data->getWeight(element_index)
      * exp(-measure_sign *
        (*composition_scorer)[last_scorer_index].weight * measure_value);
    measure_exps[element_index] = measure_value_exp;
    sum_of_exps += measure_exps[element_index];
  }

  for (size_t element_index = 0;
      element_index < data->size();
      ++element_index) {
    data->setWeight(element_index, measure_exps[element_index] / sum_of_exps);
  }
}
};
};

#endif  // LTR_LEARNERS_COMPOSITION_LEARNER_ADA_BOOST_DATA_SET_WEIGHTS_UPDATER_H_
