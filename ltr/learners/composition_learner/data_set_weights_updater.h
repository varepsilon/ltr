// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_COMPOSITION_LEARNER_DATA_SET_WEIGHTS_UPDATER_H_
#define LTR_LEARNERS_COMPOSITION_LEARNER_DATA_SET_WEIGHTS_UPDATER_H_

#include <string>

#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/composition_scorers/composition_scorer.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/printable.h"

using std::string;

using ltr::Measure;
using ltr::DataSet;

namespace ltr {
namespace composition {
/**
 * Is a parameter for composition learner. Implements a strategy of
 * updating dataset's element's weights while adding a new scorer to
 * composition scorer. The idea is to increase weights of "hard" elements
 * with bad measure values and decrease weights of "easy" elements with good
 * measure values. Thus next scorer in composition could be learned
 * primary on bad elements and level the limitations of previous scorers
 */
template <class TElement>
class DataSetWeightsUpdater : public Aliaser, public Parameterized {
  ALLOW_SHARED_PTR_ONLY_CREATION(DataSetWeightsUpdater)
 public:
  /**
   * Updates dataset's weights
   * @param data - dataset to update it's weights
   * @param composition_scorer - composition scorer with already updated
   * by ltr::composition::CompositionScorerWeightsUpdater composition weights
   */
  virtual void updateWeights(const DataSet<TElement>* data,
      const CompositionScorer::Ptr& composition_scorer) const = 0;
  /**
   * Sets measure, used in DataSetWeightsUpdater. Note that some
   * DataSetWeightsUpdaters don't use measures, so they ignore
   * the mesure setted
   */
  SET(typename Measure<TElement>::Ptr, measure);
 protected:
  typename Measure<TElement>::Ptr measure_;
};


/**
 * Fake DataSetWeightsUpdater, does nothing
 */
template <class TElement>
class FakeDataSetWeightsUpdater : public DataSetWeightsUpdater<TElement> {
  ALLOW_SHARED_PTR_ONLY_CREATION(FakeDataSetWeightsUpdater)
 public:
  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit FakeDataSetWeightsUpdater(
      const ParametersContainer& parameters = ParametersContainer()) {
  }

  void updateWeights(const DataSet<TElement>* data,
      const CompositionScorer::Ptr& composition_scorer) const {
    // doing nothing
  }
 private:
  virtual string getDefaultAlias() const {return "FakeDataSetWieghtsUpdater";}
};
};
};

#endif  // LTR_LEARNERS_COMPOSITION_LEARNER_DATA_SET_WEIGHTS_UPDATER_H_
