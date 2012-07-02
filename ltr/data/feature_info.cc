// Copyright 2011 Yandex

#include "ltr/data/feature_info.h"

namespace ltr {

bool operator==(const OneFeatureInfo& lhs, const OneFeatureInfo& rhs) {
  return lhs.type_ == rhs.type_ && lhs.values_ == rhs.values_;
}

bool operator==(const FeatureInfo& lhs, const FeatureInfo& rhs) {
    return lhs.feature_info_ == rhs.feature_info_;
}

bool operator!=(const FeatureInfo& lhs, const FeatureInfo& rhs) {
    return !(lhs == rhs);
}
}
