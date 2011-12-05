// Copyright 2011 Yandex

#include "ltr/scorers/utility/scorer_utility.h"

namespace ltr
{
  namespace utility {
    
    void MarkWithScorer(const Object& obj, const IScorer& scorer) {
      obj.setPredictedLabel(scorer(obj));
    }

    void MarkWithScorer(const ObjectPair& obj_pair, const IScorer& scorer) {
      MarkWithScorer(obj_pair.first(), scorer);
      MarkWithScorer(obj_pair.second(), scorer);
    }

    void MarkWithScorer(const ObjectList& obj_list, const IScorer& scorer) {
      for (int index = 0; index < obj_list.size(); ++index) {
        MarkWithScorer(obj_list[index], scorer);
      }
    }

  };
};
