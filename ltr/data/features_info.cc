// Copyright 2011 Yandex

#include "ltr/data/features_info.h"

namespace ltr {

bool operator==(const FeatureInfo& left, const FeatureInfo& right) {
    return left.featureType_ == right.featureType_;
}

bool operator!=(const FeatureInfo& left, const FeatureInfo& right) {
    return !(left == right);
}
}
