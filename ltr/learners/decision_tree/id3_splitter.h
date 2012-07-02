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
  private:
    void init();

    int current_feature;
    int split_idx;
    vector<double> feature_values;
    vector<double> numeric_split_values;

  public:
    typedef boost::shared_ptr<ID3_Splitter> Ptr;

    explicit ID3_Splitter(
        const ParametersContainer& parameters = ParametersContainer()) {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }

    int getNextConditions(vector<Condition::Ptr>* result);

    void setDefaultParameters() {
      this->addNewParam("SPLIT_FEATURE_N_TIMES", false);
      this->addNewParam("FEATURE_SPLIT_COUNT", 100);
      this->addNewParam("HALF_SUMMS_STEP", 5);
    }
    void checkParameters() {
      this->checkParameter<int>("FEATURE_SPLIT_COUNT",
                                std::bind2nd(std::greater_equal<int>(), 1));
      this->checkParameter<int>("HALF_SUMMS_STEP",
                                std::bind2nd(std::greater_equal<int>(), 1));

      int n = getParameter<bool>("SPLIT_FEATURE_N_TIMES");
      n += getParameter<bool>("USE_HALF_SUMMS_SPLIT");
      if (n > 1)
        throw std::logic_error("you can use only one splitting type");
    }
};
}
}

#endif  // LTR_LEARNERS_DECISION_TREE_ID3_SPLITTER_H_
