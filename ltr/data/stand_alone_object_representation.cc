// Copyright 2013 Yandex

#include "ltr/data/stand_alone_object_representation.h"
#include "ltr/utility/macros.h"

namespace ltr {

StandAloneObjectRepresentation::StandAloneObjectRepresentation(int size)
  : features_(size, 1), predicted_label_(0.0), actual_label_(0.0),
    meta_info_(new MetaInfo()) { }

StandAloneObjectRepresentation::~StandAloneObjectRepresentation() { }

MetaInfoPtr StandAloneObjectRepresentation::meta_info() {
  return meta_info_;
}

const MetaInfoPtr StandAloneObjectRepresentation::meta_info() const {
  return meta_info_;
}

void StandAloneObjectRepresentation::set_meta_info(const MetaInfo& meta_info) {
  meta_info_ = new MetaInfo(meta_info);
}

void StandAloneObjectRepresentation::push_back(double feature) {
  int current_size = size();
  resize(current_size + 1);
  features_(current_size, 0) = feature;
}

double& StandAloneObjectRepresentation::at(int index) {
  CHECK(index >= 0 && index < size());
  return features_(index, 0);
}

const double& StandAloneObjectRepresentation::at(int index) const {
  CHECK(index >= 0 || index < size());
  return features_(index, 0);
}

EigenColumn StandAloneObjectRepresentation::getEigenColumn() {
  return features_.col(0);
}

const EigenColumn StandAloneObjectRepresentation::getEigenColumn() const {
  return features_.col(0);
}

void StandAloneObjectRepresentation::setEigenColumn(const VectorXd& col) {
  resize(col.size());
  features_.col(0) = col;
}

void StandAloneObjectRepresentation::deepCopyFrom(const ObjectRepresentation& other) {  // NOLINT
  resize(other.size());
  features_.col(0) = other.getEigenColumn();
  set_actual_label(other.actual_label());
  set_predicted_label(other.predicted_label());
  set_meta_info(*other.meta_info());
}

void StandAloneObjectRepresentation::clear() {
  resize(0);
  meta_info_ = new MetaInfo();
}

int StandAloneObjectRepresentation::size() const {
  return features_.rows();
}

void StandAloneObjectRepresentation::resize(int new_size) {
  CHECK(new_size >= 0);
  // conservativeResize is a must in order to save information
  features_.conservativeResize(new_size, 1);
}

double StandAloneObjectRepresentation::actual_label() const {
  return actual_label_;
}

void StandAloneObjectRepresentation::set_actual_label(double label) {
  actual_label_ = label;
}

double StandAloneObjectRepresentation::predicted_label() const {
  return predicted_label_;
}

void StandAloneObjectRepresentation::set_predicted_label(double label) {
  predicted_label_ = label;
}

bool StandAloneObjectRepresentation::allowLightCopy() const {
  return true;
}
}
