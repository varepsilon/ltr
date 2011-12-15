// Copyright 2011 Yandex

#include "ltr/scorers/scorer.h"

namespace ltr {

double Scorer::score(const ltr::Object& obj) const {
  Object sourceObject = obj.deepCopy();
  Object convertedObject;
  for (size_t featureConverterIdx = 0;
      featureConverterIdx < featureConverters_.size();
      ++featureConverterIdx) {
    (*featureConverters_[featureConverterIdx]).apply(sourceObject,
        &convertedObject);
    sourceObject = convertedObject;
  }
  return scoreImpl(sourceObject);
}

double Scorer::operator()(const ltr::Object& obj) const {
  return this->score(obj);
}

std::string Scorer::generateCppCode(const std::string& class_name,
    int tabbing) const {
  return this->generateCppCodeImpl(class_name, tabbing);
}
}
