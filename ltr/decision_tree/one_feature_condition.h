// copyright 2012 Yandex

#ifndef LTR_DECISION_TREE_ONE_FEATURE_CONDITION_H_
#define LTR_DECISION_TREE_ONE_FEATURE_CONDITION_H_

#include "ltr/decision_tree/decision_tree.h"

namespace ltr {
namespace DecisionTree {

class OneFeatureCondition : public Condition {
  public:
    typedef boost::shared_ptr<OneFeatureCondition> Ptr;
    OneFeatureCondition() : feature_id_(0) {}
    explicit OneFeatureCondition(int feature_id) : feature_id_(feature_id) {}

    void setFeatureId(int feature_id);
  private:
    double valueImpl(const Object& obj) const;

    int feature_id_;
};

OneFeatureCondition::Ptr OneFeatureConditionPtr();
OneFeatureCondition::Ptr OneFeatureConditionPtr(int feature_id);
}
}

#endif  // LTR_DECISION_TREE_ONE_FEATURE_CONDITION_H_
