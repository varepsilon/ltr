// Copyright 2012 Yandex

#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "ltr/data/object.h"

#include "ltr/data/utility/object_utility.h"

#include "ltr/utility/numerical.h"

#include "ltr/data/stand_alone_object_representation.h"
#include "ltr/data/data_set_object_representation.h"

using ltr::utility::DoubleEqualWithNaN;
using ltr::utility::NaN;

namespace ltr {

Object::Object(): presentation_(new StandAloneObjectRepresentation()) { }

Object::Object(const Object& object)
  : presentation_(new StandAloneObjectRepresentation()) {
  presentation_->deepCopyFrom(*object.presentation_);
}

Object::Object(InnerRepresentation::Ptr presentation,
               ElementBounds bounds)
               : presentation_(new DataSetObjectRepresentation(presentation,
                                                               bounds.start_line_index)) { }  // NOLINT

Object::Object(InnerRepresentation::Ptr presentation, int object_index)
               : presentation_(new DataSetObjectRepresentation(presentation,
                                                           object_index)) { }

const string& Object::getMetaInfo(const string& name) const {
  // TODO(wd28): think of refactoring to not to find each name 2 times
  MetaInfoPtr meta_info = presentation_->meta_info();
  if (meta_info->find(name) == meta_info->end())
    throw std::logic_error("unknown meta info " + name);
  return meta_info->find(name)->second;
}

void Object::setMetaInfo(const string& name, const string& value) {
  presentation_->meta_info()->operator[](name) = value;
}

Object& Object::operator<<(double feature) {
  presentation_->push_back(feature);
  return *this;
}

const double& Object::operator[](int feature_index) const {
  return at(feature_index);
}

double& Object::operator[](int feature_index) {
  return at(feature_index);
}

const double& Object::at(int feature_index) const {
  return presentation_->at(feature_index);
}

double& Object::at(int feature_index) {
  return presentation_->at(feature_index);
}

EigenColumn Object::eigen_features() {
  return presentation_->getEigenColumn();
}

const EigenColumn Object::eigen_features() const {
  return presentation_->getEigenColumn();
}

Object& Object::operator=(const Object& other) {
  presentation_->deepCopyFrom(*other.presentation_);
  return *this;
}

void Object::clear() {
  presentation_->clear();
}

int Object::feature_count() const {
  return presentation_->size();
}

void Object::set_feature_count(int feature_count) {
  presentation_->resize(feature_count);
}

double Object::actual_label() const {
  return presentation_->actual_label();
}

double Object::predicted_label() const {
  return presentation_->predicted_label();
}

void Object::set_actual_label(double actual_label) {
  presentation_->set_actual_label(actual_label);
}

void Object::set_predicted_label(double predicted_label) const {
  presentation_->set_predicted_label(predicted_label);
}

Object Object::deepCopy() const {
  Object to_return;
  to_return.presentation_->deepCopyFrom(*this->presentation_);
  return to_return;
}

void Object::lightCopyFrom(const Object& obj) {
  if (!presentation_->allowLightCopy())
    throw std::logic_error("Light copy is not allowed.");
  presentation_ = obj.presentation_;
}

void Object::set_eigen_features(const VectorXd& row) {
  presentation_->setEigenColumn(row);
}

string Object::toString() const {
  std::stringstream str;
  std::fixed(str);
  str.precision(2);
  str << '[';
  for (int feature_index = 0;
       feature_index < feature_count();
       ++feature_index) {
    if (feature_index != 0)
      str << ", ";
    str << at(feature_index);
  }
  str << ']';
  str << '{' << actual_label() << ", " << predicted_label() << '}';
  return str.str();
}

bool operator==(const Object& lhs, const Object& rhs) {
  if (lhs.feature_count() != rhs.feature_count())
    return false;

  return lhs.eigen_features().isApprox(rhs.eigen_features()) &&
         *lhs.presentation_->meta_info() == *rhs.presentation_->meta_info() &&
         utility::DoubleEqualWithNaN(lhs.actual_label(), rhs.actual_label()) &&
         utility::DoubleEqualWithNaN(lhs.predicted_label(),
                                     rhs.predicted_label());
}

bool operator!=(const Object& lhs, const Object& rhs) {
    return !(lhs == rhs);
}
};
