// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_LINEAR_COMPOSITION_ADA_RANK_DATA_SET_WEIGHTS_UPDATER_H_
#define LTR_LEARNERS_LINEAR_COMPOSITION_ADA_RANK_DATA_SET_WEIGHTS_UPDATER_H_

#include <cmath>
#include <string>
#include <vector>

#include "ltr/data/data_set.h"

#include "ltr/learners/linear_composition/data_set_weights_updater.h"

#include "ltr/scorers/composition_scorers/linear_composition_scorer.h"

#include "ltr/utility/shared_ptr.h"

#include "ltr/measures/measure.h"

using std::string;
using std::vector;
using std::exp;

using ltr::Measure;
using ltr::DataSet;
using ltr::LinearCompositionScorer;

namespace ltr {
namespace lc {
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

  void updateWeights(const DataSet<TElement>* data,
      const LinearCompositionScorer& lin_scorer) const;
 private:
  virtual string getDefaultAlias() const {
    return "AdaRankDataSetWeightsUpdater";
  }
};

// template realizations
template <class TElement>
void AdaRankDataSetWeightsUpdater<TElement>::updateWeights(
    const DataSet<TElement>* data,
    const LinearCompositionScorer& lin_scorer) const {
  int last_scorer_number = static_cast<int>(lin_scorer.size()) - 1;
  lin_scorer[last_scorer_number].scorer->predict(*data);

  vector<double> measure_exps(data->size());
  double sum_exps = 0.0;

  for (int data_index = 0; data_index < data->size(); ++data_index) {
    double measure_value = this->measure_->operator()(data->at(data_index));
    double normalized_measure_value =
      (measure_value - this->measure_->worst()) /
      (this->measure_->best() - this->measure_->worst());

    double measure_value_exp = exp(-normalized_measure_value);
    measure_exps[data_index] = measure_value_exp;
    sum_exps += measure_value_exp;
  }

  for (int data_index = 0; data_index < data->size(); ++data_index) {
    data->setWeight(data_index, measure_exps[data_index] / sum_exps);
  }
}
};
};

#endif  // LTR_LEARNERS_LINEAR_COMPOSITION_ADA_RANK_DATA_SET_WEIGHTS_UPDATER_H_
