// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_BAYESIAN_LEARNER_BAYESIAN_LEARNER_H_
#define LTR_LEARNERS_BAYESIAN_LEARNER_BAYESIAN_LEARNER_H_

#include <map>
#include <string>

#include "ltr/learners/learner.h"
#include "ltr/scorers/bayesian_scorer.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/density_estimators/learners/base_probability_density_learner.h"
#include "ltr/density_estimators/scorers/base_probability_density_estimator.h"
#include "ltr/utility/shared_ptr.h"
#include "ltr/utility/statistics_calculation.h"

using std::map;
using std::string;

using ltr::BaseProbabilityDensityLearner;
using ltr::BaseProbabilityDensityEstimator;
using ltr::ParametersContainer;
using ltr::utility::CalculateLabelsCapacity;
using ltr::Label;

namespace ltr {
template <class TElement, class TDensityLearner>
class BayesianLearner : public BaseLearner<TElement, BayesianScorer> {
 public:
  typedef ltr::utility::shared_ptr<BayesianLearner> Ptr;

  BayesianLearner() {
  }

 private:
  void learnImpl(const DataSet<TElement>& data, BayesianScorer* scorer);
  virtual string getDefaultAlias() const {return "BayesianLearner";}
  TDensityLearner density_learner_;
};

typedef map<Label, int> LabelToCapacity;
typedef map<Label, double> LabelToPriorProbability;

template <class TElement, class TDensityLearner>
void BayesianLearner<TElement, TDensityLearner>::learnImpl(
    const DataSet<TElement>& data,
    BayesianScorer* scorer) {
  LabelToCapacity LabelsCapacity = CalculateLabelsCapacity(data);
  LabelToPriorProbability prior_probability;
  for (map<double, int>::iterator capacity_iterator = LabelsCapacity.begin();
       capacity_iterator != LabelsCapacity.end();
       ++capacity_iterator) {
    prior_probability[capacity_iterator->first] =
      ((double)capacity_iterator->second) / data.size();
  }

  density_learner_.learn(data);
  *scorer = BayesianScorer(prior_probability, density_learner_.make());
}
};

#endif  // LTR_LEARNERS_BAYESIAN_LEARNER_BAYESIAN_LEARNER_H_
