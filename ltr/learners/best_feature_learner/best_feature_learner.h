// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_BEST_FEATURE_LEARNER_BEST_FEATURE_LEARNER_H_
#define LTR_LEARNERS_BEST_FEATURE_LEARNER_BEST_FEATURE_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <stdexcept>
#include <string>
#include <limits>


#include "ltr/learners/learner.h"
#include "ltr/scorers/one_feature_scorer.h"
#include "ltr/scorers/utility/scorer_utility.h"
#include "ltr/measures/measure.h"

using ltr::DataSet;
using ltr::OneFeatureScorer;
using ltr::Measure;

using std::string;

namespace ltr {

template< class TElement >
class BestFeatureLearner : public BaseLearner<TElement, OneFeatureScorer> {
 public:
  typedef boost::shared_ptr<BestFeatureLearner> Ptr;

  BestFeatureLearner
      (const ParametersContainer& parameters = ParametersContainer()) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
  }

  explicit BestFeatureLearner(typename Measure<TElement>::Ptr pMeasure) {
    this->set_measure(pMeasure);
  }

  void setDefaultParameters() {
    Parameterized::addNewParam<Measure<TElement>*>("measure", NULL);
  }
  virtual void parametersUpdateCallback() {
    Measure<TElement> *msr =
        Parameterized::getParameter<Measure<TElement>*>("measure");
    this->measure_ = typename Measure<TElement>::Ptr(msr);
  }

 private:
  virtual void learnImpl(const DataSet<TElement>& data,
                         OneFeatureScorer* scorer);
  virtual string getDefaultAlias() const {return "BestFeatureLeaner";}
};

template< class TElement >
void BestFeatureLearner<TElement>::learnImpl(const DataSet<TElement>& data,
                                             OneFeatureScorer* scorer) {
  if (data.feature_count() == 0) {
    throw std::logic_error("There are no features for BF learner.");
  }

  // \TODO Rewrite using setter and getters
  size_t bestFeatureIdx = 0;
  OneFeatureScorer current_scorer(bestFeatureIdx);
  utility::MarkDataSet(data, current_scorer);
  double bestMeasureValue = this->measure_->average(data);

  for (size_t featureIdx = 1; featureIdx < data.feature_count(); ++featureIdx) {
    OneFeatureScorer current_scorer(featureIdx);
    utility::MarkDataSet(data, current_scorer);
    double measureValue = this->measure_->average(data);
    if (this->measure_->better(measureValue, bestMeasureValue)) {
      bestMeasureValue = measureValue;
      bestFeatureIdx = featureIdx;
    }
  }
  *scorer = OneFeatureScorer(bestFeatureIdx);
}
}
#endif  // LTR_LEARNERS_BEST_FEATURE_LEARNER_BEST_FEATURE_LEARNER_H_
