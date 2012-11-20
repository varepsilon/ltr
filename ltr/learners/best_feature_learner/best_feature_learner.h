// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_BEST_FEATURE_LEARNER_BEST_FEATURE_LEARNER_H_
#define LTR_LEARNERS_BEST_FEATURE_LEARNER_BEST_FEATURE_LEARNER_H_

#include <stdexcept>
#include <string>
#include <limits>

#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/learners/learner.h"
#include "ltr/scorers/one_feature_scorer.h"
#include "ltr/measures/measure.h"

using std::string;

using ltr::DataSet;
using ltr::OneFeatureScorer;
using ltr::Measure;

namespace ltr {

template< class TElement >
class BestFeatureLearner : public BaseLearner<TElement, OneFeatureScorer> {
 public:
  typedef ltr::utility::shared_ptr<BestFeatureLearner> Ptr;

  BestFeatureLearner() {
  }

  explicit BestFeatureLearner(const ParametersContainer& parameters) {
    this->setParameters(parameters);
  }

  explicit BestFeatureLearner(typename Measure<TElement>::Ptr pMeasure) {
    this->set_measure(pMeasure);
  }

  void setDefaultParameters() {
    measure_ = typename Measure<TElement>::Ptr();
  }

  GET_SET(typename Measure<TElement>::Ptr, measure);

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters) {
    measure_ = parameters.Get<typename Measure<TElement>::Ptr>("measure");
  }
  virtual void learnImpl(const DataSet<TElement>& data,
                         OneFeatureScorer* scorer);
  virtual string getDefaultAlias() const {return "BestFeatureLeaner";}

  typename Measure<TElement>::Ptr measure_;
};

template< class TElement >
void BestFeatureLearner<TElement>::learnImpl(const DataSet<TElement>& data,
                                             OneFeatureScorer* scorer) {
  rInfo("Starting learning");
  if (measure_.get() == 0) {
    rError("Measure is not setted");
    throw std::logic_error("Set measure first.");
  }
  if (data.feature_count() == 0) {
    rError("Data is empty");
    throw std::logic_error("There are no features for BF learner.");
  }

  // \TODO Rewrite using setter and getters
  size_t best_feature_index = 0;
  OneFeatureScorer current_scorer(best_feature_index);
  current_scorer.predict(data);
  double best_measure_value = this->measure_->average(data);

  for (int feature_index = 1;
       feature_index < data.feature_count();
       ++feature_index) {
    OneFeatureScorer current_scorer(feature_index);
    current_scorer.predict(data);
    double measure_value = this->measure_->average(data);
    if (this->measure_->better(measure_value, best_measure_value)) {
      best_measure_value = measure_value;
      best_feature_index = feature_index;
    }
  }
  *scorer = OneFeatureScorer(best_feature_index);
}
}
#endif  // LTR_LEARNERS_BEST_FEATURE_LEARNER_BEST_FEATURE_LEARNER_H_
