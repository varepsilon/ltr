// Copyright Yandex 2013

#include "ltr/data/data_set_object_representation.h"

namespace ltr {
DataSetObjectRepresentation::DataSetObjectRepresentation(
  InnerRepresentation::Ptr presentation, int index)
  : presentation_(presentation), index_(index) { }

DataSetObjectRepresentation::~DataSetObjectRepresentation() { }

MetaInfoPtr DataSetObjectRepresentation::meta_info() {
  return presentation_->meta_info(index_);
}

const MetaInfoPtr DataSetObjectRepresentation::meta_info() const {
  return presentation_->meta_info(index_);
}

void DataSetObjectRepresentation::set_meta_info(const MetaInfo& meta_info) {
  presentation_->set_meta_info(index_, new MetaInfo(meta_info));
}

void DataSetObjectRepresentation::push_back(double feature) {
  // we should resize matrix not via its objects's methods, but
  // via data set methods
  throw std::logic_error("Adding features isn\'t supported on matrices");
}

double& DataSetObjectRepresentation::at(int index) {
  return presentation_->feature(index, index_);
}

const double& DataSetObjectRepresentation::at(int index) const {
  return presentation_->feature(index, index_);
}

EigenColumn DataSetObjectRepresentation::getEigenColumn() {
  return presentation_->get_features_column(index_);
}

const EigenColumn DataSetObjectRepresentation::getEigenColumn() const {
  return presentation_->get_features_column(index_);
}

void DataSetObjectRepresentation::setEigenColumn(const VectorXd& column) {
  presentation_->set_features_column(index_, column);
}

void DataSetObjectRepresentation::deepCopyFrom(
  const ObjectRepresentation& other) {
  resize(other.size());
  presentation_->set_features_column(index_, other.getEigenColumn());
  set_actual_label(other.actual_label());
  set_predicted_label(other.predicted_label());
  set_meta_info(*other.meta_info());
}

void DataSetObjectRepresentation::clear() {
  throw std::logic_error("Clearing isn\'t supported on matrices");
}

int DataSetObjectRepresentation::size() const {
  return presentation_->row_count();
}

void DataSetObjectRepresentation::resize(int new_size) {
  if (new_size != size())
    throw std::logic_error("Resizing isn\'t supported on matrices");
}

double DataSetObjectRepresentation::actual_label() const {
  return presentation_->actual_labels()(index_);
}

void DataSetObjectRepresentation::set_actual_label(double label) {
  presentation_->set_actual_label(index_, label);
}

double DataSetObjectRepresentation::predicted_label() const {
  return presentation_->predicted_labels()(index_);
}

void DataSetObjectRepresentation::set_predicted_label(double label) {
  presentation_->set_predicted_label(index_, label);
}

bool DataSetObjectRepresentation::allowLightCopy() const {
  // Implemented in order to prevent
  // data_set[0].lightCopy(object);
  return false;
}
}
