// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_LINEAR_COMPOSITION_LINEAR_COMPOSITION_LEARNER_H_
#define LTR_LEARNERS_LINEAR_COMPOSITION_LINEAR_COMPOSITION_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <functional>

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/linear_composition_scorer.h"
#include "ltr/learners/learner.h"
#include "ltr/learners/linear_composition/data_set_weights_updater.h"
#include "ltr/learners/linear_composition/linear_composition_scorer_weights_updater.h"


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
   * Learner, wich uses other learner (weak learner), produce with it
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
      : public Learner<TElement, LinearCompositionScorer> {
  public:
    typedef boost::shared_ptr<LinearCompositionLearner> Ptr;

    /**
     * @param parameters Standart LTR parameter container with int parameter
     * NUMBER_OF_ITERATIONS (number of scorers to be added into linear composition
     * scorer). By default NUMBER_OF_ITERATIONS = 10
     */
    LinearCompositionLearner(
        const ParametersContainer& parameters = ParametersContainer())
        : data_set_weights_updater(new FakeDataSetWeightsUpdater<TElement>),
        linear_composition_scorer_weights_updater
          (new FakeLCScorerWeightsUpdater<TElement>),
        Learner<TElement, LinearCompositionScorer>("LinearCompositionLearner") {
      this->setDefaultParameters();
      this->copyParameters(parameters);
      this->checkParameters();
    }
    /**
     * Sets initial linear composition scorer
     */
    void setInitialScorer(const LinearCompositionScorer& in_scorer) {
      scorer_ = in_scorer;
    }
    LinearCompositionScorer makeImpl() const {
      return scorer_;
    }
    /**
     * Clears inner linear composition scorer into zero-length one.
     * Such a scorer is actually a FakeScorer (always returns 0.0)
     */
    void reset() {
      scorer_.clear();
    }
    /**
     * clears parameters and sets int NUMBER_OF_ITERATIONS = 10
     */
    void setDefaultParameters() {
      this->clearParameters();
      this->addNewParam("NUMBER_OF_ITERATIONS", 10);
    }
    /**
     * Checks if NUMBER_OF_ITERATIONS > 0 (should be true)
     */
    void checkParameters() const {
      Parameterized::checkParameter<int>("NUMBER_OF_ITERATIONS",
                                         std::bind2nd(std::greater<int>(), 0));
    }
    // void setMeasure(typename Measure<TElement>::Ptr measure);
    // void setWeakLearner(typename BaseLearner<TElement>::Ptr weak_learner);

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

    string toString() const {
      std::stringstream str;
      str << "Linear composition learner with parameter NUMBER_OF_ITERATIONS = "
        << this->parameters().Get<int>("NUMBER_OF_ITERATIONS");
      str << ", [" << p_weak_learner_->toString() << "] as weak learner, ["
        << data_set_weights_updater->toString()
        << "] as dataset weights updeter and ["
        << linear_composition_scorer_weights_updater->toString()
        << "] as composition scorer weights updater";
      return str.str();
    }
  private:
    LinearCompositionScorer scorer_;

    typename LCScorerWeightsUpdater<TElement>::Ptr
      linear_composition_scorer_weights_updater;
    typename DataSetWeightsUpdater<TElement>::Ptr data_set_weights_updater;

    void learnImpl(const DataSet<TElement>& data);
  };


  // template realizations
  template <class TElement>
  void LinearCompositionLearner<TElement>::
      learnImpl(const DataSet<TElement>& data) {
    if (this->p_measure_) {
      linear_composition_scorer_weights_updater->setMeasure(this->p_measure_);
      data_set_weights_updater->setMeasure(this->p_measure_);
      this->p_weak_learner_->setMeasure(this->p_measure_);
    }

    for (int iteration = 0;
        iteration < this->parameters().
         template Get<int>("NUMBER_OF_ITERATIONS");
        ++iteration) {
      this->p_weak_learner_->reset();

      this->p_weak_learner_->learn(data);
      Scorer::Ptr current_scorer = this->p_weak_learner_->makeScorerPtr();
      scorer_.add(current_scorer, 1.0);

      linear_composition_scorer_weights_updater->updateWeights(data, &scorer_);
      data_set_weights_updater->updateWeights(&data, scorer_);
    }
  }
};
};

#endif  // LTR_LEARNERS_LINEAR_COMPOSITION_LINEAR_COMPOSITION_LEARNER_H_
