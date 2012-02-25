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
    /**
     * For every split of crossvalidation done ValidationResult holds scorer
     * with it's learner's report and array of measure values on some test data
     */
    class ValidationResult {
    public:
      /**
       * @param in_measure_names - names (aliases) of measures, which values
       * are held in ValidationResult
       */
      explicit ValidationResult(const vector<string>& in_measure_names);
      /**
       * Adds information about one split - resulted scorer, learner's report
       * and values of measures
       * @param in_scorer - scorer, received by learner after it's work on train data
       * @param in_report - report of learner about scorer received
       * @param in_measure_value - measure values of recieved scorer on test data
       */
      void addSplitInfo(Scorer::Ptr in_scorer,
        const string& in_report, const vector<double>& in_measure_value);
      /**
       * Returns number of splits, about which holds information ValidationResult
       */
      size_t getSplitCount() const;
      /**
       * Gets scorer by split index
       */
      Scorer::Ptr getScorer(int split_index) const;
      /**
       * Gets report by split index
       */
      const string& getReport(int split_index) const;
      /**
       * Gets measure values by split index
       */
      const vector<double>& getMeasureValues(int split_index) const;
      /**
       * Gets names (aliases) of measures, which values
       * are held in ValidationResult
       */
      const vector<string>& getMeasureNames() const;
    private:
      /**
       * Holds whole information about one split
       */
      struct OneSplitData {
        // now scorer here is not used
        // but holding it is cheap and could be useful in future
        Scorer::Ptr scorer;
        string report;
        vector<double> measure_values;

        /**
         * @param in_scorer - scorer, received by learner after it's work on train data
         * @param in_report - report of learner about scorer received
         * @param in_measure_value - measure values of recieved scorer on test data
         */
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
