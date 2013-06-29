// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_COMPOSITION_LEARNER_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_
#define LTR_LEARNERS_COMPOSITION_LEARNER_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_

#include <string>
#include <stdexcept>

#include "ltr/utility/boost/shared_ptr.h"

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/composition_scorers/composition_scorer.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/printable.h"

using std::string;
using ltr::Measure;
using ltr::DataSet;

namespace ltr {
namespace composition {
/**
 * Is a parameter for composition learner. Implements a strategy of
 * updating composition's weights while adding a new scorer to it.
 * E. g. simple boosting always updates only the weight of the last scorer,
 * whereas gradient boosting updates all weights in composition
 */
template <class TElement>
class CompositionScorerWeightsUpdater : public Aliaser, public Parameterized {
  ALLOW_SHARED_PTR_ONLY_CREATION(CompositionScorerWeightsUpdater)
 public:
  /**
   * Updates composition's weights
   * @param data - dataset to be used
   * @param composition_scorer - composition scorer to update it's weights.
   * Has last scorer (just added) with weight 1.0
   */
  virtual void updateWeights(const DataSet<TElement>& data,
      CompositionScorer::Ptr composition_scorer) const = 0;
  /**
   * Sets measure, used in BaseCompositionScorerWeightsUpdater. Note that some
   * BaseCompositionScorerWeightsUpdaters don't use measures, so they ignore
   * the mesure setted
   */
  SET(typename Measure<TElement>::Ptr, measure);
 protected:
  typename Measure<TElement>::Ptr measure_;
};

/**
 * Fake CompositionScorerWeightsUpdater, does nothing (all weights in composition
 * scorer will be 1.0)
 */
template <class TElement>
class FakeCompositionScorerWeightsUpdater
    : public CompositionScorerWeightsUpdater<TElement> {
  ALLOW_SHARED_PTR_ONLY_CREATION(FakeCompositionScorerWeightsUpdater)
 public:
  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit FakeCompositionScorerWeightsUpdater(
      const ParametersContainer& parameters = ParametersContainer()) {
  }

  void updateWeights(const DataSet<TElement>& data,
      CompositionScorer::Ptr composition_scorer) const {
    // doing nothing
  }
 private:
  virtual string getDefaultAlias() const {
    return "FakeCompositionScorerWeightsUpdater";
  }
};
};
};

#endif  // LTR_LEARNERS_COMPOSITION_LEARNER_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_
