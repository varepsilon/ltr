// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <string>
#include <stdexcept>

#include "ltr/measures/reciprocal_rank.h"
#include "ltr/measures/utils/measure_utility.h"

using std::sort;
using std::string;
using std::logic_error;
using ltr::utility::PredictedAndActualLabels;
using ltr::utility::ExtractLabels;
using ltr::utility::PredictedDecreasingActualIncreasing;

namespace ltr {
  double ReciprocalRank::RRFormula(double pos) {
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
      if (labels_it->actual >= getDoubleParameter("SCORE_FOR_RELEVANT")) {
        result = RRFormula(static_cast<double>(pos));
        relevant_found = true;
        break;
      }
    }

    if (!relevant_found) {
      string str;
      str.append(alias() + " no relevants for some query on relevant score = ").
        append(boost::lexical_cast<string>(
          getDoubleParameter("SCORE_FOR_RELEVANT")));
      throw logic_error(str);
    }

    if (result > 1.0) {
      throw logic_error(alias() + " calculated > 1");
    } else if (result  < 0.0) {
      throw logic_error(alias() + " calculated < 0");
    }
    return result;
  }
};
