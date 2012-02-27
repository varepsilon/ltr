// Copyright 2011 Yandex

#ifndef LTR_LINEAR_COMPOSITION_ADA_RANK_LC_SCORER_WEIGHTS_UPDATER_H_
#define LTR_LINEAR_COMPOSITION_ADA_RANK_LC_SCORER_WEIGHTS_UPDATER_H_

#include <boost/shared_ptr.hpp>

#include <algorithm>

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/linear_composition_scorer.h"
#include "ltr/scorers/utility/scorer_utility.h"
#include "ltr/utility/numerical.h"
#include "ltr/scorers/scorer.h"

using std::exp;
using std::log;
using std::swap;

using ltr::Measure;
using ltr::DataSet;
using ltr::LinearCompositionScorer;
using ltr::utility::MarkDataSet;
using ltr::utility::DoubleEps;
using ltr::Scorer;

namespace ltr {
namespace lc {
  template <class TElement>
  class AdaRankLCScorerWeightsUpdater {
  public:
    typedef boost::shared_ptr<AdaRankLCScorerWeightsUpdater> Ptr;

    explicit AdaRankLCScorerWeightsUpdater() {}

    void updateWeights(const DataSet<TElement>& data,
        LinearCompositionScorer* lin_scorer) const;

    void setMeasure(typename Measure<TElement>::Ptr in_measure) {
      measure_ = in_measure;
    }
  protected:
    typename Measure<TElement>::Ptr measure_;
  };

  // template realizations
  template <class TElement>
  void AdaRankLCScorerWeightsUpdater<TElement>::updateWeights(
      const DataSet<TElement>& data,
      LinearCompositionScorer* lin_scorer) const {
    int last_scorer_number = static_cast<int>(lin_scorer->size()) - 1;
    MarkDataSet(data, *lin_scorer->at(last_scorer_number).scorer);

    double numerator = 0.0;
    double denominator = 0.0;
// todo: weightted average + max and min of measure (eg vector <-)
    for (size_t i = 0; i < data.size(); ++i) {
      double measure_value = measure_->operator()(data[i]);

      numerator += data.getWeight(i) * measure_value;
      denominator += data.getWeight(i) * (1 - measure_value);
    }

    bool measure_more_is_better = measure_->better(1.0, 0.0);
    if (!measure_more_is_better) {
      swap(numerator, denominator);
    }

    if (denominator < DoubleEps) {
      Scorer::Ptr best_scorer = (*lin_scorer)[last_scorer_number].scorer;
      lin_scorer->clear();
      lin_scorer->add(best_scorer, 1.0);
      // report_ = "AdaRank on mertic " +
      //  measure_.Alias() + ". Best ranker found!";
      return;
    }

    double result_weight = 0.5 * log(numerator/denominator);

    (*lin_scorer)[last_scorer_number].weight = result_weight;
  }
};
};

#endif  // LTR_LINEAR_COMPOSITION_ADA_RANK_LC_SCORER_WEIGHTS_UPDATER_H_
