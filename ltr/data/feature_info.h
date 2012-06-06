// Copyright 2011 Yandex

#ifndef LTR_DATA_FEATURE_INFO_H_
#define LTR_DATA_FEATURE_INFO_H_

#include <map>
#include <string>
#include <vector>

#include "ltr/interfaces/parameterized.h"
#include <boost/shared_ptr.hpp> //NOLINT

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
typedef map<size_t, string> NominalFeatureValues;

/** 
 * Structure represents info about one feature.
 */
struct OneFeatureInfo {
  /** 
    * Feature type
    */
  FeatureType type_;
  /** 
    * Possible feature values. Matters only for nominal features.
    */
  NominalFeatureValues values_;
  OneFeatureInfo(FeatureType type = NUMERIC,
                 NominalFeatureValues values = NominalFeatureValues()) {
    type_ = type;
    values_ = values;
  }
};

bool operator==(const OneFeatureInfo& lhs, const OneFeatureInfo& rhs);

/** \class Class is storing info about all features
 */
class FeatureInfo {
 public:
  /** \typedef Shared pointer to feature info.
   */
  typedef boost::shared_ptr<FeatureInfo> Ptr;
  /** Constructor, creates info about given count of features
   *  with given type
   */
  FeatureInfo(size_t feature_count = 0, FeatureType type = NUMERIC)
             : feature_info_(feature_count, type) {}
  /** Returns count of features
   */
  size_t get_feature_count() const {
      return feature_info_.size();
  }
  /** Changes number of features
   */
  void resize(size_t feature_count, FeatureType type = NUMERIC) {
      feature_info_.resize(feature_count, OneFeatureInfo(type));
  }
  /** Ands info about one feature
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
  NominalFeatureValues& getFeatureValues(size_t feature_index) {
    CHECK(feature_index < feature_info_.size() && feature_index >= 0);
    return feature_info_[feature_index].values_;
  }
  /** Returns possible values of feature with given index
   */
  const NominalFeatureValues& getFeatureValues(size_t feature_index) const {
    CHECK(feature_index < feature_info_.size() && feature_index >= 0);
    return feature_info_[feature_index].values_;
  }
  /** Returns type of feature with given index
   */
  FeatureType getFeatureType(size_t feature_index) const {
    CHECK(feature_index < feature_info_.size() && feature_index >= 0);
    return feature_info_[feature_index].type_;
  }
  /** Changes type of feature with given index
   */
  void setFeatureType(size_t feature_index, FeatureType type) {
    CHECK(feature_index < feature_info_.size() && feature_index >= 0);
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
}

#endif  // LTR_DATA_FEATURE_INFO_H_
