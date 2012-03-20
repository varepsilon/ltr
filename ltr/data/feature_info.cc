// Copyright 2011 Yandex

#include "ltr/data/feature_info.h"

namespace ltr {

bool operator==(const OneFeatureInfo& left, const OneFeatureInfo& right) {
  return left.type_ == right.type_ && left.values_ == right.values_;
}

bool operator==(const FeatureInfo& left, const FeatureInfo& right) {
    return left.features_info_ == right.features_info_;
}

bool operator!=(const FeatureInfo& left, const FeatureInfo& right) {
    return !(left == right);
}
}
