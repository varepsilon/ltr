// Copyright 2012 Yandex

#include <boost/lexical_cast.hpp>

#include "ltr/serialization_test/generator/generator_utility.h"

#include "ltr/measures/measure.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/learners/decision_tree/id3_learner.h"
#include "ltr/measures/abs_error.h"
#include "ltr/learners/gp_learner/gp_learner.h"
#include "ltr/learners/composition_learner/composition_learner.h"
#include "ltr/scorers/composition_scorers/linear_composition_scorer.h"
#include "ltr/scorers/composition_scorers/max_weight_composition_scorer.h"
#include "ltr/scorers/composition_scorers/order_statistic_composition_scorer.h"
#include "ltr/scorers/composition_scorers/median_composition_scorer.h"
#include "ltr/feature_converters/feature_random_sampler_learner.h"
#include "ltr/learners/linear_learner/linear_learner.h"
#include "ltr/scorers/nearest_neighbor_scorer.h"
#include "ltr/learners/nearest_neighbor_learner/nearest_neighbor_learner.h"
#include "ltr/utility/neighbor_weighter.h"
#include "ltr/metrics/metric.h"
#include "ltr/metrics/euclidean_metric.h"
#include "ltr/aggregators/aggregator.h"
#include "ltr/aggregators/vote_aggregator.h"


using ltr::BestFeatureLearner;
using ltr::AbsError;
using ltr::Measure;
using ltr::ID3_Learner;
using ltr::gp::GPLearner;
using ltr::composition::CompositionLearner;
using ltr::composition::LinearCompositionScorer;
using ltr::composition::MaxWeightCompositionScorer;
using ltr::composition::OrderStatisticCompositionScorer;
using ltr::composition::MedianCompositionScorer;
using ltr::FeatureRandomSamplerLearner;
using ltr::LinearLearner;
using ltr::NNScorer;
using ltr::utility::NeighborWeighter;
using ltr::BaseMetric;
using ltr::NNLearner;

using serialization_test::Generator;

// It is highly recommended to use rule "one tested scorer - one learner"
// Otherwise name conflicts while feature converter's serialization may occur

int main(int argc, char* argv[]) {
  ltr::Log LOG("generator.log");
  Generator generator;

  Measure<Object>::Ptr abs_error(new AbsError());
  BestFeatureLearner<Object>::Ptr bf_learner(
      new BestFeatureLearner<Object>(abs_error));
  generator.setScorerTest(bf_learner, "BFLearner");

  GPLearner<Object>::Ptr gp_learner(new GPLearner<Object>(abs_error));
  generator.setScorerTest(gp_learner, "GPLearner");

  ID3_Learner::Ptr id3_learner(new ID3_Learner);
  generator.setScorerTest(id3_learner, "ID3Learner");

  LinearLearner<Object>::Ptr linear_learner(new LinearLearner<Object>);
  generator.setScorerTest(linear_learner, "LinearLearner");

  CompositionLearner<Object, LinearCompositionScorer>::Ptr rsm_lc_learner(
    new CompositionLearner<Object, LinearCompositionScorer>);
  FeatureRandomSamplerLearner<Object>::Ptr
    rsm(new FeatureRandomSamplerLearner<Object>);
  bf_learner->addFeatureConverterLearner(rsm);
  rsm_lc_learner->set_weak_learner(bf_learner);
  generator.setScorerTest(rsm_lc_learner, "RSMLCLearner");

  CompositionLearner<Object, MaxWeightCompositionScorer>::Ptr
    rsm_max_weight_composition_learner(
      new CompositionLearner<Object, MaxWeightCompositionScorer>);
  rsm_max_weight_composition_learner->set_weak_learner(bf_learner);
  generator.setScorerTest(rsm_max_weight_composition_learner, "RSMMaxWeightCompositionLearner");

  CompositionLearner<Object, OrderStatisticCompositionScorer>::Ptr
    rsm_order_statistic_composition_learner(
      new CompositionLearner<Object, OrderStatisticCompositionScorer>);
  rsm_order_statistic_composition_learner->set_weak_learner(bf_learner);
  generator.setScorerTest(rsm_order_statistic_composition_learner,
    "RSMOrderStatisticCompositionLearner");

  CompositionLearner<Object, MedianCompositionScorer>::Ptr
    rsm_median_composition_learner(new CompositionLearner<Object, MedianCompositionScorer>);
  rsm_median_composition_learner->set_weak_learner(bf_learner);
  generator.setScorerTest(rsm_median_composition_learner, "RSMMedianCompositionLearner");

  NNLearner<Object>::Ptr
    nn_learner(new NNLearner<Object>(new ltr::EuclideanMetric,
                                     new ltr::utility::InverseLinearDistance,
                                     new ltr::VoteAggregator,
                                     2));
  generator.setScorerTest(nn_learner, "KNNScorer");

  generator.write(argv[1]);
  return 0;
}
