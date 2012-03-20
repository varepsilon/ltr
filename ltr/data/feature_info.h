// Copyright 2011 Yandex

#ifndef LTR_DATA_FEATURE_INFO_H_
#define LTR_DATA_FEATURE_INFO_H_

#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>
#include <map>

using std::map;
using std::string;

namespace ltr {

enum FeatureType {NUMERIC, NOMINAL, BOOLEAN};
typedef map<int, string> NominalFeatureValues;

struct OneFeatureInfo {
  FeatureType type_;
  NominalFeatureValues values_;
  OneFeatureInfo(FeatureType type = NUMERIC,
                 NominalFeatureValues values = NominalFeatureValues()) {
    type_ = type;
    values_ = values;
  }
};

bool operator==(const OneFeatureInfo& left, const OneFeatureInfo& right);

class FeatureInfo {
    public:
    typedef boost::shared_ptr<FeatureInfo> Ptr;

    FeatureInfo(size_t featureCount = 0, FeatureType type = NUMERIC)
        :features_info_(featureCount, type) {}

    size_t getFeatureCount() const {
        return features_info_.size();
    }
    void setFeatureCount(size_t featureCount, FeatureType type = NUMERIC) {
        features_info_.resize(featureCount, OneFeatureInfo(type));
    }
    void addFeature(OneFeatureInfo info) {
      features_info_.push_back(info);
    }
    void addFeature(FeatureType type = NUMERIC,
                    NominalFeatureValues values = NominalFeatureValues()) {
      addFeature(OneFeatureInfo(type, values));
    }

    NominalFeatureValues& getFeatureValues(size_t idx) {
      return features_info_[idx].values_;
    }

    const NominalFeatureValues& getFeatureValues(size_t idx) const {
      return features_info_[idx].values_;
    }

    FeatureType getFeatureType(size_t featureIdx) const {
        return features_info_[featureIdx].type_;
    }
    void setFeatureType(size_t idx, FeatureType type) {
        features_info_[idx].type_ = type;
    }

    friend bool operator==(const FeatureInfo& left, const FeatureInfo& right);

    private:
    std::vector<OneFeatureInfo> features_info_;
};

bool operator==(const FeatureInfo& left, const FeatureInfo& right);
bool operator!=(const FeatureInfo& left, const FeatureInfo& right);
}

#endif  // LTR_DATA_FEATURE_INFO_H_
