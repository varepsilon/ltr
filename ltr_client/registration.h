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

using ltr::cv::Splitter;
using ltr::cv::KFoldSimpleSplitter;
using ltr::cv::TKFoldSimpleSplitter;
using ltr::cv::LeaveOneOutSplitter;

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
}

#endif  // LTR_CLIENT_REGISTRATION_H_
