// Copyright 2012 Yandex

#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "ltr/data/object.h"

#include "ltr/data/utility/object_utility.h"

#include "ltr/utility/numerical.h"

using ltr::utility::DoubleEqualWithNaN;
using ltr::utility::NaN;

namespace ltr {
Object::Object()
  : features_(new Features()),
    meta_info_(new MetaInfo()),
    actual_label_(0),
    predicted_label_(0) {}

Object::Object(const Object& object)
  : features_(object.features_),
    meta_info_(object.meta_info_),
    actual_label_(object.actual_label_),
    predicted_label_(object.predicted_label_) {}

const Features& Object::features() const {
  return *features_;
}

Features& Object::features() {
  return *features_;
}

const string& Object::getMetaInfo(const string& name) const {
  if (meta_info_->find(name) == meta_info_->end())
    throw std::logic_error("unknown meta info " + name);
  return meta_info_->find(name)->second;
}

void Object::setMetaInfo(const string& name, const string& value) {
  meta_info_->operator[](name) = value;
}

Object& Object::operator<<(double feature) {
  features_->push_back(feature);
  return *this;
}

const double& Object::operator[](int feature_index) const {
  return at(feature_index);
}

double& Object::operator[](int feature_index) {
  return at(feature_index);
}

const double& Object::at(int feature_index) const {
  if (feature_index < 0 || feature_index >= (int)features_->size()) {
    throw std::logic_error("Error: feature index out of bounds");
  }
  return features_->at(feature_index);
}

double& Object::at(int feature_index) {
  if (feature_index < 0 || feature_index >= (int)features_->size()) {
    throw std::logic_error("Error: feature index out of bounds");
  }
  return features_->at(feature_index);
}

Object& Object::operator=(const Object& other)  {
  features_ = other.features_;
  meta_info_ = other.meta_info_;
  actual_label_ = other.actual_label_;
  predicted_label_ = other.predicted_label_;
  return *this;
}

void Object::clear() {
  features_->clear();
}

int Object::feature_count() const {
  return (*features_).size();
}

double Object::actual_label() const {
  return actual_label_;
}

double Object::predicted_label() const {
  return predicted_label_;
}

void Object::set_actual_label(double actual_label) {
  actual_label_ = actual_label;
}

void Object::set_predicted_label(double predicted_label) const {
  predicted_label_ = predicted_label;
}

Object Object::deepCopy() const {
  Object result = *this;
  result.features_.reset(new Features(*(this->features_)));
  result.meta_info_.reset(new map<string, string>(*(this->meta_info_)));
  return result;
}

string Object::toString() const {
  std::stringstream str;
  std::fixed(str);
  str.precision(2);
  str << '[';
  for (int feature_index = 0;
       feature_index < (int)features().size();
       ++feature_index) {
    if (feature_index != 0)
      str << ", ";
    str << features()[feature_index];
  }
  str << ']';
  str << '{' << actual_label() << ", " << predicted_label() << '}';
  return str.str();
}

bool operator==(const Object& lhs, const Object& rhs) {
  return DoubleEqualWithNaN(*lhs.features_, *rhs.features_) &&
         *lhs.meta_info_ == *rhs.meta_info_ &&
         utility::DoubleEqualWithNaN(lhs.actual_label(), rhs.actual_label()) &&
         utility::DoubleEqualWithNaN(lhs.predicted_label(),
                                     rhs.predicted_label());
}

bool operator!=(const Object& lhs, const Object& rhs) {
    return !(lhs == rhs);
}
};
