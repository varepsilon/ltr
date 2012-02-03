// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_LEARNER_H_
#define LTR_LEARNERS_LEARNER_H_

#include <boost/shared_ptr.hpp>
#include <vector>

#include "ltr/interfaces/reporter.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/scorer.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/parameters_container/parameters_container.h"

namespace ltr {

template< class TElement >
class BaseLearner : public Reporter, public Aliaser, public Parameterized {
  public:
  typedef boost::shared_ptr<BaseLearner> Ptr;
  typedef boost::shared_ptr<BaseLearner> BasePtr;

  void learn(const DataSet<TElement>& data);

  void addFeatureConverter(
      typename ltr::FeatureConverter::ConstPtr p_FeatureConverter);

  virtual void reset() = 0;
  virtual Scorer::Ptr makeScorerPtr() const = 0;

  const FeatureConverterArray& getFeatureConverters() const {
    return featureConverters_;
  }
  void setFeatureConverters(const FeatureConverterArray& featureConverters) {
    this->featureConverters_ = featureConverters;
  }

  virtual ~BaseLearner() {}

  private:
  virtual void learnImpl(const DataSet<TElement>& data) = 0;

  FeatureConverterArray featureConverters_;
};


template< class TElement, class TScorer >
class Learner : public BaseLearner< TElement > {
  public:
  virtual TScorer make() const = 0;
  virtual Scorer::Ptr makeScorerPtr() const;
  virtual void setInitialScorer(const TScorer& in_scorer) = 0;
};

template< class TElement, class TScorer >
Scorer::Ptr Learner< TElement, TScorer >::makeScorerPtr() const {
  Scorer *scr = new TScorer(make());
  return Scorer::Ptr(scr);
}

template< class TElement >
void BaseLearner< TElement >::addFeatureConverter(
    typename ltr::FeatureConverter::ConstPtr p_FeatureConverter) {
  featureConverters_.push_back(p_FeatureConverter);
}

template< class TElement >
void BaseLearner< TElement >::learn(const DataSet<TElement>& data) {
  DataSet<TElement> sourceData = data.deepCopy();
  DataSet<TElement> convertedData;
  for (size_t featureConverterIdx = 0;
      featureConverterIdx < featureConverters_.size();
      ++featureConverterIdx) {
    (*featureConverters_[featureConverterIdx]).apply(
        sourceData,
        &convertedData);
    sourceData = convertedData;
  }
  learnImpl(sourceData);
}
};
#endif  // LTR_LEARNERS_LEARNER_H_
