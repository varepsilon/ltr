// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_LINEAR_COMPOSITION_ADA_RANK_LC_SCORER_WEIGHTS_UPDATER_H_
#define LTR_LEARNERS_LINEAR_COMPOSITION_ADA_RANK_LC_SCORER_WEIGHTS_UPDATER_H_

#include <string>

#include "ltr/utility/shared_ptr.h"  // NOLINT

#include <algorithm>
#include <limits>

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/composition_scorers/linear_composition_scorer.h"
#include "ltr/utility/numerical.h"
#include "ltr/scorers/scorer.h"
#include "ltr/learners/linear_composition/linear_composition_scorer_weights_updater.h"

using std::exp;
using std::log;
using std::numeric_limits;
using std::swap;
using std::string;

using ltr::Measure;
using ltr::DataSet;
using ltr::LinearCompositionScorer;
using ltr::utility::DOUBLE_EPS;
using ltr::Scorer;

namespace ltr {
namespace lc {
/**
 * LCScorerWeightsUpdater which implements AdaRank algorithm's part
 * concerning updating weights of composition. About AdaRank see
 * http://research.microsoft.com/en-us/people/hangli/xu-sigir07.pdf
 */
template <class TElement>
class AdaRankLCScorerWeightsUpdater
  : public LCScorerWeightsUpdater<TElement> {
 public:
  typedef ltr::utility::shared_ptr<AdaRankLCScorerWeightsUpdater> Ptr;

  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit AdaRankLCScorerWeightsUpdater(
      const ParametersContainer& parameters = ParametersContainer()) {
    this->setDefaultParameters();
  }

  void updateWeights(const DataSet<TElement>& data,
      LinearCompositionScorer* lin_scorer) const;
 private:
  virtual string getDefaultAlias() const {
    return "AdaRankLCScorerWeightsUpdater";
  }
};

// template realizations
template <class TElement>
void AdaRankLCScorerWeightsUpdater<TElement>::updateWeights(
    const DataSet<TElement>& data,
    LinearCompositionScorer* lin_scorer) const {
  int last_scorer_number = static_cast<int>(lin_scorer->size()) - 1;
  lin_scorer->at(last_scorer_number).scorer->predict(data);

  double numerator = 0.0;
  double denominator = 0.0;

  for (int i = 0; i < data.size(); ++i) {
    double measure_value = this->measure_->operator()(data[i]);
    double normalized_measure_value =
      (measure_value - this->measure_->worst()) /
      (this->measure_->best() - this->measure_->worst());

    numerator += data.getWeight(i) * normalized_measure_value;
    denominator += data.getWeight(i) * (1 - normalized_measure_value);
  }

  if (denominator < DOUBLE_EPS) {
    Scorer::Ptr best_scorer = (*lin_scorer)[last_scorer_number].scorer;
    lin_scorer->clear();
    lin_scorer->add(best_scorer, 1.0);
    // report_ = "AdaRank on metric " +
    //  measure_.Alias() + ". Best ranker found!";
    return;
  }

  double result_weight = 0.5 * log(numerator/denominator);

  (*lin_scorer)[last_scorer_number].weight = result_weight;
}
};
};

#endif  // LTR_LEARNERS_LINEAR_COMPOSITION_ADA_RANK_LC_SCORER_WEIGHTS_UPDATER_H_
