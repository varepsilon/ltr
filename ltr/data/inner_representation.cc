// Copyright 2013 Yandex

#include "ltr/data/inner_representation.h"
#include "ltr/utility/macros.h"

namespace ltr {

InnerRepresentation::InnerRepresentation(int size, int features)
  : features_(features, size), predicted_labels_(size),
    actual_labels_(size), meta_info_() {
  for (int index = 0; index < size; ++index)
    meta_info_.push_back(new MetaInfo);
}

void InnerRepresentation::set_meta_info(int index, MetaInfoPtr ptr) {
  checkIndex(index);
  meta_info_[index] = ptr;
}

void InnerRepresentation::set_actual_label(int index, double value) {
  checkIndex(index);
  actual_labels_[index] = value;
}

void InnerRepresentation::set_predicted_label(int index, double value) {
  checkIndex(index);
  predicted_labels_[index] = value;
}

void InnerRepresentation::set_column_count(int columns) {
  resize(row_count(), columns);
}

EigenColumn InnerRepresentation::get_features_column(int index) {
  checkIndex(index);
  return features_.col(index);
}

void InnerRepresentation::set_features_column(int index, const VectorXd& row) {
  checkIndex(index);
  features_.col(index) = row;
}

const MatrixXd& InnerRepresentation::get_features_matrix() const {
  return features_;
}

void InnerRepresentation::set_row_count(int rows) {
  resize(rows, column_count());
}

void InnerRepresentation::resize(int rows, int cols) {
  CHECK(rows >= 0 && cols >= 0);
  features_.conservativeResize(rows, cols);
  predicted_labels_.conservativeResize(cols);
  actual_labels_.conservativeResize(cols);
  meta_info_.resize(cols);
}

MetaInfoPtr InnerRepresentation::meta_info(int index) const {
  checkIndex(index);
  return meta_info_[index];
}

const double& InnerRepresentation::feature(int row, int column) const {
  checkIndex(row, column);
  return features_(row, column);
}

double& InnerRepresentation::feature(int row, int column) {
  checkIndex(row, column);
  return features_(row, column);
}

void InnerRepresentation::add_several_objects(int num_objects) {
  CHECK(num_objects > 0);
  set_column_count(column_count() + num_objects);
}

void InnerRepresentation::checkIndex(int row, int column) const {
  CHECK(row >= 0 && row < row_count())
  CHECK(column >= 0 && column < column_count())
}

void InnerRepresentation::checkIndex(int index) const {
  CHECK(index >= 0 && index < column_count())
}

ElementBounds::ElementBounds(int start, int end): start_line_index(start),
                                                  end_line_index(end) { }
}
