// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_COMPOSITION_LEARNER_COMPOSITION_LEARNER_H_
#define LTR_LEARNERS_COMPOSITION_LEARNER_COMPOSITION_LEARNER_H_

#include <rlog/rlog.h>

#include <string>
#include <functional>

#include "ltr/utility/shared_ptr.h"

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/composition_scorers/composition_scorer.h"
#include "ltr/learners/learner.h"
#include "ltr/learners/composition_learner/data_set_weights_updater.h"
#include "ltr/learners/composition_learner/composition_scorer_weights_updater.h"
#include "ltr/predictions_aggregators/predictions_aggregator.h"
#include "ltr/predictions_aggregators/sum_predictions_aggregator.h"

using std::string;

using ltr::Measure;
using ltr::DataSet;
using ltr::Learner;
using ltr::composition::CompositionScorer;
using ltr::composition::FakeDataSetWeightsUpdater;
using ltr::composition::FakeCompositionScorerWeightsUpdater;
using ltr::composition::CompositionScorerWeightsUpdater;
using ltr::composition::DataSetWeightsUpdater;
using ltr::PredictionsAggregator;
using ltr::SumPredictionsAggregator;

namespace ltr {
namespace composition {
/**
 * \brief Learner, wich uses other learner (weak learner), produces with it
 * different scorers and tie them into a composition scorer.
 * Has some extra parameters:

 * weak learner - learner to be used in producing scorers - elements
 * for composition scorer. Can hold a feature converter inside it
 * (for example, RSM)

 * CompositionScorerWeightsUpdater - a strategy for setting weights to
 * scorers in composition scorer. By default is fake, so every scorer in
 * composition has 1.0 weight

 * DataSetWeightsUpdater - a strategy for setting weights for dataset's
 * elements before each weak learner's learn() call. Provides the variability
 * of weak scorers. By default is fake, so dataset weights remain constant
 */
template <class TElement>
class CompositionLearner
    : public BaseLearner<TElement, CompositionScorer> {
 public:
  typedef ltr::utility::shared_ptr<CompositionLearner> Ptr;
  /**
   * @param parameters Standart LTR parameter container with int parameter
   * NUMBER_OF_ITERATIONS (number of scorers to be added into composition
   * scorer) and COMPOSITION_SCORER_PARAMETERS parameter of ParametersContainer
   * type. By default NUMBER_OF_ITERATIONS = 10 and COMPOSITION_SCORER_PARAMETERS
   * are the defaulf parameters of the composition scorer
   */
  explicit CompositionLearner(
      const ParametersContainer& parameters)
      : data_set_weights_updater_(new FakeDataSetWeightsUpdater<TElement>),
        composition_scorer_weights_updater_
        (new FakeCompositionScorerWeightsUpdater<TElement>) {
    this->setDefaultParameters();
    this->setParameters(parameters);
  }

  explicit CompositionLearner(
    int number_of_iterations = 10,
    typename DataSetWeightsUpdater<TElement>::Ptr
      data_set_weights_updater = new FakeDataSetWeightsUpdater<TElement>,
    typename CompositionScorerWeightsUpdater<TElement>::Ptr
      composition_scorer_weights_updater =
        (new FakeCompositionScorerWeightsUpdater<TElement>()))
    : data_set_weights_updater_(data_set_weights_updater),
      composition_scorer_weights_updater_(composition_scorer_weights_updater) {
    number_of_iterations_ = number_of_iterations;
    this->setDefaultScorer();
  }
  /**
   * Set default scorer
   */
  void setDefaultScorer() {
    SumPredictionsAggregator::Ptr aggregator(new SumPredictionsAggregator);
    CompositionScorer scorer(aggregator);
    this->setInitialScorer(scorer);
  }
  /**
   * Clears parameters and sets NUMBER_OF_ITERATIONS = 10
   */
  void setDefaultParameters() {
    this->setDefaultScorer();
    number_of_iterations_ = 10;
  }
  /**
   * Checks if NUMBER_OF_ITERATIONS > 0
   */
  void checkParameters() const {
    CHECK(number_of_iterations_ > 0); // NOLINT
  }

  /**
   * Sets the CompositionScorerWeightsUpdater parameter for the composition learner
   */
  void set_composition_scorer_weights_updater(
    typename CompositionScorerWeightsUpdater<TElement>::Ptr
      composition_scorer_weights_updater) {
    composition_scorer_weights_updater_
      = composition_scorer_weights_updater;
  }
  SET(typename DataSetWeightsUpdater<TElement>::Ptr,
    data_set_weights_updater);
  GET_SET(int, number_of_iterations);
  GET_SET(typename Learner<TElement>::Ptr, weak_learner);

 protected:
  typename Learner<TElement>::Ptr weak_learner_;

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters) {
    number_of_iterations_ = parameters.Get<int>("NUMBER_OF_ITERATIONS");
  }

  typename CompositionScorerWeightsUpdater<TElement>::Ptr
    composition_scorer_weights_updater_;
  typename DataSetWeightsUpdater<TElement>::Ptr data_set_weights_updater_;

  void learnImpl(const DataSet<TElement>& data,
                 CompositionScorer* scorer);
  virtual string getDefaultAlias() const {return "CompositionLearner";}

  int number_of_iterations_;
};

// template realizations
template <class TElement>
void CompositionLearner<TElement>::learnImpl(
    const DataSet<TElement>& data, CompositionScorer* scorer) {
  rInfo("Learning has been started");

  for (int element_index = 0; element_index < data.size(); ++element_index) {
    data.setWeight(element_index, 1.0 / data.size());
  }

  for (int iteration = 0;
      iteration < number_of_iterations_;
      ++iteration) {
    rInfo("Iteration #%d has been started", iteration);
    this->weak_learner_->reset();
    this->weak_learner_->learn(data);
    rDebug("Weak learner have been learned");
    Scorer::Ptr weak_scorer = this->weak_learner_->make();
    scorer->add(weak_scorer, 1.0);
    composition_scorer_weights_updater_->updateWeights(data, scorer);
    rDebug("Composition scorer's weights have been updated");
    data_set_weights_updater_->updateWeights(&data, *scorer);
    rDebug("Dataset's weights have been updated");
  }

  rInfo("Learning has been ended");
}
};
};

#endif  // LTR_LEARNERS_COMPOSITION_LEARNER_COMPOSITION_LEARNER_H_
