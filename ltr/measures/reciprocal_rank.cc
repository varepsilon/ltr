// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <stdexcept>
#include <sstream>

#include "ltr/measures/reciprocal_rank.h"
#include "ltr/measures/utils/measure_utility.h"

using std::sort;
using std::string;
using std::logic_error;
using ltr::utility::PredictedAndActualLabels;
using ltr::utility::ExtractLabels;
using ltr::utility::PredictedDecreasingActualIncreasing;

namespace ltr {
  double ReciprocalRank::RRFormula(int pos) {
    return 1.0 / pos;
  }

  double ReciprocalRank::get_measure(const ObjectList& objects) const {
    vector<PredictedAndActualLabels> labels = ExtractLabels(objects);
    sort(labels.begin(), labels.end(), PredictedDecreasingActualIncreasing);

    double result = 0.0;
    bool relevant_found = false;
    size_t pos = 1;

    for (vector<PredictedAndActualLabels>::const_iterator labels_it
        = labels.begin(); labels_it != labels.end();
        ++labels_it, pos += 1) {
      if (labels_it->actual >=
          this->getParameter<double>("SCORE_FOR_RELEVANT")) {
        result = RRFormula(pos);
        relevant_found = true;
        break;
      }
    }

    if (!relevant_found) {
      // todo: log here!
    }
    return result;
  }

  string ReciprocalRank::toString() const {
    std::stringstream str;
    std::fixed(str);
    str.precision(2);
    str << "Reciprocal Rank measure with parameter SCORE_FOR_RELEVANT = ";
    str << this->getParameter<double>("SCORE_FOR_RELEVANT");
    return str.str();
  }
};
