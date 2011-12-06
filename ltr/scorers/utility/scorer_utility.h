// Copyright 2011 Yandex

#ifndef LTR_SCORERS_UTILITY_SCORER_UTILITY_H_
#define LTR_SCORERS_UTILITY_SCORER_UTILITY_H_

#pragma once

#include <boost/lexical_cast.hpp>

#include <string>

#include "ltr/scorers/scorer.h"
#include "ltr/data/data_set.h"

namespace ltr {
  namespace utility {

    template <class Markable>
    void MarkDataSet(const DataSet<Markable>& elements, const IScorer& scorer);

    template <class Markable>
    void MarkElement(const Markable& element, const IScorer& scorer);


    std::string GenerateLocalClassName(const IScorer& scorer, size_t index) {
      return "Local" + boost::lexical_cast<std::string>(index) +
        "_" + scorer.alias();
    }
  };
};

#endif  // LTR_SCORERS_UTILITY_SCORER_UTILITY_H_
