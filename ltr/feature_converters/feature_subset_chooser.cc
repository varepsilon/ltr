// Copyright 2011 Yandex

#include "ltr/feature_converters/feature_subset_chooser.h"

namespace ltr {

FeatureInfo
FeatureSubsetChooser::convertFeatureInfo(const FeatureInfo& oldFeatureInfo) {
    FeatureInfo convertedFeatureInfo(countChoosedFeatures());

    for (size_t choosedFeatureIdx = 0;
            choosedFeatureIdx < countChoosedFeatures();
            ++choosedFeatureIdx) {
        convertedFeatureInfo.setFeatureType(choosedFeatureIdx,
                oldFeatureInfo.getFeatureType(
                        choosedFeaturesIndexes_[choosedFeatureIdx]));
    }

    return convertedFeatureInfo;
}

void FeatureSubsetChooser::apply(const ltr::Object& argument,
        ltr::Object* value) {
    Object result;
    for (size_t choosedFeatureIdx = 0;
            choosedFeatureIdx < countChoosedFeatures();
            ++choosedFeatureIdx) {
        result <<
                argument.features()[choosedFeaturesIndexes_[choosedFeatureIdx]];
    }

    *value = result;
}
}
