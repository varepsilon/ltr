// Copyright 2012 Yandex

#include <boost/lexical_cast.hpp>

#include "ltr/serialization_test/generator/generator_utility.h"

#include "ltr/measures/measure.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/learners/decision_tree/decision_tree_learner.h"
#include "ltr/learners/decision_tree/leaf_generator/most_common_label_leaf_generator.h"
#include "ltr/learners/decision_tree/split_criteria/data_size_stop_criteria.h"
#include "ltr/learners/decision_tree/split_criteria/same_label_stop_criteria.h"
#include "ltr/learners/decision_tree/id3_splitter.h"
#include "ltr/learners/decision_tree/oblivious_tree_splitter.h"
#include "ltr/measures/abs_error.h"
#include "ltr/learners/gp_learner/gp_learner.h"
#include "ltr/learners/composition_learner/composition_learner.h"
#include "ltr/feature_converters/feature_random_sampler_learner.h"
#include "ltr/learners/linear_learner/linear_learner.h"
#include "ltr/scorers/nearest_neighbor_scorer.h"
#include "ltr/learners/nearest_neighbor_learner/nearest_neighbor_learner.h"
#include "ltr/utility/neighbor_weighter.h"
#include "ltr/metrics/metric.h"
#include "ltr/metrics/euclidean_metric.h"
#include "ltr/predictions_aggregators/predictions_aggregator.h"
#include "ltr/predictions_aggregators/vote_predictions_aggregator.h"
#include "ltr/predictions_aggregators/sum_predictions_aggregator.h"
#include "ltr/scorers/composition_scorers/composition_scorer.h"


using ltr::BestFeatureLearner;
using ltr::AbsError;
using ltr::Measure;
using ltr::decision_tree::DecisionTreeLearner;
using ltr::decision_tree::MostCommonLabelLeafGenerator;
using ltr::decision_tree::DataSizeStopSplittingCriteria;
using ltr::decision_tree::SameLabelStopSplittingCriteria;
using ltr::decision_tree::ID3Splitter;
using ltr::decision_tree::ObliviousTreeSplitter;
using ltr::gp::GPLearner;
using ltr::composition::CompositionLearner;
using ltr::FeatureRandomSamplerLearner;
using ltr::LinearLearner;
using ltr::NNScorer;
using ltr::utility::NeighborWeighter;
using ltr::BaseMetric;
using ltr::NNLearner;
using ltr::SumPredictionsAggregator;
using ltr::composition::CompositionScorer;

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

  /*GPLearner<Object>::Ptr gp_learner(new GPLearner<Object>(abs_error));
  generator.setScorerTest(gp_learner, "GPLearner");*/

  DecisionTreeLearner::Ptr ID3 = new DecisionTreeLearner(
    new ID3Splitter, new MostCommonLabelLeafGenerator);
  ID3->addStopSplittingCriteria(new DataSizeStopSplittingCriteria);
  ID3->addStopSplittingCriteria(new SameLabelStopSplittingCriteria);
  generator.setScorerTest(ID3, "ID3");

  DecisionTreeLearner::Ptr ODT = new DecisionTreeLearner(
    new ObliviousTreeSplitter, new MostCommonLabelLeafGenerator);
  ID3->addStopSplittingCriteria(new DataSizeStopSplittingCriteria);
  ID3->addStopSplittingCriteria(new SameLabelStopSplittingCriteria);
  generator.setScorerTest(ODT, "ObliviousTree");

  LinearLearner<Object>::Ptr linear_learner(new LinearLearner<Object>);
  generator.setScorerTest(linear_learner, "LinearLearner");

  SumPredictionsAggregator::Ptr sum_aggregator(new SumPredictionsAggregator);
  CompositionScorer scorer(sum_aggregator);
  CompositionLearner<Object>::Ptr rsm_lc_learner(
    new CompositionLearner<Object>);
  rsm_lc_learner->setInitialScorer(scorer);
  FeatureRandomSamplerLearner<Object>::Ptr
    rsm(new FeatureRandomSamplerLearner<Object>);
  bf_learner->addFeatureConverterLearner(rsm);
  rsm_lc_learner->set_weak_learner(bf_learner);
  generator.setScorerTest(rsm_lc_learner, "RSMLCLearner");

  NNLearner<Object>::Ptr
    nn_learner(new NNLearner<Object>(new ltr::EuclideanMetric,
                                     new ltr::utility::InverseLinearDistance,
                                     new ltr::VotePredictionsAggregator,
                                     2));
  generator.setScorerTest(nn_learner, "KNNScorer");

  generator.write(argv[1]);
  return 0;
}
