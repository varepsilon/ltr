// Copyright 2011 Yandex

#ifndef LTR_LINEAR_COMPOSITION_ADA_RANK_DATA_SET_WEIGHTS_UPDATER_H_
#define LTR_LINEAR_COMPOSITION_ADA_RANK_DATA_SET_WEIGHTS_UPDATER_H_

#include <boost/shared_ptr.hpp>

#include <cmath>
#include <vector>

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/linear_composition_scorer.h"
#include "ltr/scorers/utility/scorer_utility.h"

using std::vector;
using std::exp;

using ltr::Measure;
using ltr::DataSet;
using ltr::LinearCompositionScorer;
using ltr::utility::MarkDataSet;

namespace ltr {
namespace lc {
  template <class TElement>
  class AdaRankDataSetWeightsUpdater {
  public:
    typedef boost::shared_ptr<AdaRankDataSetWeightsUpdater> Ptr;

    explicit AdaRankDataSetWeightsUpdater() {}

    void updateWeights(const DataSet<TElement>* data,
        const LinearCompositionScorer& lin_scorer) const;

    void setMeasure(typename Measure<TElement>::Ptr in_measure) {
      measure_ = in_measure;
    }
  protected:
    typename Measure<TElement>::Ptr measure_;
  };

  // template realizations
  template <class TElement>
  void AdaRankDataSetWeightsUpdater<TElement>::updateWeights(
      const DataSet<TElement>* data,
      const LinearCompositionScorer& lin_scorer) const {
    int last_scorer_number = static_cast<int>(lin_scorer.size()) - 1;
    MarkDataSet(*data, lin_scorer[last_scorer_number].scorer);

    vector<double> measure_exps(data->size());
    double sum_exps = 0.0;

    for (size_t data_index = 0; data_index < data->size(); ++data_index) {
      double measure_value = measure_->operator()(data->at(data_index));
      double measure_value_exp = exp(1 - 2 * measure_value);
      measure_exps[data_index] = measure_value_exp;
      sum_exps += measure_value_exp;
    }

    for (size_t data_index = 0; data_index < data->size(); ++data_index) {
      data->at(data_index).setWeight(measure_exps[data_index] / sum_exps);
    }
  }
};
};

#endif  // LTR_LINEAR_COMPOSITION_ADA_RANK_DATA_SET_WEIGHTS_UPDATER_H_
