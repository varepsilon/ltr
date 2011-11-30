// Copyright 2011 Yandex

#include <sstream>

#include "ltr/data/object.h"
#include "ltr/data/utility/object_utility.h"
#include "ltr/utility/numerical.h"

namespace ltr {

Object::Object() : features_(new Features()),
        actual_label_(1.0),
        predicted_label_(utility::NaN) {}

const Features& Object::features() const {
    return *features_;
}
Features& Object::features() {
    return *features_;
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
    return result;
}

size_t Object::size() const {
    return 1;
};

bool operator==(const Object& o1, const Object& o2) {
    return
    utility::equalWithNaN(o1.actualLabel(), o2.actualLabel()) &&
    utility::equalWithNaN(o1.predictedLabel(), o2.predictedLabel()) &&
    (o1.features_.get() == o2.features_.get() ||
            equalWithNaN(*o1.features_, *o2.features_));
}

bool operator!=(const Object& o1, const Object& o2) {
    return !(o1 == o2);
}

std::ostream& operator<<(std::ostream& ostr, const Object& obj) {
    ostr << '(';
    for (size_t i = 0; i < obj.featureCount(); ++i) {
        if (i != 0) ostr << ',';
        ostr << obj[i];
    }
    ostr << ')';
    return ostr;
}
}
