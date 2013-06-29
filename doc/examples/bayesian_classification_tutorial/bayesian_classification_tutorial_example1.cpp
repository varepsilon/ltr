// Copyright 2012 Yandex

#include <iostream>  // NOLINT

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"
#include "ltr/data/data_set.h"
#include "ltr/density_estimators/base_probability_density_learner.h"
#include "ltr/density_estimators/fisher_discriminant_density_learner.h"
#include "ltr/density_estimators/fisher_discriminant.h"
#include "ltr/learners/bayesian_learner/bayesian_learner.h"
#include "ltr/scorers/bayesian_scorer.h"

using std::cout;
using std::endl;

using ltr::Object;
using ltr::DataSet;
using ltr::BayesianScorer;
using ltr::BayesianLearner;
using ltr::FisherDiscriminant;
using ltr::FisherDiscriminantLearner;
using ltr::FisherDiscriminantDensityLearner;
using ltr::BaseProbabilityDensityLearner;

int main() {
  Object object1;
  object1 << 0 << 0;
  object1.set_actual_label(1.0);

  Object object2;
  object2 << 10 << 0;
  object2.set_actual_label(0.0);

  Object object3;
  object3 << 12 << 0;
  object3.set_actual_label(0.0);

  DataSet<Object> data;
  data.add(object1);
  data.add(object2);
  data.add(object3);
  FisherDiscriminantLearner<Object>::Ptr learner(
      new FisherDiscriminantLearner<Object>);
  learner->learn(data);
  BayesianScorer::Ptr scorer = learner->makeSpecific();

  Object object4;
  object4 << 1 << 0;
  cout << scorer->score(object4) << endl;
}
