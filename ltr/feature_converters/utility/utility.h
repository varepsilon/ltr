// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_UTILITY_UTILITY_H_
#define LTR_FEATURE_CONVERTERS_UTILITY_UTILITY_H_

#include <vector>

#include "ltr/feature_converters/feature_converter.h"

using std::vector;

namespace ltr {
namespace utility {

template <typename TElement>
void ApplyFeatureConverter(
    FeatureConverter::ConstPtr converter,
    const DataSet<TElement>& argument,
    DataSet<TElement>* value) {
  DataSet<TElement> result
      (converter->getNewFeatureInfo());

  for (size_t elementIdx = 0; elementIdx < argument.size(); ++elementIdx) {
    vector<Object> objectsInTElement;
    for (size_t objIdx = 0;
        objIdx < argument[elementIdx].size();
        ++objIdx) {
      Object objToAdd;
      converter->apply(argument[elementIdx][objIdx], &objToAdd);
      objectsInTElement.push_back(objToAdd);
    }
    TElement telementToAdd(objectsInTElement);
    result.add(telementToAdd, argument.getWeight(elementIdx));
  }
  *value = result;
}
}
}
#endif  // LTR_FEATURE_CONVERTERS_UTILITY_UTILITY_H_
