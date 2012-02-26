// Copyright 2011 Yandex

#ifndef LTR_LINEAR_COMPOSITION_LINEAR_COMPOSITION_LEARNER_H_
#define LTR_LINEAR_COMPOSITION_LINEAR_COMPOSITION_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/linear_composition_scorer.h"
#include "ltr/learners/learner.h"
#include "ltr/linear_composition/ada_rank_data_set_weights_updater.h"
#include "ltr/linear_composition/ada_rank_lc_scorer_weights_updater.h"

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


namespace ltr {
namespace lc {
  template <class TElement,
    class TLCSWeightsUpdater = FakeLCScorerWeightsUpdater<TElement>,
    class TDSWeightsUpdater = FakeDataSetWeightsUpdater<TElement> >
  class LinearCompositionLearner
      : public Learner<TElement, LinearCompositionScorer> {
  public:
    typedef boost::shared_ptr<LinearCompositionLearner> Ptr;

    LinearCompositionLearner(
        const ParametersContainer& parameters = ParametersContainer())
        : feature_converter_learner(new FakeFeatureConverterLearner<TElement>),
        data_preprocessor_learner(new FakePreprocessorLearner<TElement>),
        Learner("LinearCompositionLearner") {
      this->setDefaultParameters();
      this->copyParameters(parameters);
      this->checkParameters();
    }

    void setInitialScorer(const LinearCompositionScorer& in_scorer) {
      scorer_ = in_scorer;
    }
    LinearCompositionScorer make() const {
      return scorer_;
    }
    void reset() {
      scorer_.clear();
    }
    void setDefaultParameters() {
      this->clearParameters();
      this->addIntParameter("NUMBER_OF_ITERATIONS", 10);
    }
    void checkParameters() const {
      CHECK_INT_PARAMETER("NUMBER_OF_ITERATIONS", X > 0);
    }
    // void setMeasure(typename Measure<TElement>::Ptr measure);
    // void setWeakLearner(typename BaseLearner<TElement>::Ptr weak_learner);
    void setFeatureConverterLearner(
        typename IFeatureConverterLearner<TElement>::Ptr
          in_feature_converter_learner) {
      feature_converter_learner = in_feature_converter_learner;
    }
    void setDataPreprocessorLearner(
      typename IDataPreprocessorLearner<TElement>::Ptr
        in_data_preprocessor_learner) {
      data_preprocessor_learner = in_data_preprocessor_learner;
    }
  private:
    LinearCompositionScorer scorer_;
    TLCSWeightsUpdater linear_composition_scorer_weights_updater_;
    TDSWeightsUpdater data_set_weights_updater_;

    typename IFeatureConverterLearner<TElement>::Ptr feature_converter_learner;
    typename IDataPreprocessorLearner<TElement>::Ptr data_preprocessor_learner;

    void learnImpl(const DataSet<TElement>& data);
  };


  // template realizations
  template <class TElement, class TLCSWeightsUpdater, class TDSWeightsUpdater>
  void LinearCompositionLearner<TElement,
      TLCSWeightsUpdater, TDSWeightsUpdater>::
        learnImpl(const DataSet<TElement>& data) {
    linear_composition_scorer_weights_updater_.setMeasure(this->p_measure_);
    data_set_weights_updater_.setMeasure(this->p_measure_);
    this->p_weak_learner_->setMeasure(this->p_measure_);

    for (int iteration = 0;
        iteration < this->getIntParameter("NUMBER_OF_ITERATIONS");
        ++iteration) {
      this->p_weak_learner_->reset();
      DataSet<TElement> train_data, buf_data;

      data_preprocessor_learner->learn(data);
      typename DataPreprocessor<TElement>::Ptr data_preprocessor
        = data_preprocessor_learner->make();
      data_preprocessor->apply(data, &buf_data);

      feature_converter_learner->learn(data);
      FeatureConverter::Ptr feature_converter
        = feature_converter_learner->make();
      feature_converter->apply(buf_data, &train_data);

      this->p_weak_learner_->learn(train_data);
      Scorer::Ptr current_scorer = this->p_weak_learner_->makeScorerPtr();
      ltr::FeatureConverterArray in_scorer_converters;
      in_scorer_converters.push_back(feature_converter);
      current_scorer->setFeatureConverters(in_scorer_converters);
      scorer_.add(current_scorer, 1.0);

      linear_composition_scorer_weights_updater_.updateWeights(data, &scorer_);
      data_set_weights_updater_.updateWeights(&data, scorer_);
    }
  }
};
};

#endif  // LTR_LINEAR_COMPOSITION_LINEAR_COMPOSITION_LEARNER_H_
