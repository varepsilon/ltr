// Copyright 2011 Yandex

#ifndef LTR_LINEAR_COMPOSITION_LINEAR_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_
#define LTR_LINEAR_COMPOSITION_LINEAR_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_

#include <boost/shared_ptr.hpp>
#include <string>

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/linear_composition_scorer.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/interfaces/parameterized.h"

using std::string;
using ltr::Measure;
using ltr::DataSet;
using ltr::LinearCompositionScorer;

namespace ltr {
namespace lc {
  template <class TElement>
  class LCScorerWeightsUpdater : public Aliaser, public Parameterized {
  public:
    typedef boost::shared_ptr<LCScorerWeightsUpdater> Ptr;

    explicit LCScorerWeightsUpdater(const string& alias) : Aliaser(alias) {}

    virtual void updateWeights(const DataSet<TElement>& data,
        LinearCompositionScorer* lin_scorer) const =0;

    void setMeasure(typename Measure<TElement>::Ptr in_measure) {
      measure_ = in_measure;
    }
  protected:
    typename Measure<TElement>::Ptr measure_;
  };



  template <class TElement>
  class FakeLCScorerWeightsUpdater : public LCScorerWeightsUpdater<TElement> {
  public:
    typedef boost::shared_ptr<FakeLCScorerWeightsUpdater> Ptr;

    explicit FakeLCScorerWeightsUpdater(
        const ParametersContainer& parameters = ParametersContainer())
        : LCScorerWeightsUpdater<TElement>("FakeLCScorerWeightsUpdater") {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }

    void updateWeights(const DataSet<TElement>& data,
        LinearCompositionScorer* lin_scorer) const {
      // doing nothing
    }
  };
};
};

#endif  // LTR_LINEAR_COMPOSITION_LINEAR_COMPOSITION_SCORER_WEIGHTS_UPDATER_H_
