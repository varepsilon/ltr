// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_LINEAR_COMPOSITION_LINEAR_COMPOSITION_LEARNER_H_
#define LTR_LEARNERS_LINEAR_COMPOSITION_LINEAR_COMPOSITION_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <string>
#include <functional>

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/composition_scorers/linear_composition_scorer.h"
#include "ltr/learners/learner.h"
#include "ltr/learners/linear_composition/data_set_weights_updater.h"
#include "ltr/learners/linear_composition/linear_composition_scorer_weights_updater.h"

using std::string;

using ltr::Measure;
using ltr::DataSet;
using ltr::LinearCompositionScorer;
using ltr::Learner;
using ltr::lc::FakeDataSetWeightsUpdater;
using ltr::lc::FakeLCScorerWeightsUpdater;
using ltr::lc::LCScorerWeightsUpdater;
using ltr::lc::DataSetWeightsUpdater;


namespace ltr {
namespace lc {
/**
 * BaseLearner, wich uses other learner (weak learner), produce with it
 * different scorers and tie them into linear composition scorer.
 * Has some extra parameters:
 * weak learner - learner to be used in producing scorers - elements
 * for linear composition scorer

 * LCScorerWeightsUpdater - a strategy for setting weights to scorers in
 * linear composition scorer. By default is fake, so every scorer in
 * composition has 1.0 weight

 * DataSetWeightsUpdater - a strategy for setting weights for dataset's
 * elements before each weak learner's learn() call. Provides the variability
 * of weak scorers. By default is fake, so dataset weights remain const

 * BaseFeatureConverterLearner - let weak learner call learn() on dataset with
 * converted features (e. g. RSM - random subspace method). Provides the
 * variability of weak scorers. By default is fake, so dataset is not pre-
 * feature converted before weak learning
 */
template <class TElement>
class LinearCompositionLearner
    : public BaseLearner<TElement, LinearCompositionScorer> {
 public:
  typedef boost::shared_ptr<LinearCompositionLearner> Ptr;

  /**
   * @param parameters Standart LTR parameter container with int parameter
   * NUMBER_OF_ITERATIONS (number of scorers to be added into linear composition
   * scorer). By default NUMBER_OF_ITERATIONS = 10
   */
  explicit LinearCompositionLearner(
      const ParametersContainer& parameters)
      : data_set_weights_updater(new FakeDataSetWeightsUpdater<TElement>),
      linear_composition_scorer_weights_updater
        (new FakeLCScorerWeightsUpdater<TElement>) {
    this->setParameters(parameters);
  }

  explicit LinearCompositionLearner(int number_of_iterations = 10)
      : data_set_weights_updater(new FakeDataSetWeightsUpdater<TElement>),
      linear_composition_scorer_weights_updater
        (new FakeLCScorerWeightsUpdater<TElement>) {
    number_of_iterations_ = number_of_iterations;
  }

  /**
   * clears parameters and sets int NUMBER_OF_ITERATIONS = 10
   */
  void setDefaultParameters() {
    number_of_iterations_ = 10;
  }
  /**
   * Checks if NUMBER_OF_ITERATIONS > 0 (should be true)
   */
  void checkParameters() const {
    CHECK(number_of_iterations_ > 0);
  }
  // void set_measure(typename Measure<TElement>::Ptr measure);
  // void set_weak_learner(typename Learner<TElement>::Ptr weak_learner);

  void setDataSetWeightsUpdater(
    typename DataSetWeightsUpdater<TElement>::Ptr
      in_data_set_weights_updater) {
    data_set_weights_updater = in_data_set_weights_updater;
  }
  void setLCScorerWeightsUpdater(
    typename LCScorerWeightsUpdater<TElement>::Ptr
      in_linear_composition_scorer_weights_updater) {
    linear_composition_scorer_weights_updater
      = in_linear_composition_scorer_weights_updater;
  }

  GET_SET(int, number_of_iterations);

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters) {
    number_of_iterations_ = parameters.Get<int>("NUMBER_OF_ITERATIONS");
  }

  typename LCScorerWeightsUpdater<TElement>::Ptr
    linear_composition_scorer_weights_updater;
  typename DataSetWeightsUpdater<TElement>::Ptr data_set_weights_updater;

  void learnImpl(const DataSet<TElement>& data,
                 LinearCompositionScorer* scorer);
  virtual string getDefaultAlias() const {return "LinearCompositionLearner";}

  int number_of_iterations_;
};


// template realizations
template <class TElement>
void LinearCompositionLearner<TElement>::learnImpl(
    const DataSet<TElement>& data, LinearCompositionScorer* scorer) {
  linear_composition_scorer_weights_updater->set_measure(this->measure_);
  data_set_weights_updater->set_measure(this->measure_);
  this->weak_learner_->set_measure(this->measure_);

  for (int iteration = 0;
      iteration < number_of_iterations_;
      ++iteration) {
    this->weak_learner_->reset();

    this->weak_learner_->learn(data);
    Scorer::Ptr current_scorer = this->weak_learner_->make();
    scorer->add(current_scorer, 1.0);
    linear_composition_scorer_weights_updater->updateWeights(data, scorer);
    data_set_weights_updater->updateWeights(&data, *scorer);
  }
}
};
};

#endif  // LTR_LEARNERS_LINEAR_COMPOSITION_LINEAR_COMPOSITION_LEARNER_H_
