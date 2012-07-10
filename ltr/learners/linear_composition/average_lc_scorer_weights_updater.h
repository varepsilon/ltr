// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_LINEAR_COMPOSITION_AVERAGE_LC_SCORER_WEIGHTS_UPDATER_H_
#define LTR_LEARNERS_LINEAR_COMPOSITION_AVERAGE_LC_SCORER_WEIGHTS_UPDATER_H_

#include <string>

#include <boost/shared_ptr.hpp> // NOLINT

#include "ltr/measures/measure.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/composition_scorers/linear_composition_scorer.h"
#include "ltr/scorers/utility/scorer_utility.h"
#include "ltr/utility/numerical.h"
#include "ltr/scorers/scorer.h"
#include "ltr/learners/linear_composition/linear_composition_scorer_weights_updater.h"

using std::exp;
using std::log;
using std::string;

using ltr::Measure;
using ltr::DataSet;
using ltr::LinearCompositionScorer;
using ltr::utility::MarkDataSet;
using ltr::Scorer;

namespace ltr {
namespace lc {
/**
 * LCScorerWeightsUpdater wich holds composition scorer's weights equal to
 * 1.0 / (composition length)
 */
template <class TElement>
class AverageLCScorerWeightsUpdater
  : public LCScorerWeightsUpdater<TElement> {
 public:
  typedef boost::shared_ptr<AverageLCScorerWeightsUpdater> Ptr;

  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit AverageLCScorerWeightsUpdater(
      const ParametersContainer& parameters = ParametersContainer()) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
  }

  void updateWeights(const DataSet<TElement>& data,
      LinearCompositionScorer* lin_scorer) const;
 private:
  virtual string getDefaultAlias() const {
    return "AverageLCScorerWeightsUpdater";
  }
};

// template realizations
template <class TElement>
void AverageLCScorerWeightsUpdater<TElement>::updateWeights(
    const DataSet<TElement>& data,
    LinearCompositionScorer* lin_scorer) const {
  double weight = 1.0 / lin_scorer->size();
  for (int i = 0; i < lin_scorer->size(); ++i) {
    lin_scorer->at(i).weight = weight;
  }
}
};
};

#endif  // LTR_LEARNERS_LINEAR_COMPOSITION_AVERAGE_LC_SCORER_WEIGHTS_UPDATER_H_
