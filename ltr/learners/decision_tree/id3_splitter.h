// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_ID3_SPLITTER_H_
#define LTR_LEARNERS_DECISION_TREE_ID3_SPLITTER_H_

#include <boost/shared_ptr.hpp>

#include <vector>

#include "ltr/learners/decision_tree/conditions_learner.h"

#include "utility/logger.h"
using logger::LogStream;

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
    LogStream log;

  public:
    typedef boost::shared_ptr<ID3_Splitter> Ptr;

    explicit ID3_Splitter(
        const ParametersContainer& parameters = ParametersContainer())
        : log(logger::Logger::LL_INFO, "ID3_splitter ") {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }

    int getNextConditions(vector<Condition::Ptr>* result);

    void setDefaultParameters() {
      this->addBoolParameter("SPLIT_FEATURE_N_TIMES", false);
      this->addIntParameter("FEATURE_SPLIT_COUNT", 100);
      this->addIntParameter("HALF_SUMMS_STEP", 5);
    }
    void checkParameters() {
      CHECK_INT_PARAMETER("FEATURE_SPLIT_COUNT", X >= 1);
      CHECK_INT_PARAMETER("HALF_SUMMS_STEP", X >= 1);
      int n = this->getBoolParameter("SPLIT_FEATURE_N_TIMES");
      n += this->getBoolParameter("USE_HALF_SUMMS_SPLIT");
      if (n > 1)
        throw std::logic_error("you can use only one splitting type");
    }
};
}
}

#endif  // LTR_LEARNERS_DECISION_TREE_ID3_SPLITTER_H_
