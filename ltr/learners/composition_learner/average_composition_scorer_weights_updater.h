// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_COMPOSITION_LEARNER_AVERAGE_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_
#define LTR_LEARNERS_COMPOSITION_LEARNER_AVERAGE_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_

#include <string>

#include "ltr/utility/shared_ptr.h" // NOLINT

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/composition_scorers/composition_scorer.h"
#include "ltr/utility/numerical.h"
#include "ltr/scorers/scorer.h"
#include "ltr/learners/composition_learner/composition_scorer_weights_updater.h"

using std::exp;
using std::log;
using std::string;

using ltr::Measure;
using ltr::DataSet;
using ltr::Scorer;

namespace ltr {
namespace composition {
/**
 * CompositionScorerWeightsUpdater wich holds composition scorer's weights equal to
 * 1.0 / (composition length)
 */
template <class TElement, class TCompositionScorer>
class AverageCompositionScorerWeightsUpdater
  : public CompositionScorerWeightsUpdater<TElement, TCompositionScorer> {
 public:
  typedef ltr::utility::shared_ptr<AverageCompositionScorerWeightsUpdater> Ptr;

  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit AverageCompositionScorerWeightsUpdater(
      const ParametersContainer& parameters = ParametersContainer()) {
    this->setDefaultParameters();
  }

  void updateWeights(const DataSet<TElement>& data,
      TCompositionScorer* composition_scorer) const;
 private:
  virtual string getDefaultAlias() const {
    return "AverageCompositionScorerWeightsUpdater";
  }
};

// template realizations
template <class TElement, class TCompositionScorer>
void AverageCompositionScorerWeightsUpdater<TElement, TCompositionScorer>::updateWeights(
    const DataSet<TElement>& data,
    TCompositionScorer* composition_scorer) const {
  double weight = 1.0 / composition_scorer->size();
  for (int scorer_index = 0; scorer_index < composition_scorer->size(); ++scorer_index) {
    composition_scorer->at(scorer_index).weight = weight;
  }
}
};
};

#endif  // LTR_LEARNERS_COMPOSITION_LEARNER_AVERAGE_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_
