// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_H_

#include <boost/shared_ptr.hpp>

#include <vector>

#include "ltr/data/object.h"
#include "ltr/data/features_info.h"
#include "ltr/data/data_set.h"
#include "ltr/feature_converters/feature_converter.h"

namespace ltr {

class FeatureSubsetChooser : public FeatureConverter {
    public:
    typedef boost::shared_ptr<FeatureSubsetChooser> Ptr;

    explicit FeatureSubsetChooser(const vector<size_t>& choosedFeaturesIndexes)
        :choosedFeaturesIndexes_(choosedFeaturesIndexes) {}

    explicit FeatureSubsetChooser(const vector<bool>& choosedFeatures) {
        for (size_t featureIdx = 0;
                featureIdx < choosedFeatures.size();
                ++featureIdx) {
            if (choosedFeatures[featureIdx]) {
                choosedFeaturesIndexes_.push_back(featureIdx);
            }
        }
    }

    void
    setChoosedFeaturesIndexes(const vector<size_t>& choosedFeaturesIndexes) {
        choosedFeaturesIndexes_ = choosedFeaturesIndexes;
    }

    const vector<size_t>& getChoosedFeaturesIndexes() const {
        return choosedFeaturesIndexes_;
    }

    size_t countChoosedFeatures() const {
        return choosedFeaturesIndexes_.size();
    }

    FeatureInfo convertFeatureInfo(const FeatureInfo& oldFeatureInfo);

    void apply(const ltr::Object& source_object,
            ltr::Object* preprocessed_element);

    private:
    vector<size_t> choosedFeaturesIndexes_;
};
}
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_H_
