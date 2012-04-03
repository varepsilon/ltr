// Copyright 2012 Yandex

#include <boost/lexical_cast.hpp>

#include "serialization_test/generator/generator_utility.h"

#include "ltr/measures/measure.h"
#include "ltr/learners/best_feature_learner.h"
#include "ltr/learners/id3_learner.h"
#include "ltr/measures/abs_error.h"
#include "ltr/learners/gp_learner.h"
#include "ltr/linear_composition/linear_composition_learner.h"
#include "ltr/feature_converters/RSM_feature_converter_learner.h"

using ltr::BestFeatureLearner;
using ltr::AbsError;
using ltr::Measure;
using ltr::ID3_Learner;
using ltr::gp::GPLearner;
using ltr::lc::LinearCompositionLearner;
using ltr::RSMFeatureConverterLearner;

using serialization_test::Generator;

// It is highly recommended to use rule "one tested scorer - one learner"
// Otherwise name conflicts while feature converter's serialization may occur

int main(int argc, char* argv[]) {
  Generator generator;

  Measure<Object>::Ptr abs_error(new AbsError());
  BestFeatureLearner<Object>::Ptr bf_learner(
      new BestFeatureLearner<Object>(abs_error));
  bf_learner->learn(generator.train_data);
  generator.setScorerTest(bf_learner, "bf_learner");

  GPLearner<Object>::Ptr gp_learner(new GPLearner<Object>(abs_error));
  gp_learner->learn(generator.train_data);
  generator.setScorerTest(gp_learner, "gp_learner");

  ID3_Learner::Ptr id3_learner(new ID3_Learner);
  id3_learner->learn(generator.train_data);
  generator.setScorerTest(id3_learner, "id3_learner");

  LinearCompositionLearner<Object>::Ptr simple_lc_learner(
    new LinearCompositionLearner<Object>); 
  simple_lc_learner->setMeasure(abs_error);
  simple_lc_learner->setWeakLearner(bf_learner);
  simple_lc_learner->learn(generator.train_data);
  generator.setScorerTest(simple_lc_learner, "simple_lc_learner");

  LinearCompositionLearner<Object>::Ptr rsm_lc_learner(
    new LinearCompositionLearner<Object>);
  rsm_lc_learner->setMeasure(abs_error);
  rsm_lc_learner->setWeakLearner(bf_learner);
  RSMFeatureConverterLearner<Object>::Ptr
    rsm(new RSMFeatureConverterLearner<Object>);
  rsm_lc_learner->setFeatureConverterLearner(rsm);
  rsm_lc_learner->learn(generator.train_data);
  generator.setScorerTest(rsm_lc_learner, "rsm_lc_learner");

  generator.setMainCode();
  generator.write(argv[1]);
  return 0;
}
