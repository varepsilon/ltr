// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_COMPOSITION_LEARNER_ADA_RANK_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_
#define LTR_LEARNERS_COMPOSITION_LEARNER_ADA_RANK_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_

#include <rlog/rlog.h>

#include <string>
#include <stdexcept>
#include <algorithm>
#include <limits>

#include "ltr/utility/shared_ptr.h"  // NOLINT

#include "ltr/utility/numerical.h"
#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/composition_scorers/composition_scorer.h"
#include "ltr/scorers/scorer.h"
#include "ltr/learners/composition_learner/composition_scorer_weights_updater.h"

using std::exp;
using std::log;
using std::numeric_limits;
using std::swap;
using std::string;
using std::logic_error;

using ltr::Measure;
using ltr::DataSet;
using ltr::utility::DoubleEqual;
using ltr::Scorer;

namespace ltr {
namespace composition {
/**
 * CompositionScorerWeightsUpdater which implements AdaRank algorithm's part
 * concerning updating weights of composition. About AdaRank see
 * http://research.microsoft.com/en-us/people/hangli/xu-sigir07.pdf
 */
template <class TElement>
class AdaRankScorerWeightsUpdater
  : public CompositionScorerWeightsUpdater<TElement> {
 public:
  typedef ltr::utility::shared_ptr<AdaRankScorerWeightsUpdater> Ptr;

  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit AdaRankScorerWeightsUpdater(
      const ParametersContainer& parameters = ParametersContainer()) {
    this->setDefaultParameters();
  }
  /**
   * @param measure Measure to be used for weights updating
   */
  explicit AdaRankScorerWeightsUpdater(
    typename Measure<TElement>::Ptr measure) {
    this->set_measure(measure);
  }

  void updateWeights(const DataSet<TElement>& data,
      CompositionScorer* composition_scorer) const;
 private:
  virtual string getDefaultAlias() const {
    return "AdaRankScorerWeightsUpdater";
  }
};

// template realizations
template <class TElement>
void AdaRankScorerWeightsUpdater<TElement>::updateWeights(
    const DataSet<TElement>& data,
    CompositionScorer* composition_scorer) const {
  if (composition_scorer->size() == 0) {
    rError("Zero-length scorer as an input");
    throw logic_error("Zero-length scorer for " + this->getDefaultAlias());
  }
  if (this->measure_->best() == utility::Inf ||
      this->measure_->best() == -utility::Inf ||
      this->measure_->worst() == utility::Inf ||
      this->measure_->worst() == -utility::Inf) {
    rError("Can't work with an infinity measure");
    throw
      std::logic_error(this->getDefaultAlias() + " has an infinity measure");
  }
  int last_scorer_index = static_cast<int>(composition_scorer->size()) - 1;
  composition_scorer->at(last_scorer_index).scorer->predict(data);

  double numerator = 0.0;
  double denominator = 0.0;

  for (size_t element_index = 0;
       element_index < data.size();
       ++element_index) {
    double measure_value = this->measure_->value(data[element_index]);
    double normalized_measure_value =
      (measure_value - this->measure_->worst()) /
      (this->measure_->best() - this->measure_->worst());

    numerator += data.getWeight(element_index) *
      (1 + normalized_measure_value);
    denominator += data.getWeight(element_index) *
      (1 - normalized_measure_value);
  }

  if (DoubleEqual(denominator, 0.0)) {
    Scorer::Ptr best_scorer =
      (*composition_scorer)[last_scorer_index].scorer;
    composition_scorer->clear();
    composition_scorer->add(best_scorer, 1.0);
    rWarning("Ideal weak scorer has been found");
    return;
  }

  double result_weight = 0.5 * log(numerator/denominator);
  if (result_weight < 0) {
    rWarning("Negative weight in composition");
  }

  (*composition_scorer)[last_scorer_index].weight = result_weight;
}
};
};

#endif  // LTR_LEARNERS_COMPOSITION_LEARNER_ADA_RANK_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_
