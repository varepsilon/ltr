// Copyright 2011 Yandex

#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "ltr/data/object.h"
#include "ltr/data/utility/object_utility.h"
#include "ltr/utility/numerical.h"

using ltr::utility::equalWithNaN;

namespace ltr {

Object::Object() : features_(new Features()),
                   meta_info_(new MetaInfo()),
                   actual_label_(1.0),
                   predicted_label_(utility::NaN),
                   feature_info_(new FeatureInfo()) {}

Object::Object(const Object& object)
              : features_(object.features_),
                meta_info_(object.meta_info_),
                actual_label_(object.actual_label_),
                predicted_label_(object.predicted_label_),
                feature_info_(object.feature_info_) {}

Object::Object(const std::vector<Object>& objects)
              : features_(new Features(*objects[0].features_)),
                meta_info_(new MetaInfo(*objects[0].meta_info_)),
                actual_label_(objects[0].actual_label_),
                predicted_label_(objects[0].predicted_label_),
                feature_info_(objects[0].feature_info_) {}

Object::Object(const FeatureInfo& feature_info)
              : meta_info_(new MetaInfo()),
                actual_label_(1.0),
                predicted_label_(utility::NaN),
                feature_info_(new FeatureInfo(feature_info)),
                features_(new Features(feature_info.get_feature_count(),
                                       utility::NaN)) {}

const Features& Object::features() const {
  return *features_;
}

Features& Object::features() {
  return *features_;
}

const MetaInfo& Object::meta_info() const {
  return *meta_info_;
}

MetaInfo& Object::meta_info() {
  return *meta_info_;
}

const FeatureInfo& Object::feature_info() const {
  return *feature_info_;
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
  feature_info_->addFeature(NUMERIC);
  return *this;
}

const Object& Object::operator[](size_t i) const {
  return at(i);
}
Object& Object::operator[](size_t i) {
  return at(i);
}
const Object& Object::at(size_t i) const {
  if (i != 0) {
    throw std::logic_error("Error: object indexation with non-zero index");
  }
  return *this;
}
Object& Object::at(size_t i) {
  if (i != 0) {
    throw std::logic_error("Error: object indexation with non-zero index");
  }
  return *this;
}

Object& Object::operator=(const Object& other)  {
  features_ = other.features_;
  meta_info_ = other.meta_info_;
  actual_label_ = other.actual_label_;
  predicted_label_ = other.predicted_label_;
  feature_info_ = other.feature_info_;
  return *this;
}

size_t Object::feature_count() const {
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
  result.feature_info_.reset(new FeatureInfo(this->feature_info()));
  return result;
}

size_t Object::size() const {
  return 1;
};

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
  return str.str();
}

bool operator==(const Object& lhs, const Object& rhs) {
  return equalWithNaN(*lhs.features_, *rhs.features_) &&
         *lhs.meta_info_ == *rhs.meta_info_ &&
         *lhs.feature_info_ == *rhs.feature_info_ &&
         utility::equalWithNaN(lhs.actual_label(), rhs.actual_label()) &&
         utility::equalWithNaN(lhs.predicted_label(), rhs.predicted_label());
}

bool operator!=(const Object& lhs, const Object& rhs) {
    return !(lhs == rhs);
}
}
