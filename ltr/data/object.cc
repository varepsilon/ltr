// Copyright 2011 Yandex

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
    predicted_label_(utility::NaN) {}

Object::Object(const Object& object)
    :features_(new Features(*object.features_)),
    meta_info_(new MetaInfo(*object.meta_info_)),
    actual_label_(object.actual_label_),
    predicted_label_(object.predicted_label_) {}

Object::Object(const std::vector<Object>& objects)
    :features_(new Features(*objects[0].features_)),
    meta_info_(new MetaInfo(*objects[0].meta_info_)),
    actual_label_(objects[0].actual_label_),
    predicted_label_(objects[0].predicted_label_) {}

const Features& Object::features() const {
  return *features_;
}

Features& Object::features() {
  return *features_;
}

const MetaInfo& Object::metaInfo() const {
  return *meta_info_;
}

MetaInfo& Object::metaInfo() {
  return *meta_info_;
}

const string& Object::getMetaInfo(string name) const {
  if (meta_info_->find(name) == meta_info_->end())
    throw std::logic_error("unknown meta info " + name);
  return meta_info_->find(name)->second;
}

void Object::setMetaInfo(string name, string value) {
  meta_info_->operator[](name) = value;
}

Object& Object::operator<<(double feature) {
  features_->push_back(feature);
  return *this;
}

const Object& Object::operator[](size_t i) const {
  return at(i);
}
Object& Object::operator[](size_t i) {
  return at(i);
}
const Object& Object::at(const size_t i) const {
  return *this;
}
Object& Object::at(const size_t i) {
  return *this;
}

Object& Object::operator=(const Object& other)  {
  features_ = other.features_;
  meta_info_ = other.meta_info_;
  actual_label_ = other.actual_label_;
  predicted_label_ = other.predicted_label_;
  return *this;
}

size_t Object::featureCount() const {
  return (*features_).size();
}

double Object::actualLabel() const {
  return actual_label_;
}

double Object::predictedLabel() const {
  return predicted_label_;
}

void Object::setActualLabel(double actual_label) {
  actual_label_ = actual_label;
}

void Object::setPredictedLabel(double predicted_label) const {
  predicted_label_ = predicted_label;
}

Object Object::deepCopy() const {
  Object result = *this;
  result.features_.reset(new Features(*(this->features_)));
  result.meta_info_.reset(new map<string, string>(*(this->meta_info_)));
  return result;
}

size_t Object::size() const {
  return 1;
};

bool operator==(const Object& ob1, const Object& ob2) {
  return equalWithNaN(*ob1.features_, *ob2.features_) &&
         *ob1.meta_info_ == *ob2.meta_info_ &&
         utility::equalWithNaN(ob1.actualLabel(), ob2.actualLabel()) &&
         utility::equalWithNaN(ob1.predictedLabel(), ob2.predictedLabel());
}

bool operator!=(const Object& o1, const Object& o2) {
    return !(o1 == o2);
}
}
