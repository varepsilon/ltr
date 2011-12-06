// Copyright 2011 Yandex

#include "ltr/scorers/utility/scorer_utility.h"

namespace ltr {
namespace utility {

template <class TElement>
void MarkDataSet(const DataSet<TElement>& elements, const Scorer& scorer) {
  for (int i = 0; i < elements.size(); ++i) {
    MarkElement(elements[i], scorer);
  }
}

template <class TElement>
void MarkElement(const TElement& element, const Scorer& scorer) {
  for (int i = 0; i < element.size(); ++i) {
    element[i].setPredictedLabel(scorer(element[i]));
  }
}

}
}
