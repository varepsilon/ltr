// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_COMPOSITION_LEARNER_ADA_BOOST_LEARNER_H_
#define LTR_LEARNERS_COMPOSITION_LEARNER_ADA_BOOST_LEARNER_H_

#include <string>
#include <functional>

#include <logog/logog.h>
#include "ltr/utility/shared_ptr.h"

#include "ltr/measures/binary_classification_accuracy.h"
#include "ltr/measures/binary_classification_margin.h"
#include "ltr/scorers/composition_scorers/linear_composition_scorer.h"
#include "ltr/learners/composition_learner/ada_boost_data_set_weights_updater.h"
#include "ltr/learners/composition_learner/ada_rank_composition_scorer_weights_updater.h"
#include "ltr/data/object.h"

using std::string;
//using ltr::Object;
//using ltr::composition::LinearCompositionScorer;

namespace ltr {
namespace composition {
class AdaBoostLearner
    : public CompositionLearner<Object, LinearCompositionScorer> {
 public:
  typedef ltr::utility::shared_ptr<AdaBoostLearner> Ptr;

  explicit AdaBoostLearner(
      const ParametersContainer& parameters)
      : CompositionLearner<Object, LinearCompositionScorer>(10,
          new AdaBoostDataSetWeightsUpdater<Object>(new BinaryClassificationMargin),
          new AdaRankScorerWeightsUpdater<Object, LinearCompositionScorer>(new BinaryClassificationAccuracy)) {
    this->setDefaultParameters();
    this->setParameters(parameters);
  }

  explicit AdaBoostLearner(
    int number_of_iterations = 10)
    : CompositionLearner<Object, LinearCompositionScorer>(number_of_iterations,
          new AdaBoostDataSetWeightsUpdater<Object>(new BinaryClassificationMargin),
          new AdaRankScorerWeightsUpdater<Object, LinearCompositionScorer>(new BinaryClassificationAccuracy)) {
  }

};
};
};

#endif  // LTR_LEARNERS_COMPOSITION_LEARNER_ADA_BOOST_LEARNER_H_
