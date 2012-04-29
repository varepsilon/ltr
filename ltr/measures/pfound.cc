// Copyright 2011 Yandex

#include <algorithm>
#include <stdexcept>
#include <sstream>

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

    size_t n = this->getIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER");
    if ((n == 0) || (n > labels.size())) {
      n = labels.size();
    }

    double p_look = 1.0, p_relevance = 0.0, result = 0.0;
    double p_break = this->getDoubleParameter("P_BREAK");
    double max_label = this->getDoubleParameter("MAX_LABEL");
    size_t cur = 1;

    for (int labels_index = 0; labels_index < n; ++labels_index) {
      if (labels[labels_index].actual < 0) {
        // todo: log here!
        // throw logic_error(alias() + " some actual object label < 0");
        labels[labels_index].actual = 0;
      }
      if (labels[labels_index].actual > max_label) {
        // todo: log here!
        // throw logic_error(alias() + " some actual object label > MAX_LABEL");
        labels[labels_index].actual = max_label;
      }
      p_relevance = labels[labels_index].actual / max_label;

      result += p_look * p_relevance;
      p_look *= (1 - p_relevance) * (1 - p_break);
    }
    return result;
  }

  string PFound::toString() const {
    std::stringstream str;
    std::fixed(str);
    str.precision(2);
    str << "PFound measure with parameters: P_BREAK = ";
    str << this->getDoubleParameter("P_BREAK");
    str << ", MAX_LABEL = ";
    str << this->getDoubleParameter("MAX_LABEL");
    str << ", NUMBER_OF_OBJECTS_TO_CONSIDER = ";
    str << getIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER");
    return str.str();
  }
};
