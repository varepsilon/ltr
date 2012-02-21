// Copyright 2011 Yandex

#ifndef LTR_LINEAR_COMPOSITION_DATA_SET_WEIGHTS_UPDATER_H_
#define LTR_LINEAR_COMPOSITION_DATA_SET_WEIGHTS_UPDATER_H_

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
  class FakeDataSetWeightsUpdater {
  public:
    typedef boost::shared_ptr<FakeDataSetWeightsUpdater> Ptr;

    explicit FakeDataSetWeightsUpdater(
        typename Measure<TElement>::Ptr in_measure)
      : measure_(in_measure) {}

    void updateWeights(DataSet<TElement>* data,
        const LinearCompositionScorer& lin_scorer) const {
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

#endif  // LTR_LINEAR_COMPOSITION_DATA_SET_WEIGHTS_UPDATER_H_
