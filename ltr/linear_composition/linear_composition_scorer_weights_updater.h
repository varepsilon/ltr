// Copyright 2011 Yandex

#ifndef LTR_LINEAR_COMPOSITION_LINEAR_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_
#define LTR_LINEAR_COMPOSITION_LINEAR_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_

#include <boost/shared_ptr.hpp>

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/linear_composition_scorer.h"

using ltr::Measure;
using ltr::DataSet;
using ltr::LinearCompositionScorer;

namespace ltr {
namespace lc {
  template <class TElement>
  class FakeLCScorerWeightsUpdater {
  public:
    typedef boost::shared_ptr<FakeLCScorerWeightsUpdater> Ptr;

    explicit FakeLCScorerWeightsUpdater(
        typename Measure<TElement>::Ptr in_measure)
      : measure_(in_measure) {}

    void updateWeights(const DataSet<TElement>& data,
        LinearCompositionScorer* lin_scorer) const {
      // doing nothing
    }

    void setMeasure(typename Measure<TElement>::Ptr in_measure) {
      measure_ = in_measure;
    }
  protected:
    typename Measure<TElement>::Ptr measure_;
  };
};
};

#endif  // LTR_LINEAR_COMPOSITION_LINEAR_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_
