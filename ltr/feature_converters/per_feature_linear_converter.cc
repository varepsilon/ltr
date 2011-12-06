// Copyright 2011 Yandex

#include <stdexcept>

#include "ltr/feature_converters/per_feature_linear_converter.h"

namespace ltr {

FeatureInfo PerFeatureLinearConverter::convertFeatureInfo(
    const FeatureInfo& oldFeatureInfo) const {
  this->checkFeatureCount(oldFeatureInfo.getFeatureCount());
  return oldFeatureInfo;
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

void PerFeatureLinearConverter::apply(const ltr::Object & argument,
    ltr::Object * value) const {
  this->checkFeatureCount(argument.featureCount());
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
    size_t checkedFeatureCount) const {
  if (checkedFeatureCount != this->featureCount()) {
    throw std::logic_error(
    "PerFeatureLinearConverter: wrong number of features in converted Object.");
  }
}
}
