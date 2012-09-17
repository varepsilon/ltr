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
void Scorer::predict(const DataSet<TElement>& elements) const {
  for (int i = 0; i < elements.size(); ++i) {
    predict(elements[i]);
  }
}

template void Scorer::predict<Object>(const DataSet<Object>& elements) const;
template void Scorer::predict<ObjectPair>(const DataSet<ObjectPair>& elements) const;
template void Scorer::predict<ObjectList>(const DataSet<ObjectList>& elements) const;

template <class TElement>
void Scorer::predict(const TElement& element) const {
  PerObjectAccessor<const TElement> per_object_accessor(&element);
  for (int object_index = 0;
       object_index < (int)per_object_accessor.object_count();
       ++object_index) {
    per_object_accessor.object(object_index).set_predicted_label(
      score(per_object_accessor.object(object_index)));
  }
}

template void Scorer::predict<Object>(const Object& element) const;
template void Scorer::predict<ObjectPair>(const ObjectPair& element) const;
template void Scorer::predict<ObjectList>(const ObjectList& element) const;

string Scorer::generateLocalClassName(size_t index) const {
  return "Local" + boost::lexical_cast<std::string>(index) +
      "_" + this->alias();
}
}
