// Copyright 2012 Yandex

#ifndef LTR_DATA_FEATURE_INFO_H_
#define LTR_DATA_FEATURE_INFO_H_

#include <map>
#include <string>
#include <vector>

#include "ltr/utility/macros.h"

#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/utility/murmur_hash.h"

using std::map;
using std::string;
using std::vector;

using ltr::utility::hash;

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
                 NominalFeatureValues values = NominalFeatureValues(),
                 const string& name = "")
  : type_(type),
    values_(values),
    name_(name) {}
  explicit OneFeatureInfo(const vector<string>& values)
  : type_(NOMINAL) {
    for (int value_index = 0; value_index < values.size(); ++value_index) {
      values_[hash(values[value_index])] = values[value_index];
    }
  }
  /**
   * Feature type
   */
  FeatureType type_;
  /**
   * Possible feature values. Matters only for nominal features.
   */
  NominalFeatureValues values_;
  /**
   * Feature name
   */
  string name_;
};

typedef OneFeatureInfo LabelInfo;

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
  void addFeature(const OneFeatureInfo& info) {
    feature_info_.push_back(info);
  }
  /** Adds info about one feature
   */
  void addFeature(FeatureType type = NUMERIC,
                  const NominalFeatureValues& values = NominalFeatureValues(),
                  const string& name = "") {
    addFeature(OneFeatureInfo(type, values, name));
  }
  /** Getters and setters for feature values
   */
  void addNominalFeatureValue(int feature_index, const string& value) {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    CHECK(feature_info_[feature_index].type_ == NOMINAL);
    feature_info_[feature_index].values_[hash(value)] = value;
  }
  void addNominalFeatureValues(int feature_index,
                               const vector<string>& values) {
    for (int value_index = 0; value_index < values.size(); ++value_index) {
      addNominalFeatureValue(feature_index, values[value_index]);
    }
  }
  string getNominalFeatureValue(int feature_index, int value_hash) const {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    CHECK(feature_info_[feature_index].type_ == NOMINAL);
    CHECK(feature_info_[feature_index].values_.find(value_hash) !=
          feature_info_[feature_index].values_.end());
    return feature_info_[feature_index].values_.find(value_hash)->second;
  }
  string getNominalFeatureValue(int feature_index, double value_hash) const {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    CHECK(feature_info_[feature_index].type_ == NOMINAL);
    int int_value_hash = static_cast<int>(value_hash);
    CHECK(feature_info_[feature_index].values_.find(int_value_hash) !=
          feature_info_[feature_index].values_.end());
    return feature_info_[feature_index].values_.find(int_value_hash)->second;
  }
  const NominalFeatureValues& getNominalFeatureValues(int feature_index) const {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    CHECK(feature_info_[feature_index].type_ == NOMINAL);
    return feature_info_[feature_index].values_;
  }
  NominalFeatureValues& getNominalFeatureValues(int feature_index) {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    CHECK(feature_info_[feature_index].type_ == NOMINAL);
    return feature_info_[feature_index].values_;
  }
  void getNominalFeatureValues(int feature_index,
                               vector<string>* result) const {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    CHECK(feature_info_[feature_index].type_ == NOMINAL);
    const NominalFeatureValues& values_map =
        feature_info_[feature_index].values_;
    result->clear();
    for (NominalFeatureValues::const_iterator value_index = values_map.begin();
         value_index != values_map.end();
         ++value_index) {
      result->push_back(value_index->second);
    }
  }
  void clearNominalFeatureValues(int feature_index) {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    CHECK(feature_info_[feature_index].type_ == NOMINAL);
    feature_info_[feature_index].values_.clear();
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

  /** Returns name of feature with given index
   */
  string getFeatureName(int feature_index) const {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    return feature_info_[feature_index].name_;
  }
  /** Changes name of feature with given index
   */
  void setFeatureName(int feature_index, const string& name) {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    feature_info_[feature_index].name_ = name;
  }

  const OneFeatureInfo& operator[](int feature_index) const {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    return feature_info_[feature_index];
  }

  OneFeatureInfo& operator[](int feature_index) {
    CHECK(feature_index < (int)feature_info_.size() && feature_index >= 0);
    return feature_info_[feature_index];
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
