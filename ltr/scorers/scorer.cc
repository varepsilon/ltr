// Copyright 2011 Yandex

#include "ltr/scorers/scorer.h"

namespace ltr {

double Scorer::score(const ltr::Object& object) const {
  Object sourceObject = object.deepCopy();
  Object convertedObject;
  for (size_t featureConverterIdx = 0;
      featureConverterIdx < feature_converters_.size();
      ++featureConverterIdx) {
    (*feature_converters_[featureConverterIdx]).apply(sourceObject,
        &convertedObject);
    sourceObject = convertedObject;
  }
  return scoreImpl(sourceObject);
}

template <class TElement>
void Scorer::markDataSet(const DataSet<TElement>& elements) {
  for (int i = 0; i < elements.size(); ++i) {
    markElement(elements[i]);
  }
}

template void Scorer::markDataSet<Object>(const DataSet<Object>& elements);
template void Scorer::markDataSet<ObjectPair>(const DataSet<ObjectPair>& elements);
template void Scorer::markDataSet<ObjectList>(const DataSet<ObjectList>& elements);

template <class TElement>
void Scorer::markElement(const TElement& element) {
  PerObjectAccessor<const TElement> poa(&element);
  for (int object_index = 0;
       object_index < (int)poa.object_count();
       ++object_index) {
    poa.object(object_index).set_predicted_label(
      score(poa.object(object_index)));
  }
}

template void Scorer::markElement<Object>(const Object& element);
template void Scorer::markElement<ObjectPair>(const ObjectPair& element);
template void Scorer::markElement<ObjectList>(const ObjectList& element);

string Scorer::generateLocalClassName(size_t index) {
  return "Local" + boost::lexical_cast<std::string>(index) +
      "_" + alias();
}

}
