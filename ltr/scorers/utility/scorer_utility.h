// Copyright 2011 Yandex

#ifndef LTR_SCORERS_UTILITY_SCORER_UTILITY_H_
#define LTR_SCORERS_UTILITY_SCORER_UTILITY_H_

#pragma once

#include <boost/lexical_cast.hpp>

#include "ltr/scorers/scorer.h"
#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"
#include "ltr/data/data_set.h"

namespace ltr {
  namespace utility {

    template <class Markable>
    void MarkDataSet(const DataSet<Markable>& elements, const IScorer& scorer) {
      for (size_t i = 0; i < elements.size(); i++) {
        MarkWithScorer(elements[i], scorer);
      }
    }

    void MarkWithScorer(const Object& obj, const IScorer& scorer);
    void MarkWithScorer(const ObjectPair& obj_pair, const IScorer& scorer);
    void MarkWithScorer(const ObjectList& obj_list, const IScorer& scorer);


    std::string GenerateLocalClassName(const IScorer& scorer, size_t index) {
      return "Local" + boost::lexical_cast<std::string>(index) +
        "_" + scorer.alias();
    }
  };
};

#endif  // LTR_SCORERS_UTILITY_SCORER_UTILITY_H_
