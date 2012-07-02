// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_ID3_LEARNER_H_
#define LTR_LEARNERS_DECISION_TREE_ID3_LEARNER_H_

#include "ltr/learners/decision_tree/decision_tree_learner.h"
#include "ltr/learners/decision_tree/id3_splitter.h"
#include "ltr/learners/decision_tree/sqr_error_quality.h"

using ltr::decision_tree::DecisionTreeLearner;
using ltr::decision_tree::SqrErrorQuality;
using ltr::decision_tree::ID3_Splitter;

using ltr::decision_tree::ConditionsLearner;
using ltr::decision_tree::SplittingQuality;

namespace ltr {
class ID3_Learner : public DecisionTreeLearner {
  public:
    explicit ID3_Learner(
      const ParametersContainer& parameters = ParametersContainer())
    : DecisionTreeLearner((parameters.Contains("") &&
                          parameters.TypeCoincides<ParametersContainer>("")) ?
                          parameters.Get<ParametersContainer>("") :
                          ParametersContainer()) {
      // : DecisionTreeLearner(parameters.getGroup(""))
      // {  //variant->any_TODO it is very ugly! fix it!
      if (parameters.Contains("") &&
          parameters.TypeCoincides<ParametersContainer>(""))
      {
        this->setConditionsLearner(
          ConditionsLearner::Ptr(
            new ID3_Splitter(
              parameters.Get<ParametersContainer>("conditions learner"))));
        this->setSplittingQuality(
          SplittingQuality::Ptr(
            new SqrErrorQuality(
              parameters.Get<ParametersContainer>("splitting quality"))));
      } else {
        this->setConditionsLearner(
          ConditionsLearner::Ptr(new ID3_Splitter));
        this->setSplittingQuality(
          SplittingQuality::Ptr(new SqrErrorQuality));
      }
      copyParameters(parameters);
    }
};
}

#endif  // LTR_LEARNERS_DECISION_TREE_ID3_LEARNER_H_
