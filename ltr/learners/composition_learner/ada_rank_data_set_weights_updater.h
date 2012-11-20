// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_COMPOSITION_LEARNER_ADA_RANK_DATA_SET_WEIGHTS_UPDATER_H_
#define LTR_LEARNERS_COMPOSITION_LEARNER_ADA_RANK_DATA_SET_WEIGHTS_UPDATER_H_

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
 * DataSetWeightsUpdater which implements AdaRank algorithm's part
 * concerning updating weights of dataset. About AdaRank see
 * http://research.microsoft.com/en-us/people/hangli/xu-sigir07.pdf
 */
template <class TElement>
class AdaRankDataSetWeightsUpdater : public DataSetWeightsUpdater<TElement> {
 public:
  typedef ltr::utility::shared_ptr<AdaRankDataSetWeightsUpdater> Ptr;

  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit AdaRankDataSetWeightsUpdater(
      const ParametersContainer& parameters = ParametersContainer()) {
    this->setDefaultParameters();
  }
  /**
   * @param measure Measure to be used for weights updating
   */
  explicit AdaRankDataSetWeightsUpdater
      (typename Measure<TElement>::Ptr measure) {
    this->set_measure(measure);
  }

  void updateWeights(const DataSet<TElement>* data,
      const CompositionScorer& composition_scorer) const;
 private:
  virtual string getDefaultAlias() const {
    return "AdaRankDataSetWeightsUpdater";
  }
};

// template realizations
template <class TElement>
void AdaRankDataSetWeightsUpdater<TElement>::updateWeights(
    const DataSet<TElement>* data,
    const CompositionScorer& composition_scorer) const {
  if (composition_scorer.size() == 0) {
    rError("Zero-length scorer as an input");
    throw logic_error("Zero-length scorer for " + this->getDefaultAlias());
  }
  if (this->measure_->best() == utility::Inf ||
      this->measure_->best() == -utility::Inf ||
      this->measure_->worst() == utility::Inf ||
      this->measure_->worst() == -utility::Inf) {
    rError("Can't work with an infinity measure");
    throw std::logic_error(this->getDefaultAlias()
      + " has an infinity measure");
  }
  composition_scorer.predict(*data);

  vector<double> measure_exps(data->size());
  double sum_of_exps = 0.0;

  for (size_t element_index = 0;
      element_index < data->size();
      ++element_index) {
    double measure_value = this->measure_->value(data->at(element_index));
    double normalized_measure_value =
      (measure_value - this->measure_->worst()) /
      (this->measure_->best() - this->measure_->worst());

    double measure_value_exp = exp(-2 * normalized_measure_value);
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

#endif  // LTR_LEARNERS_COMPOSITION_LEARNER_ADA_RANK_DATA_SET_WEIGHTS_UPDATER_H_
