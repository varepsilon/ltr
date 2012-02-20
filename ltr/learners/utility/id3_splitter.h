// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_UTILITY_ID3_SPLITTER_H_
#define LTR_LEARNERS_UTILITY_ID3_SPLITTER_H_

#include <vector>

#include "ltr/learners/utility/conditions_learner.h"

namespace ltr {
namespace decision_tree {

class ID3_Splitter : public ConditionsLearner {
  private:
    void init();

    int current_feature;
    int split_idx;
    vector<double> feature_values;

  public:
    int getNextConditions(vector<Condition::Ptr>* result);
};
}
}

#endif  // LTR_LEARNERS_UTILITY_ID3_SPLITTER_H_
