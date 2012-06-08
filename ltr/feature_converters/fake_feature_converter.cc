// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>

#include <string>

#include "ltr/feature_converters/fake_feature_converter.h"

using std::string;

namespace ltr {
void FakeFeatureConverter::applyImpl(const Object& argument,
    Object* value) const {
  *value = argument.deepCopy();
}

void FakeFeatureConverter::fillOutputFeatureInfo() {
  output_feature_info_ = input_feature_info_;
}
}
