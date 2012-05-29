// Copyright 2011 Yandex

#include <stdexcept>

#include "ltr/feature_converters/per_feature_linear_converter.h"

using std::logic_error;

namespace ltr {

FeatureInfo PerFeatureLinearConverter::getNewFeatureInfo() const {
  this->checkFeatureCount(feature_info_.get_feature_count());
  return feature_info_;
}

void PerFeatureLinearConverter::resize(size_t feature_count) {
  coefficient_.resize(feature_count);
  shift_.resize(feature_count);
}

double PerFeatureLinearConverter::getCoefficient(size_t feature_idx) const {
  return coefficient_[feature_idx];
}

void PerFeatureLinearConverter::setCoefficient(size_t feature_idx,
    double coefficient) {
  coefficient_[feature_idx] = coefficient;
}

double PerFeatureLinearConverter::getShift(size_t feature_idx) const {
  return shift_[feature_idx];
}

void PerFeatureLinearConverter::setShift(size_t feature_idx, double shift) {
  shift_[feature_idx] = shift;
}

void PerFeatureLinearConverter::applyImpl(const Object& argument,
    Object* value) const {
  this->checkFeatureCount(argument.feature_count());
  *value = argument.deepCopy();
  for (size_t feature_idx = 0;
      feature_idx < value->features().size();
      ++feature_idx) {
    value->features()[feature_idx] *= coefficient_[feature_idx];
    value->features()[feature_idx] += shift_[feature_idx];
  }
}

size_t PerFeatureLinearConverter::featureCount() const {
  return coefficient_.size();
}

void PerFeatureLinearConverter::checkFeatureCount(
    size_t checked_feature_count) const {
  if (checked_feature_count != this->featureCount()) {
    throw logic_error(
    "PerFeatureLinearConverter: wrong number of features in converted Object.");
  }
}
}
