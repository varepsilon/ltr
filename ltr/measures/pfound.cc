// Copyright 2011 Yandex

#include <algorithm>
#include <stdexcept>

#include "ltr/data/object_list.h"
#include "ltr/measures/pfound.h"
#include "ltr/measures/utils/measure_utility.h"

using std::sort;
using std::logic_error;
using ltr::utility::PredictedAndActualLabels;
using ltr::utility::ExtractLabels;
using ltr::utility::PredictedDecreasingActualIncreasing;
using ltr::ObjectList;

namespace ltr {
  double PFound::get_measure(const ObjectList& objects) const {
	  vector<PredictedAndActualLabels> labels = ExtractLabels(objects);
    sort(labels.begin(), labels.end(), PredictedDecreasingActualIncreasing);

    size_t n = this->parameters().getInt("NUMBER_OF_OBJECTS_TO_CONSIDER");
    if ((n == 0) || (n > labels.size())) {
      n = labels.size();
    }

    double p_look = 1.0, p_relevance = 0.0, result = 0.0;
    double p_break = this->parameters().getDouble("P_BREAK");
    double max_label = this->parameters().getDouble("MAX_LABEL");
    size_t cur = 1;

    for (int labels_index = 0; labels_index < n; ++labels_index) {
      if (labels[labels_index].actual < 0) {
        throw logic_error(alias() + " some actual object label < 0");
      }
      p_relevance = labels[labels_index].actual / max_label;

      result += p_look * p_relevance;
      p_look *= (1 - p_relevance) * (1 - p_break);
    }

    if (result > 1.0) {
      throw logic_error(alias() + " calculated > 1");
    }
    return result;
  }
};
