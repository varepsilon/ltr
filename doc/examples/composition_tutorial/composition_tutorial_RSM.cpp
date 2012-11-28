#include <iostream>

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"
#include "ltr/learners/composition_learner/composition_learner.h"
#include "ltr/scorers/composition_scorers/composition_scorer.h"
#include "ltr/measures/abs_error.h"
#include "ltr/feature_converters/feature_random_sampler_learner.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"

using ltr::composition::CompositionLearner;
using ltr::composition::CompositionScorer;
using ltr::Object;
using ltr::AbsError;
using ltr::FeatureRandomSamplerLearner;
using ltr::BestFeatureLearner;

int main() {
  srand(14);

  DataSet<Object> dataset(3); // dataset with 3 features
  for (int i = 0; i < 20; ++i) { // 20 elements
    Object current;
    current << rand() << rand() << rand(); // random features
    current.set_actual_label(rand()); // random labels
    dataset << current;
  }

  // by default composition learner is linear
  CompositionLearner<Object> linear_composition_learner;

  AbsError::Ptr abs_error(new AbsError); // ::Ptr means a shared pointer wherever in LTR

  FeatureRandomSamplerLearner<Object>::Ptr rsm(new FeatureRandomSamplerLearner<Object>);
  BestFeatureLearner<Object>::Ptr best_feature_learner(new BestFeatureLearner<Object>(abs_error));
  best_feature_learner->add_feature_converter_learner(rsm);

  linear_composition_learner.set_weak_learner(best_feature_learner);

  linear_composition_learner.learn(dataset);
  CompositionScorer::Ptr linear_composition_scorer = linear_composition_learner.makeSpecific();

  std::cout << *linear_composition_scorer;
}
