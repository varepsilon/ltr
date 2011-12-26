// Copyright 2011 Yandex

#include <string>
#include <vector>

#include "ltr/scorers/scorer.h"
#include "ltr/crossvalidation/validation_result.h"

using std::string;
using std::vector;


namespace ltr {
  namespace cv {
    ValidationResult::ValidationResult(const vector<string>& in_measure_names)
      : measure_names(in_measure_names) {
    }

    void ValidationResult::addSplitInfo(Scorer::Ptr in_scorer,
        const string& in_report, const vector<double>& in_measure_value) {
      datas_.push_back(OneSplitData(in_scorer, in_report, in_measure_value));
    }

    size_t ValidationResult::getSplitCount() const {
      return datas_.size();
    }

    Scorer::Ptr ValidationResult::getScorer(size_t split_index) const {
      return datas_[split_index].scorer;
    }

    const string& ValidationResult::getReport(size_t split_index) const {
      return datas_[split_index].report;
    }

    const vector<double>&
        ValidationResult::getMeasureValues(size_t split_index) const {
      return datas_[split_index].measure_values;
    }

    const vector<string>& ValidationResult::getMeasureNames() const {
      return measure_names;
    }
  };
};
