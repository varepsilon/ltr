// Copyright 2011 Yandex

#include "ltr/scorers/utility/scorer_utility.h"

namespace ltr {
  namespace utility {
    template <class Markable>
    void MarkDataSet(const DataSet<Markable>& elements, const IScorer& scorer){
      for (size_t i = 0; i < elements.size(); ++i) {
        MarkElement(elements[i], scorer);
      }
    }

    template <class Markable>
    void MarkElement(const Markable& element, const IScorer& scorer) {
      for (int i = 0; i < element.size(); ++i) {
        element[i].setPredictedLabel(scorer(element[i]));
      }
    }
  };
};
