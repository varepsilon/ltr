// Copyright 2012 Yandex
#ifndef LTR_CLIENT_REGISTRATION_H_
#define LTR_CLIENT_REGISTRATION_H_

#include <string>
#include <typeinfo>
#include "ltr/aggregators/aggregator.h"
#include "ltr/aggregators/average_aggregator.h"
#include "ltr/aggregators/vote_aggregator.h"
#include "ltr/aggregators/sum_aggregator.h"

#include "ltr/crossvalidation/splitter.h"
#include "ltr/crossvalidation/crossvalidator.h"
#include "ltr/crossvalidation/k_fold_simple_splitter.h"
#include "ltr/crossvalidation/leave_one_out_splitter.h"
#include "ltr/crossvalidation/tk_fold_simple_splitter.h"

#include "ltr/learners/learner.h"
#include "ltr/learners/bayesian_learner/bayesian_learner.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/learners/linear_learner/linear_learner.h"
#include "ltr/learners/nearest_neighbor_learner/nearest_neighbor_learner.h"
#include "ltr/learners/gp_learner/gp_learner.h"

#include "ltr/feature_converters/fake_feature_converter.h"
#include "ltr/feature_converters/fake_feature_converter_learner.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/feature_normalizer_learner.h"
#include "ltr/feature_converters/feature_random_sampler_learner.h"
#include "ltr/feature_converters/feature_sampler.h"
#include "ltr/feature_converters/feature_sampler_learner.h"
#include "ltr/feature_converters/nan_to_average_learner.h"
#include "ltr/feature_converters/nan_to_neutral_converter.h"
#include "ltr/feature_converters/nan_to_zero_learner.h"
#include "ltr/feature_converters/nominal_to_bool_converter.h"
#include "ltr/feature_converters/nominal_to_bool_learner.h"
#include "ltr/feature_converters/per_feature_linear_converter.h"
#include "ltr/feature_converters/remove_nominal_converter.h"
#include "ltr/feature_converters/remove_nominal_learner.h"

#include "ltr/data_preprocessors/data_preprocessor.h"
#include "ltr/data_preprocessors/data_random_sampler.h"
#include "ltr/data_preprocessors/data_sampler.h"
#include "ltr/data_preprocessors/fake_data_preprocessor.h"

#include "ltr/measures/abs_error.h"
#include "ltr/measures/accuracy.h"
#include "ltr/measures/average_precision.h"
#include "ltr/measures/binary_classification_accuracy.h"
#include "ltr/measures/measure.h"
#include "ltr/measures/dcg.h"
#include "ltr/measures/gmrr.h"
#include "ltr/measures/ndcg.h"
#include "ltr/measures/normalized_measure.h"
#include "ltr/measures/pfound.h"
#include "ltr/measures/reciprocal_rank.h"
#include "ltr/measures/squared_error.h"
#include "ltr/measures/true_point.h"

#include "ltr/metrics/metric.h"
#include "ltr/metrics/euclidean_metric.h"
#include "ltr/metrics/manhattan_metric.h"

#include "ltr/density_estimators/learners/base_probability_density_learner.h"
#include "ltr/density_estimators/learners/fisher_discriminant_learner.h"
#include "ltr/density_estimators/learners/normal_naive_bayes_learner.h"
#include "ltr/density_estimators/learners/quadratic_discriminant_learner.h"
#include "ltr/density_estimators/scorers/base_probability_density_estimator.h"
#include "ltr/density_estimators/scorers/fisher_discriminant.h"
#include "ltr/density_estimators/scorers/non_linear_discriminant.h"

#include "ltr_client/factory.h"
#include "ltr_client/ltr_client.h"
#include "ltr_client/configurator.h"

#include "ltr/utility/neighbor_weighter.h"

using std::string;

using ltr::Aggregator;
using ltr::AverageAggregator;
using ltr::AbsError;
using ltr::Accuracy;
using ltr::AveragePrecision;
using ltr::BayesianLearner;
using ltr::BestFeatureLearner;
using ltr::BinaryClassificationAccuracy;
using ltr::GMRR;
using ltr::NormalizedMeasure;
using ltr::PFound;
using ltr::ReciprocalRank;
using ltr::SquaredError;
using ltr::TruePoint;
using ltr::Parameterized;
using ltr::ParametersContainer;
using ltr::Learner;
using ltr::DataSet;
using ltr::Scorer;
using ltr::ObjectList;
using ltr::ObjectPair;
using ltr::Object;
using ltr::Log;
using ltr::LinearLearner;
using ltr::EuclideanMetric;
using ltr::ManhattanMetric;
using ltr::Measure;
using ltr::NNLearner;
using ltr::NDCG;
using ltr::DCG;
using ltr::SumAggregator;
using ltr::VoteAggregator;

using ltr::FeatureConverter;
using ltr::FakeFeatureConverter;
using ltr::FakeFeatureConverterLearner;
using ltr::FeatureConverterLearner;
using ltr::FeatureNormalizerLearner;
using ltr::FeatureRandomSamplerLearner;
using ltr::FeatureSampler;
using ltr::FeatureSamplerLearner;
using ltr::NanToAverageConverterLearner;
using ltr::NanToNeutralConverter;
using ltr::NanToZeroConverterLearner;
using ltr::NominalToBoolConverter;
using ltr::NominalToBoolConverterLearner;
using ltr::PerFeatureLinearConverter;
using ltr::RemoveNominalConverter;
using ltr::RemoveNominalConverterLearner;

