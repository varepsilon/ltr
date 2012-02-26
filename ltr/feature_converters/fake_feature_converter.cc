// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>

#include <string>

#include "ltr/feature_converters/fake_feature_converter.h"

using std::string;

namespace ltr {

FeatureInfo FakeFeatureConverter::convertFeatureInfo(
    const FeatureInfo& oldFeatureInfo) const {
  return oldFeatureInfo;
}

void FakeFeatureConverter::apply(const Object& argument,
    Object* value) const {
  *value = argument;
}
}
