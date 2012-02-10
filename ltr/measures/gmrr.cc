// Copyright 2011 Yandex

#include <algorithm>
#include <stdexcept>
#include <cmath>

#include "ltr/measures/gmrr.h"
#include "ltr/data/object_list.h"
#include "ltr/measures/utils/measure_utility.h"

using std::sort;
using std::logic_error;
using std::pow;
using ltr::utility::PredictedAndActualLabels;
using ltr::utility::ExtractLabels;
using ltr::utility::PredictedDecreasingActualIncreasing;
using ltr::ObjectList;


namespace ltr {
  double GMRR::GMRRFormula(double pos, double label, double p_look) const {
    return (1.0 / pos) * p_look *
      (pow(2.0, label) - 1) / pow(2.0, this->parameters().getInt("MAX_LABEL"));
  }

  double GMRR::get_measure(const ObjectList& objects) const {
    vector<PredictedAndActualLabels> labels = ExtractLabels(objects);
    sort(labels.begin(), labels.end(), PredictedDecreasingActualIncreasing);

    size_t n = this->parameters().getInt("NUMBER_OF_OBJECTS_TO_CONSIDER");
    if ((n == 0) || (n > labels.size())) {
      n = labels.size();
    }

    double p_look = 1.0, p_relevance = 0.0, result = 0.0;
    int pos = 1;
    for (int labels_index = 0; labels_index < n; ++labels_index, ++pos) {
      if (labels[labels_index].actual < 0) {
        throw logic_error(alias() + " some actual object label < 0");
      }
      result += GMRRFormula(static_cast<double>(pos), labels[labels_index].actual, p_look);
      p_relevance = labels[labels_index].actual / this->parameters().getInt("MAX_LABEL");
      p_look *= (1 - p_relevance);
    }

    if (result > 1.0) {
      throw logic_error(alias() + " calculated > 1");
    } else if (result < 0.0) {
      throw logic_error(alias() + " calculated < 0");
    }
    return result;
  }
};
