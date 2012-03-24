// Copyright 2012 Yandex

#include <boost/lexical_cast.hpp>

#include "serialization_test/generator/generator_utility.h"

#include "ltr/measures/measure.h"
#include "ltr/learners/best_feature_learner.h"
#include "ltr/measures/abs_error.h"

using ltr::BestFeatureLearner;
using ltr::AbsError;
using ltr::Measure;

using namespace serialization_test;

int main(int argc, char* argv[]) {
  Generator generator;

  Measure<Object>::Ptr abs_error(new AbsError());
  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>(abs_error));
  bf_learner->learn(generator.train_data);
  generator.setScorerTest(bf_learner, "bf_learner");

  generator.setMainCode();
  generator.write(argv[1]);
  return 0;
}
