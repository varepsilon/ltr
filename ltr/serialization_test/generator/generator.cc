// Copyright 2012 Yandex

#include <boost/lexical_cast.hpp>

#include "ltr/serialization_test/generator/generator_utility.h"

#include "ltr/measures/measure.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/learners/decision_tree/id3_learner.h"
#include "ltr/measures/abs_error.h"
#include "ltr/learners/gp_learner/gp_learner.h"
#include "ltr/learners/linear_composition/linear_composition_learner.h"
#include "ltr/feature_converters/feature_random_sampler_learner.h"
#include "ltr/learners/linear_learner/linear_learner.h"

using ltr::BestFeatureLearner;
using ltr::AbsError;
using ltr::Measure;
using ltr::ID3_Learner;
using ltr::gp::GPLearner;
using ltr::lc::LinearCompositionLearner;
using ltr::FeatureRandomSamplerLearner;
using ltr::LinearLearner;

using serialization_test::Generator;

// It is highly recommended to use rule "one tested scorer - one learner"
// Otherwise name conflicts while feature converter's serialization may occur

int main(int argc, char* argv[]) {
  Generator generator;

  LOGOG_INITIALIZE();
  {
  logog::Cout out;

  Measure<Object>::Ptr abs_error(new AbsError());
  BestFeatureLearner<Object>::Ptr bf_learner(
      new BestFeatureLearner<Object>(abs_error));
  bf_learner->learn(generator.train_data);
  generator.setScorerTest(bf_learner, "BFLearner");

  GPLearner<Object>::Ptr gp_learner(new GPLearner<Object>(abs_error));
  gp_learner->learn(generator.train_data);
  generator.setScorerTest(gp_learner, "GPLearner");

  ID3_Learner::Ptr id3_learner(new ID3_Learner);
  id3_learner->learn(generator.train_data);
  generator.setScorerTest(id3_learner, "ID3Learner");

  LinearLearner<Object>::Ptr linear_learner(new LinearLearner<Object>);
  linear_learner->learn(generator.train_data);
  generator.setScorerTest(linear_learner, "LinearLearner");

  LinearCompositionLearner<Object>::Ptr simple_lc_learner(
    new LinearCompositionLearner<Object>);
  simple_lc_learner->setMeasure(abs_error);
  simple_lc_learner->setWeakLearner(bf_learner);
  simple_lc_learner->learn(generator.train_data);
  generator.setScorerTest(simple_lc_learner, "SimpleLCLearner");

  LinearCompositionLearner<Object>::Ptr rsm_lc_learner(
    new LinearCompositionLearner<Object>);
  rsm_lc_learner->setMeasure(abs_error);
  FeatureRandomSamplerLearner<Object>::Ptr
    rsm(new FeatureRandomSamplerLearner<Object>);
  bf_learner->addFeatureConverterLearner(rsm);
  rsm_lc_learner->setWeakLearner(bf_learner);
  rsm_lc_learner->learn(generator.train_data);
  generator.setScorerTest(rsm_lc_learner, "RSMLCLearner");

  }
  LOGOG_SHUTDOWN();

  generator.write(argv[1]);
  return 0;
}
