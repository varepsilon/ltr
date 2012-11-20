// Copyright 2012 Yandex

#ifndef LTR_DATA_FEATURE_INFO_H_
#define LTR_DATA_FEATURE_INFO_H_

#include <map>
#include <string>
#include <vector>

#include "ltr/utility/macros.h"

#include "ltr/utility/boost/shared_ptr.h"

using std::map;
using std::string;
using std::vector;

namespace ltr {
/**
 * Types of features. Can be:
 * 1. Numeric - 42, 0.666, -3
 * 2. Nominal - "red", "green", "blue"
 * 3. Boolean - true(1), false(0)
 */
enum FeatureType {NUMERIC, NOMINAL, BOOLEAN};
/**
 * \typedef Map from numeric to string values for nominal features.
 */
typedef map<int, string> NominalFeatureValues;
/**
 * Structure represents info about one feature.
 */
struct OneFeatureInfo {
  OneFeatureInfo(FeatureType type = NUMERIC,
                 NominalFeatureValues values = NominalFeatureValues())
  : type_(type),
    values_(values) {}
  /**
   * Feature type
   */
  FeatureType type_;
  /**
   * Possible feature values. Matters only for nominal features.
   */
  NominalFeatureValues values_;
};

bool operator==(const OneFeatureInfo& lhs, const OneFeatureInfo& rhs);
/** \brief Class is storing info about all features
 */
class FeatureInfo {
 public:
  /** \typedef Shared pointer to feature info.
   */
  typedef ltr::utility::shared_ptr<FeatureInfo> Ptr;
  /** Constructor, creates info about given count of features
   *  with given type
   */
  FeatureInfo(int feature_count = 0, FeatureType type = NUMERIC)
             : feature_info_(feature_count, type) {}
  /** Returns count of features
   */
  int feature_count() const {
      return feature_info_.size();
  }
  /** Changes number of features
   */
  void resize(int feature_count, FeatureType type = NUMERIC) {
      feature_info_.resize(feature_count, OneFeatureInfo(type));
  }
  /** Adds info about one feature
   */
  void addFeature(OneFeatureInfo info) {
    feature_info_.push_back(info);
  }
  /** Adds info about one feature
   */
  void addFeature(FeatureType type = NUMERIC,
                  NominalFeatureValues values = NominalFeatureValues()) {
    addFeature(OneFeatureInfo(type, values));
  }
  /** Returns possible values of feature with given index
   */
  NominalFeatureValues& getFeatureValues(int feature_index) {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    return feature_info_[feature_index].values_;
  }
  /** Returns possible values of feature with given index
   */
  const NominalFeatureValues& getFeatureValues(int feature_index) const {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    return feature_info_[feature_index].values_;
  }
  /** Returns type of feature with given index
   */
  FeatureType getFeatureType(int feature_index) const {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    return feature_info_[feature_index].type_;
  }
  /** Changes type of feature with given index
   */
  void setFeatureType(int feature_index, FeatureType type) {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    feature_info_[feature_index].type_ = type;
  }

  void clear() {
    feature_info_.clear();
  }

  bool empty() {
    return feature_info_.empty();
  }

  friend bool operator==(const FeatureInfo& lhs, const FeatureInfo& rhs);

 private:
  vector<OneFeatureInfo> feature_info_;
};

bool operator==(const FeatureInfo& lhs, const FeatureInfo& rhs);

bool operator!=(const FeatureInfo& lhs, const FeatureInfo& rhs);
};
#endif  // LTR_DATA_FEATURE_INFO_H_
