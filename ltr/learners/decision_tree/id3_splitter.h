// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_ID3_SPLITTER_H_
#define LTR_LEARNERS_DECISION_TREE_ID3_SPLITTER_H_

#include <boost/shared_ptr.hpp>

#include <vector>
#include <functional>

#include "ltr/learners/decision_tree/conditions_learner.h"

#include "logog/logog.h"

using std::vector;

namespace ltr {
namespace decision_tree {
/**
\class ID3_Splitter Implements ID3 splitting aplorithm.
For each nominal feature generates all possible
half-summs of nearest values splits.
For nominal features splits by value of the feature.
*/
class ID3_Splitter : public ConditionsLearner {
 public:
  typedef boost::shared_ptr<ID3_Splitter> Ptr;

  explicit ID3_Splitter(const ParametersContainer& parameters) {
    this->setParameters(parameters);
  }

  explicit ID3_Splitter(bool split_feature_n_times = false,
    int feature_split_count = 100,
    int half_summs_step = 5) {

    split_feature_n_times_ = split_feature_n_times;
    feature_split_count_ = feature_split_count;
    half_summs_step_ = half_summs_step;
  }

  int getNextConditions(vector<Condition::Ptr>* result);

  void setDefaultParameters() {
    split_feature_n_times_ = false;
    feature_split_count_ = 100;
    half_summs_step_ = 5;
  }

  void checkParameters() {
    CHECK(feature_split_count_ >= 1);
    CHECK(half_summs_step_ >= 1);
  }

  GET_SET(bool, split_feature_n_times);
  GET_SET(int, feature_split_count);
  GET_SET(int, half_summs_step);

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters) {
    split_feature_n_times_ = parameters.Get<bool>("SPLIT_FEATURE_N_TIMES");
    feature_split_count_ = parameters.Get<int>("FEATURE_SPLIT_COUNT");
    half_summs_step_ = parameters.Get<int>("HALF_SUMMS_STEP");
  }

  void init();

  int current_feature;
  int split_idx;
  bool split_feature_n_times_;
  int feature_split_count_;
  int half_summs_step_;
  vector<double> feature_values;
  vector<double> numeric_split_values;
};
}
}

#endif  // LTR_LEARNERS_DECISION_TREE_ID3_SPLITTER_H_
