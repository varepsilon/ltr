// Copyright 2011 Yandex

#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <sstream>

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
  double GMRR::GMRRFormula(double label) const {
    return (pow(2.0, label) - 1)
      / pow(2.0, max_label_);
  }

  double GMRR::get_measure(const ObjectList& objects) const {
    vector<PredictedAndActualLabels> labels = ExtractLabels(objects);
    sort(labels.begin(), labels.end(), PredictedDecreasingActualIncreasing);

    size_t number_of_objects_to_consider = number_of_objects_to_consider_;
    if ((number_of_objects_to_consider == 0) || 
        (number_of_objects_to_consider > labels.size())) {
      number_of_objects_to_consider = labels.size();
    }

    double p_look = 1.0, p_relevance = 0.0, result = 0.0;
    int pos = 1;
    for (int labels_index = 0; 
         labels_index < number_of_objects_to_consider; 
         ++labels_index, ++pos) {
      if (labels[labels_index].actual < 0) {
        // todo: log here!
        // throw logic_error(alias() + " some actual object label < 0");
        labels[labels_index].actual = 0;
      }
      if (labels[labels_index].actual
          > max_label_) {
        // todo: log here!
        // throw logic_error(alias() + " some actual object label > MAX_LABEL");
        labels[labels_index].actual = max_label_;
      }
      p_relevance = GMRRFormula(labels[labels_index].actual);
      result += (1.0 / pos) * p_look * p_relevance;
      p_look *= (1 - p_relevance);
    }
    return result;
  }

  string GMRR::toString() const {
    std::stringstream str;
    str << "GMRR measure with parameter NUMBER_OF_OBJECTS_TO_CONSIDER = ";
    str << this->number_of_objects_to_consider_;
    return str.str();
  }
};
