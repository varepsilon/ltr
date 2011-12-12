// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_VALIDATION_RESULT_H_
#define LTR_CROSSVALIDATION_VALIDATION_RESULT_H_

#include <string>
#include <vector>

#include "scorers/scorer.h"

using std::string;
using std::vector;


namespace ltr {
  namespace cv {

    class ValidationResult {
     public:
      ValidationResult(const vector<string>& in_measure_names);

      void addSplitInfo(Scorer::Ptr in_scorer,
        const string& in_report, double in_measure_value);

      size_t getSplitCount() const;
      Scorer::Ptr getScorer(size_t split_index) const;
      const string& getReport(size_t split_index) const;
      const vector<double>& getMeasureValues(size_t split_index) const;
      const vector<string>& getMeasureNames() const;
     private:
      struct OneSplitData {
        // now scorer here is not used, but holding it is cheap and could be useful in future
        Scorer::Ptr scorer;
        string report;
        vector<double> measure_values;

        OneSplitData(Scorer::Ptr in_scorer, const string& in_report,
            double in_measure_value) :
          scorer(in_scorer),
          report(in_report),
          measure_value(in_measure_value) {}
      };

      vector<OneSplitData> datas_;
      vector<string> measure_names;
    };
  };
};

namespace ltr {
  namespace cv {
    ValidationResult::ValidationResult(const vector<string>& in_measure_names)
      : measure_names(in_measure_names) {
    }

    void ValidationResult::addSplitInfo(Scorer::Ptr in_scorer,
        const string& in_report, double in_measure_value) {
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

    const vector<double>& ValidationResult::getMeasureValues(size_t split_index) const {
      return datas_[split_index].measure_values;
    }

    const vector<string>& ValidationResult::getMeasureNames() const {
      return measure_names;
    }
  };
};

#endif  // LTR_CROSSVALIDATION_VALIDATION_RESULT_H_
