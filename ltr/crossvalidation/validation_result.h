// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_VALIDATION_RESULT_H_
#define LTR_CROSSVALIDATION_VALIDATION_RESULT_H_

#include <string>
#include <vector>

#include "ltr/scorers/scorer.h"

using std::string;
using std::vector;


namespace ltr {
  namespace cv {
    /*
    * For every split of crossvalidation done ValidationResult holds scorer
    * with it's learner's report and array of measure values on some test data
    */
    class ValidationResult {
     public:
      explicit ValidationResult(const vector<string>& in_measure_names);

      void addSplitInfo(Scorer::Ptr in_scorer,
        const string& in_report, const vector<double>& in_measure_value);

      size_t getSplitCount() const;
      Scorer::Ptr getScorer(size_t split_index) const;
      const string& getReport(size_t split_index) const;
      const vector<double>& getMeasureValues(size_t split_index) const;
      const vector<string>& getMeasureNames() const;
     private:
      struct OneSplitData {
        // now scorer here is not used
        // but holding it is cheap and could be useful in future
        Scorer::Ptr scorer;
        string report;
        vector<double> measure_values;

        OneSplitData(Scorer::Ptr in_scorer, const string& in_report,
            const vector<double>& in_measure_value) :
          scorer(in_scorer),
          report(in_report),
          measure_values(in_measure_value) {}
      };

      vector<OneSplitData> datas_;
      vector<string> measure_names;
    };
  };
};

#endif  // LTR_CROSSVALIDATION_VALIDATION_RESULT_H_
