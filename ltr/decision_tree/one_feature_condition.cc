// Copyright 2011 Yandex

#include "ltr/decision_tree/one_feature_condition.h"

namespace ltr {
namespace DecisionTree {
  double OneFeatureCondition::valueImpl(const Object& obj) const {
    return obj.features()[feature_id_];
  }

  OneFeatureCondition::Ptr OneFeatureConditionPtr() {
    return OneFeatureCondition::Ptr(new OneFeatureCondition());
  }

  OneFeatureCondition::Ptr OneFeatureConditionPtr(int feature_id) {
    return OneFeatureCondition::Ptr(new OneFeatureCondition(feature_id));
  }
}
}
