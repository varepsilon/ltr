// Copyright 2012 Yandex

#include "ltr/utility/boost/lexical_cast.hpp"

#include "ltr/serialization_test/generator/generator_utility.h"

#include "ltr/measures/measure.h"
#include "ltr/measures/abs_error.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/learners/decision_tree/decision_tree_learner.h"
#include "ltr/learners/decision_tree/leaf_generator/most_common_label_leaf_generator.h"
#include "ltr/learners/decision_tree/split_criteria/data_size_stop_criteria.h"
#include "ltr/learners/decision_tree/split_criteria/same_label_stop_criteria.h"
#include "ltr/learners/decision_tree/id3_splitter.h"
#include "ltr/learners/decision_tree/oblivious_tree_splitter.h"
#include "ltr/learners/gp_learner/gp_learner.h"
#include "ltr/learners/composition_learner/composition_learner.h"
#include "ltr/learners/linear_learner/linear_learner.h"
#include "ltr/learners/bayesian_learner/bayesian_learner.h"
#include "ltr/learners/nearest_neighbor_learner/nearest_neighbor_learner.h"
#include "ltr/metrics/metric.h"
#include "ltr/metrics/euclidean_metric.h"
#include "ltr/predictions_aggregators/predictions_aggregator.h"
#include "ltr/predictions_aggregators/vote_predictions_aggregator.h"
#include "ltr/predictions_aggregators/sum_predictions_aggregator.h"
#include "ltr/scorers/composition_scorers/composition_scorer.h"
#include "ltr/scorers/nearest_neighbor_scorer.h"
#include "ltr/feature_converters/feature_random_sampler_learner.h"
#include "ltr/feature_converters/feature_sampler_learner.h"
#include "ltr/feature_converters/pca_learner.h"
#include "ltr/feature_converters/fake_feature_converter_learner.h"
#include "ltr/feature_converters/feature_normalizer_learner.h"
#include "ltr/feature_converters/nan_to_average_learner.h"
#include "ltr/feature_converters/nan_to_zero_learner.h"
#include "ltr/feature_converters/nominal_to_bool_learner.h"
#include "ltr/feature_converters/remove_nominal_learner.h"
#include "ltr/utility/neighbor_weighter.h"

using ltr::BayesianLearner;
using ltr::BaseProbabilityDensityLearner;
using ltr::BaseProbabilityDensityEstimator;
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
using ltr::LinearLearner;
using ltr::NNScorer;
using ltr::utility::NeighborWeighter;
using ltr::BaseMetric;
using ltr::NNLearner;
using ltr::FisherDiscriminantLearner;
using ltr::NormalNaiveBayesLearner;
using ltr::SumPredictionsAggregator;
using ltr::composition::CompositionScorer;
using ltr::PCALearner;
using ltr::FakeFeatureConverterLearner;
using ltr::FeatureNormalizerLearner;
using ltr::FeatureRandomSamplerLearner;
using ltr::FeatureSamplerLearner;
using ltr::NanToAverageConverterLearner;
using ltr::NanToZeroConverterLearner;
using ltr::NominalToBoolConverterLearner;
using ltr::RemoveNominalConverterLearner;

using serialization_test::Generator;

// It is highly recommended to use rule "one tested scorer - one learner"
// Otherwise name conflicts while feature converter's serialization may occur

int main(int argc, char* argv[]) {
  Generator generator;

  generator.addScorerTest(
      new BayesianLearner<Object, FisherDiscriminantDensityLearner<Object> >(),
      "BayesianLearner");

  Measure<Object>::Ptr abs_error(new AbsError());
  BestFeatureLearner<Object>::Ptr bf_learner(
      new BestFeatureLearner<Object>(abs_error));
  generator.addScorerTest(bf_learner, "BFLearner");

  GPLearner<Object>::Ptr gp_learner(new GPLearner<Object>(abs_error));
  generator.addScorerTest(gp_learner, "GPLearner");

  DecisionTreeLearner<Object>::Ptr ID3 = new DecisionTreeLearner<Object>(
    new ID3Splitter, new MostCommonLabelLeafGenerator);
  ID3->add_stop_splitting_criteria(new DataSizeStopSplittingCriteria(1));
  ID3->add_stop_splitting_criteria(new SameLabelStopSplittingCriteria);
  generator.addScorerTest(ID3, "ID3");

  DecisionTreeLearner<Object>::Ptr ODT = new DecisionTreeLearner<Object>(
    new ObliviousTreeSplitter, new MostCommonLabelLeafGenerator);
  ODT->add_stop_splitting_criteria(new DataSizeStopSplittingCriteria(1));
  ODT->add_stop_splitting_criteria(new SameLabelStopSplittingCriteria);
  generator.addScorerTest(ODT, "ObliviousTree");

  LinearLearner<Object>::Ptr linear_learner(new LinearLearner<Object>);
  generator.addScorerTest(linear_learner, "LinearLearner");

  NNLearner<Object>::Ptr
    nn_learner(new NNLearner<Object>(new ltr::EuclideanMetric,
                                     new ltr::utility::InverseLinearDistance,
                                     new ltr::VotePredictionsAggregator,
                                     2));
  generator.addScorerTest(nn_learner, "KNNScorer");

  FisherDiscriminantLearner<Object>::Ptr
    fisher_learner(new FisherDiscriminantLearner<Object>);
  generator.addScorerTest(fisher_learner, "FisherDiscriminant");

  NormalNaiveBayesLearner<Object>::Ptr
    naive_learner(new NormalNaiveBayesLearner<Object>);
  generator.addScorerTest(naive_learner, "NaiveBayes");

  generator.addFeatureConverterTest(new PCALearner<Object>, "PCALearner");

  generator.addFeatureConverterTest(new FakeFeatureConverterLearner<Object>,
                                    "FakeFeatureLearner");

  generator.addFeatureConverterTest(new FeatureNormalizerLearner<Object>,
                                    "FeatureNormalizerLearner");

  generator.addFeatureConverterTest(new FeatureRandomSamplerLearner<Object>,
                                    "FeatureRandomSamplerLearner");

  generator.addFeatureConverterTest(new FeatureSamplerLearner<Object>,
                                    "FeatureSamplerLearner");

  generator.addFeatureConverterTest(new NanToAverageConverterLearner<Object>,
                                    "NanToAverageConverterLearner");

  generator.addFeatureConverterTest(new NanToZeroConverterLearner<Object>,
                                    "NanToZeroConverterLearner");

  generator.addFeatureConverterTest(new NominalToBoolConverterLearner<Object>,
                                    "NominalToBoolConverterLearner");

  generator.addFeatureConverterTest(new RemoveNominalConverterLearner<Object>,
                                    "RemoveNominalConverterLearner");

  generator.write(argv[1]);
  return 0;
}
