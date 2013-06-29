// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_COMPOSITION_LEARNER_ADA_BOOST_LEARNER_H_
#define LTR_LEARNERS_COMPOSITION_LEARNER_ADA_BOOST_LEARNER_H_

#include <rlog/rlog.h>

#include <string>
#include <functional>

#include "ltr/utility/boost/shared_ptr.h"

#include "ltr/measures/binary_classification_accuracy.h"
#include "ltr/measures/binary_classification_margin.h"
#include "ltr/scorers/composition_scorers/composition_scorer.h"
#include "ltr/learners/composition_learner/ada_boost_data_set_weights_updater.h"
#include "ltr/learners/composition_learner/ada_rank_composition_scorer_weights_updater.h"
#include "ltr/predictions_aggregators/sum_predictions_aggregator.h"
#include "ltr/learners/composition_learner/composition_learner.h"

using std::string;
using ltr::composition::CompositionLearner;
using ltr::Object;

namespace ltr {
namespace composition {
class AdaBoostLearner
    : public CompositionLearner<Object> {
  ALLOW_SHARED_PTR_ONLY_CREATION(AdaBoostLearner)
 public:
  explicit AdaBoostLearner(
      const ParametersContainer& parameters)
      : CompositionLearner<Object>(10,
          new AdaBoostDataSetWeightsUpdater<Object>(
            new BinaryClassificationMargin),
          new AdaRankScorerWeightsUpdater<Object>(
            new BinaryClassificationAccuracy)) {
    this->setDefaultParameters();
    this->setParameters(parameters);
  }

  explicit AdaBoostLearner(
    int number_of_iterations = 10)
    : CompositionLearner<Object>(
        number_of_iterations,
        new AdaBoostDataSetWeightsUpdater<Object>(
          new BinaryClassificationMargin),
        new AdaRankScorerWeightsUpdater<Object>(
          new BinaryClassificationAccuracy)) {
  }
};
};
};

#endif  // LTR_LEARNERS_COMPOSITION_LEARNER_ADA_BOOST_LEARNER_H_
