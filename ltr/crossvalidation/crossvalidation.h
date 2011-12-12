// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_CROSSVALIDATION_H_
#define LTR_CROSSVALIDATION_CROSSVALIDATION_H_

#include <string>
#include <vector>

#include "ltr/learners/learner.h"
#include "ltr/measures/measure.h"
#include "ltr/crossvalidation/validation_result.h"
#include "ltr/crossvalidation/splitter.h"
#include "ltr/scorers/utility/scorer_utility.h"

using std::string;
using std::vector;

namespace ltr {
  namespace cv {
    template<class TElement, class TScorer>
    const ValidationResult& Validate(
      const DataSet<TElement>& data_set,
      const vector< Measure<TElement> >& measures,
      typename Learner<TElement, TScorer>::Ptr learner,
      const Splitter<TElement>& splitter);
  };
};

namespace ltr {
  namespace cv {
    template<class TElement, class TScorer>
    const ValidationResult& Validate(
        const DataSet<TElement>& data_set,
        const vector< Measure<TElement> >& measures,
        typename Learner<TElement, TScorer>::Ptr learner,
        const Splitter<TElement>& splitter) {
      vector<string> measure_names;
      for (int i = 0; i < measures.size(); ++i) {
        measure_names.push_back(measures[i].alias());
      }
      ValidationResult output(measure_names);

      for (int split_index = 0;
          split_index < splitter.splitCount();
          ++split_index) {
        SplittedDataSet current_splitted(splitter.split(split_index, data_set));

        learner->reset();
        learner->learn(current_splitted.train_set);

        string current_report = learner->report();
        TScorer::Ptr current_scorer = TScorer::Ptr(new learner->make());

        utility::MarkDataSet(current_splitted.test_set, *current_scorer);

        vector<double> current_measure_values;
        for (int measure_index = 0;
            measure_index < measures.size();
            ++measure_index) {
          current_measure_values.push_back(
            measures[measure_index].average(current_splitted.test_set));
        }

        output.addSplitInfo(current_scorer,
          current_report,
          current_measure_values);
      }

      return output;
    }
  };
};

#endif  // LTR_CROSSVALIDATION_CROSSVALIDATION_H_
