// Copyright 2011 Yandex

#ifndef LTR_DATA_FEATURE_INFO_H_
#define LTR_DATA_FEATURE_INFO_H_

#include <boost/shared_ptr.hpp>

#include <vector>

namespace ltr {

enum FeatureType {NUMERIC, NOMINAL, BOOLEAN};

class FeatureInfo {
    public:
    typedef boost::shared_ptr<FeatureInfo> Ptr;

    FeatureInfo(size_t featureCount = 0, FeatureType type = NUMERIC)
        :featureType_(featureCount, type) {}

    size_t getFeatureCount() const {
        return featureType_.size();
    }
    void setFeatureCount(size_t featureCount, FeatureType type = NUMERIC) {
        featureType_.resize(featureCount, type);
    }

    FeatureType getFeatureType(size_t featureIdx) const {
        return featureType_[featureIdx];
    }
    void setFeatureType(size_t idx, FeatureType type) {
        featureType_[idx] = type;
    }

    friend bool operator==(const FeatureInfo& left, const FeatureInfo& right);

    private:
    std::vector<FeatureType> featureType_;
};

bool operator==(const FeatureInfo& left, const FeatureInfo& right);
bool operator!=(const FeatureInfo& left, const FeatureInfo& right);
}

#endif  // LTR_DATA_FEATURE_INFO_H_
