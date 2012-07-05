// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_ID3_LEARNER_H_
#define LTR_LEARNERS_DECISION_TREE_ID3_LEARNER_H_

#include <string>

#include "ltr/learners/decision_tree/decision_tree_learner.h"
#include "ltr/learners/decision_tree/id3_splitter.h"
#include "ltr/learners/decision_tree/sqr_error_quality.h"

using ltr::decision_tree::DecisionTreeLearner;
using ltr::decision_tree::SqrErrorQuality;
using ltr::decision_tree::ID3_Splitter;

using ltr::decision_tree::ConditionsLearner;
using ltr::decision_tree::SplittingQuality;

using std::string;

namespace ltr {
class ID3_Learner : public DecisionTreeLearner {
 public:
  explicit ID3_Learner(const ParametersContainer& parameters)
  : DecisionTreeLearner(parameters) {
    this->set_conditions_learner(
      ConditionsLearner::Ptr(
        new ID3_Splitter(
          parameters.Get<ParametersContainer>("ID3SplitterParams"))));
    this->set_splitting_quality(
      SplittingQuality::Ptr(new SqrErrorQuality(
          parameters.Get<ParametersContainer>("SqrErrorQualityParams"))));
  }

  explicit ID3_Learner(bool split_feature_n_times = false,
    int feature_split_count = 100,
    int half_summs_step = 5,
    int min_vertex_size = 3,
    double label_eps = 0.001)
  : DecisionTreeLearner(min_vertex_size, label_eps) {
    this->set_conditions_learner(
      ConditionsLearner::Ptr(
        new ID3_Splitter(split_feature_n_times,
        feature_split_count,
        half_summs_step)));
    this->set_splitting_quality(
      SplittingQuality::Ptr(new SqrErrorQuality()));
  }
 private:
  virtual string getDefaultAlias() const {return "ID3_Learner";}
};
}

#endif  // LTR_LEARNERS_DECISION_TREE_ID3_LEARNER_H_