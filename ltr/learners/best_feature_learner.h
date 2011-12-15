// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_BEST_FEATURE_LEARNER_H_
#define LTR_LEARNERS_BEST_FEATURE_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <limits>

#include "ltr/learners/learner.h"
#include "ltr/scorers/one_feature_scorer.h"
#include "ltr/scorers/utility/scorer_utility.h"
#include "ltr/measures/measure.h"

using ltr::DataSet;
using ltr::OneFeatureScorer;
using ltr::Measure;

namespace ltr {

template< class TElement >
class BestFeatureLearner : public Learner<TElement, OneFeatureScorer> {
  public:
  typedef boost::shared_ptr<BestFeatureLearner> Ptr;

  BestFeatureLearner(typename Measure<TElement>::Ptr pMeasure,
      size_t initialScorerIdx = 0) :
        scorer_(initialScorerIdx),
        p_measure_(pMeasure) {}

  void setInitialScorer(const OneFeatureScorer& initialScorer) {
    scorer_ = initialScorer;
  }
  OneFeatureScorer make() const {
    return scorer_;
  }
  void reset() {
    scorer_ = OneFeatureScorer(0);
  }


  private:
  typename Measure<TElement>::Ptr p_measure_;
  OneFeatureScorer scorer_;

  void learnImpl(const DataSet<TElement>& data);
};

template< class TElement >
void BestFeatureLearner<TElement>::learnImpl(const DataSet<TElement>& data) {
  double bestMeasureValue = p_measure_->worstValue();
  size_t bestFeatureIdx = 0;

  for (size_t featureIdx = 0; featureIdx < data.featureCount(); ++featureIdx) {
    OneFeatureScorer scorer(featureIdx);
    utility::MarkDataSet(data, scorer);
    double measureValue = p_measure_->average(data);
    if (p_measure_->isBetter(measureValue, bestMeasureValue)) {
      bestMeasureValue = measureValue;
      bestFeatureIdx = featureIdx;
    }
  }
  scorer_ = OneFeatureScorer(bestFeatureIdx, this->getFeatureConverters());
}
}
#endif  // LTR_LEARNERS_BEST_FEATURE_LEARNER_H_
