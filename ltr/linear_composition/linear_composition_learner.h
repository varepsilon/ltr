// Copyright 2011 Yandex

#ifndef LTR_LINEAR_COMPOSITION_LINEAR_COMPOSITION_LEARNER_H_
#define LTR_LINEAR_COMPOSITION_LINEAR_COMPOSITION_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/linear_composition_scorer.h"
#include "ltr/learners/learner.h"
#include "ltr/linear_composition/data_set_weights_updater.h"
#include "ltr/linear_composition/linear_composition_scorer_weights_updater.h"

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/data_preprocessors/data_preprocessor_learner.h"

#include "ltr/feature_converters/fake_feature_converter_learner.h"
#include "ltr/data_preprocessors/fake_preprocessor_learner.h"


using ltr::Measure;
using ltr::DataSet;
using ltr::FeatureConverterArray;
using ltr::LinearCompositionScorer;
using ltr::Learner;
using ltr::lc::FakeDataSetWeightsUpdater;
using ltr::lc::FakeLCScorerWeightsUpdater;
using ltr::FakeFeatureConverterLearner;
using ltr::FakePreprocessorLearner;
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

   * FeatureConverterLearner - let weak learner call learn() on dataset with
   * converted features (e. g. RSM - random subspace method). Provides the
   * variability of weak scorers. By default is fake, so dataset is not pre-
   * feature converted before weak learning

   * DataPreprocessorLearner - let weak learner call learn() on preprocessed
   * dataset (e. g. begging). Provides the variability of weak scorers. By default
   * is fake, so dataset is not preprocessed before weak learning
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
        : feature_converter_learner(new FakeFeatureConverterLearner<TElement>),
        data_preprocessor_learner(new FakePreprocessorLearner<TElement>),
        data_set_weights_updater(new FakeDataSetWeightsUpdater<TElement>),
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
      this->addIntParameter("NUMBER_OF_ITERATIONS", 10);
    }
    /**
     * Checks if NUMBER_OF_ITERATIONS > 0 (should be true)
     */
    void checkParameters() const {
      CHECK_INT_PARAMETER("NUMBER_OF_ITERATIONS", X > 0);
    }
    // void setMeasure(typename Measure<TElement>::Ptr measure);
    // void setWeakLearner(typename BaseLearner<TElement>::Ptr weak_learner);
    void setFeatureConverterLearner(
        typename BaseFeatureConverterLearner<TElement>::Ptr
          in_feature_converter_learner) {
      feature_converter_learner = in_feature_converter_learner;
    }
    void setDataPreprocessorLearner(
      typename BaseDataPreprocessorLearner<TElement>::Ptr
        in_data_preprocessor_learner) {
      data_preprocessor_learner = in_data_preprocessor_learner;
    }
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
  private:
    LinearCompositionScorer scorer_;

    typename LCScorerWeightsUpdater<TElement>::Ptr
      linear_composition_scorer_weights_updater;
    typename DataSetWeightsUpdater<TElement>::Ptr data_set_weights_updater;

    typename BaseFeatureConverterLearner<TElement>::Ptr
      feature_converter_learner;
    typename BaseDataPreprocessorLearner<TElement>::Ptr
      data_preprocessor_learner;

    void learnImpl(const DataSet<TElement>& data);
  };


  // template realizations
  template <class TElement>
  void LinearCompositionLearner<TElement>::
      learnImpl(const DataSet<TElement>& data) {
    linear_composition_scorer_weights_updater->setMeasure(this->p_measure_);
    data_set_weights_updater->setMeasure(this->p_measure_);
    data_preprocessor_learner->setMeasure(this->p_measure_);
    this->p_weak_learner_->setMeasure(this->p_measure_);

    for (int iteration = 0;
        iteration < this->getIntParameter("NUMBER_OF_ITERATIONS");
        ++iteration) {
      this->p_weak_learner_->reset();
      DataSet<TElement> train_data, buf_data;

      data_preprocessor_learner->learn(data);
      typename DataPreprocessor<TElement>::Ptr data_preprocessor
        = data_preprocessor_learner->makePtr();
      data_preprocessor->apply(data, &buf_data);

      feature_converter_learner->learn(data);
      FeatureConverter::Ptr feature_converter
        = feature_converter_learner->makePtr();
      ltr::utility::ApplyFeatureConverter(feature_converter,
                                          buf_data, &train_data);

      this->p_weak_learner_->learn(train_data);
      Scorer::Ptr current_scorer = this->p_weak_learner_->makeScorerPtr();
      ltr::FeatureConverterArray in_scorer_converters;
      in_scorer_converters.push_back(feature_converter);
      current_scorer->setFeatureConverters(in_scorer_converters);
      scorer_.add(current_scorer, 1.0);

      linear_composition_scorer_weights_updater->updateWeights(data, &scorer_);
      data_set_weights_updater->updateWeights(&data, scorer_);
    }
  }
};
};

#endif  // LTR_LINEAR_COMPOSITION_LINEAR_COMPOSITION_LEARNER_H_
