// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_LINEAR_COMPOSITION_LINEAR_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_
#define LTR_LEARNERS_LINEAR_COMPOSITION_LINEAR_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_

#include <boost/shared_ptr.hpp>
#include <string>

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/linear_composition_scorer.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/interfaces/parameterized.h"

using std::string;
using ltr::Measure;
using ltr::DataSet;
using ltr::LinearCompositionScorer;

namespace ltr {
namespace lc {
/**
 * Is a parameter for linear composition learner. Implements a strategy of
 * updating linear composition's weights while adding a new scorer to it.
 * E. g. simple boosting always updates only the weight of the last scorer,
 * whereas gradient boosting updates all weights in composition
 */
template <class TElement>
class LCScorerWeightsUpdater : public Aliaser, public Parameterized {
 public:
  typedef boost::shared_ptr<LCScorerWeightsUpdater> Ptr;

  /**
   * Updates linear composition's weights
   * @param data - dataset, not upadated by DataSetWeightsUpdater on
   * current iteration
   * @param lin_scorer - linear composition scorer to update it's weights.
   * Has last scorer (just added) with weight 1.0
   */
  virtual void updateWeights(const DataSet<TElement>& data,
      LinearCompositionScorer* lin_scorer) const =0;
  /**
   * Sets measure, used in LCScorerWeightsUpdater. Note that some
   * LCScorerWeightsUpdaters don't use measures, so they ignore 
   * the mesure setted
   */
  void set_measure(typename Measure<TElement>::Ptr in_measure) {
    measure_ = in_measure;
  }
 protected:
  typename Measure<TElement>::Ptr measure_;
};


/**
 * Fake LCScorerWeightsUpdater, does nothing (all weights in composition
 * scorer will be 1.0)
 */
template <class TElement>
class FakeLCScorerWeightsUpdater : public LCScorerWeightsUpdater<TElement> {
 public:
  typedef boost::shared_ptr<FakeLCScorerWeightsUpdater> Ptr;

  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit FakeLCScorerWeightsUpdater(
      const ParametersContainer& parameters = ParametersContainer()) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
  }

  void updateWeights(const DataSet<TElement>& data,
      LinearCompositionScorer* lin_scorer) const {
    // doing nothing
  }
 private:
  virtual string getDefaultAlias() const {return "FakeLCScorerWeightsUpdater";}
};
};
};

#endif  // LTR_LEARNERS_LINEAR_COMPOSITION_LINEAR_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_
