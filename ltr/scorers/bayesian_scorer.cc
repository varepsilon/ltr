// Copyright 2012 Yandex

#include "ltr/scorers/bayesian_scorer.h"

using ltr::BayesianScorer;

namespace ltr {
double BayesianScorer::scoreImpl(const Object& object) const {
  double result = 0;
  double max_score = 0;
  for (LabelToPriorProbability::const_iterator label_iterator =
        prior_probability_.begin();
       label_iterator != prior_probability_.end();
       ++label_iterator) {
    double label = label_iterator->first;
    double probability = label_iterator->second;
    double current_score = log(probability) +
      estimator_->estimate(object, label);
    if (current_score > max_score) {
      max_score = current_score;
      result = label_iterator->first;
    }
  }
  return result;
}
};
