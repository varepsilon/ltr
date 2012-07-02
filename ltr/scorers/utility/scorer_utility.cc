// Copyright 2011 Yandex School Practice

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"
#include "ltr/scorers/utility/scorer_utility.h"

namespace ltr {
namespace utility {

template <class TElement>
void MarkDataSet(const DataSet<TElement>& elements, const Scorer& scorer) {
  for (int i = 0; i < elements.size(); ++i) {
    MarkElement(elements[i], scorer);
  }
}

template void MarkDataSet< Object >(const DataSet<Object>& elements,
    const Scorer& scorer);
template void MarkDataSet< ObjectPair >(const DataSet<ObjectPair>& elements,
    const Scorer& scorer);
template void MarkDataSet< ObjectList >(const DataSet<ObjectList>& elements,
    const Scorer& scorer);

template <class TElement>
void MarkElement(const TElement& element, const Scorer& scorer) {
  for (int i = 0; i < element.size(); ++i) {
    element[i].set_predicted_label(scorer(element[i]));
  }
}

template void MarkElement<Object>(const Object& element, const Scorer& scorer);
template void MarkElement<ObjectPair>(const ObjectPair& element,
    const Scorer& scorer);
template void MarkElement<ObjectList>(const ObjectList& element,
    const Scorer& scorer);

std::string GenerateLocalClassName(const Scorer& scorer, size_t index) {
  return "Local" + boost::lexical_cast<std::string>(index) +
      "_" + scorer.alias();
}
}
}