using ltr::DataPreprocessor;
using ltr::DataRandomSampler;
using ltr::DataSampler;
using ltr::FakePreprocessor;

using ltr::cv::Splitter;
using ltr::cv::KFoldSimpleSplitter;
using ltr::cv::TKFoldSimpleSplitter;
using ltr::cv::LeaveOneOutSplitter;

using ltr::BaseProbabilityDensityLearner;
using ltr::FisherDiscriminantLearner;
using ltr::NormalNaiveBayesLearner;
using ltr::QuadraticDiscriminantLearner;
using ltr::BaseProbabilityDensityEstimator;
using ltr::FisherDiscriminant;
using ltr::NonLinearDiscriminant;

using ltr::gp::GPLearner;

using ltr::ListwiseMeasure;
using ltr::PointwiseMeasure;
using ltr::PairwiseMeasure;

using ltr::utility::InverseLinearDistance;
using ltr::utility::InverseSquareDistance;
using ltr::utility::InverseOrder;

string name_storage; // NOLINT

#define REGISTER(Base, Derived)\
        factory->registerType<Base, Derived>(#Derived);

#define REGISTER_LISTWISE(Base, Derived)\
        name_storage = #Derived;\
        factory->registerType<Base<ObjectList>, \
        Derived<ObjectList> >(name_storage + "listwise");

#define REGISTER_POINTWISE(Base, Derived)\
        name_storage = #Derived;\
        factory->registerType<Base<Object>, \
        Derived<Object> >(name_storage + "pointwise");

#define REGISTER_PAIRWISE(Base, Derived)\
        name_storage = #Derived;\
        factory->registerType<Base<ObjectPair>, \
        Derived<ObjectPair> >(name_storage + "pairwise");

#define REGISTER_EVERY_WISE(Base, Derived)\
        REGISTER_LISTWISE(Base, Derived)\
        REGISTER_POINTWISE(Base, Derived)\
        REGISTER_PAIRWISE(Base, Derived)


void RegisterAllTypes(Factory* factory) {
  REGISTER(Aggregator, AverageAggregator);
  REGISTER(Aggregator, SumAggregator);
  REGISTER(Aggregator, VoteAggregator);

  REGISTER_EVERY_WISE(Splitter, KFoldSimpleSplitter);
  REGISTER_EVERY_WISE(Splitter, TKFoldSimpleSplitter);
  REGISTER_EVERY_WISE(Splitter, LeaveOneOutSplitter);

  REGISTER_EVERY_WISE(Learner, BestFeatureLearner);
  REGISTER_EVERY_WISE(Learner, NNLearner);
  REGISTER_EVERY_WISE(Learner, GPLearner);
  REGISTER_POINTWISE(Learner, LinearLearner);

  REGISTER(ListwiseMeasure, NDCG);
  REGISTER(ListwiseMeasure, DCG);
  REGISTER(PointwiseMeasure, AbsError);
  REGISTER_POINTWISE(Measure, Accuracy);
  REGISTER_PAIRWISE(Measure, Accuracy);
  REGISTER(ListwiseMeasure, AveragePrecision);
  REGISTER(PointwiseMeasure, BinaryClassificationAccuracy); // NOLINT
  REGISTER(ListwiseMeasure, GMRR);
  REGISTER_EVERY_WISE(Measure, NormalizedMeasure);
  REGISTER(ListwiseMeasure, PFound);
  REGISTER(ListwiseMeasure, ReciprocalRank);
  REGISTER(PointwiseMeasure, SquaredError);
  REGISTER(PointwiseMeasure, TruePoint);

  REGISTER(BaseMetric, EuclideanMetric);
  REGISTER(BaseMetric, ManhattanMetric);

  REGISTER(NeighborWeighter, InverseLinearDistance);
  REGISTER(NeighborWeighter, InverseSquareDistance);
  REGISTER(NeighborWeighter, InverseOrder);

  REGISTER(FeatureConverter, FakeFeatureConverter);
  REGISTER(FeatureConverter, FeatureSampler);
  REGISTER(FeatureConverter, NanToNeutralConverter);
  REGISTER(FeatureConverter, NominalToBoolConverter);
  REGISTER(FeatureConverter, PerFeatureLinearConverter);
  REGISTER(FeatureConverter, RemoveNominalConverter);

  REGISTER_EVERY_WISE(FeatureConverterLearner, FakeFeatureConverterLearner);
  REGISTER_EVERY_WISE(FeatureConverterLearner, FeatureNormalizerLearner);
  REGISTER_EVERY_WISE(FeatureConverterLearner, FeatureSamplerLearner);
  REGISTER_POINTWISE(FeatureConverterLearner, NanToAverageConverterLearner);
  REGISTER_EVERY_WISE(FeatureConverterLearner, NanToZeroConverterLearner);
  REGISTER_EVERY_WISE(FeatureConverterLearner, NominalToBoolConverterLearner);
  REGISTER_EVERY_WISE(FeatureConverterLearner, RemoveNominalConverterLearner);

  REGISTER_EVERY_WISE(DataPreprocessor, DataRandomSampler);
  REGISTER_EVERY_WISE(DataPreprocessor, DataSampler);
  REGISTER_EVERY_WISE(DataPreprocessor, FakePreprocessor);

  REGISTER(BaseProbabilityDensityEstimator, FisherDiscriminant);
  REGISTER(BaseProbabilityDensityEstimator, NonLinearDiscriminant);
}

#endif  // LTR_CLIENT_REGISTRATION_H_
