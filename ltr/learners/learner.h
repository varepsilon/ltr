// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_LEARNER_H_
#define LTR_LEARNERS_LEARNER_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

using std::string;

#include "ltr/interfaces/reporter.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/scorer.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/utility/utility.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/measures/measure.h"

namespace ltr {
/**
 * BaseLearner, implements a learning (on data) strategy of a specific algorithm
 * (e. g. desicion tree). Has a descendent Learner which also can produce specific result
 * scorers. Is used everywhere where a Ptr on Learner is needed - having Ptr on
 * Learner is inconvenient cause Learner is also parametrised be TScorer
 * and inheritance tree is a forest
 */
template<class TElement>
class BaseLearner : public Reporter, public Aliaser, public Parameterized {
  public:
  typedef boost::shared_ptr<BaseLearner> Ptr;
  typedef boost::shared_ptr<BaseLearner> BasePtr;

  explicit BaseLearner(const string& alias) : Aliaser(alias) {}

  void learn(const DataSet<TElement>& data);

  void addFeatureConverter(
      typename ltr::FeatureConverter::ConstPtr p_feature_converter);

  virtual void reset() = 0;
  virtual Scorer::Ptr makeScorerPtr() const = 0;

  const FeatureConverterArray& getFeatureConverters() const {
    return feature_converters_;
  }
  void setFeatureConverters(const FeatureConverterArray& featureConverters) {
    this->feature_converters_ = featureConverters;
  }

  virtual ~BaseLearner() {}
  void setMeasure(typename Measure<TElement>::Ptr measure) {
    p_measure_ = measure;
  }
  void setWeakLearner(typename BaseLearner<TElement>::Ptr weak_learner) {
    p_weak_learner_ = weak_learner;
  }

  protected:
  typename Measure<TElement>::Ptr p_measure_;
  typename BaseLearner<TElement>::Ptr p_weak_learner_;

  private:
  virtual void learnImpl(const DataSet<TElement>& data) = 0;

  FeatureConverterArray feature_converters_;
};

/**
 * Learner, implements a learning (on data) strategy of a specific algorithm
 * (e. g. desicion tree) and can produce specific result scorers. Everywhere where
 * a Ptr on Learner is needed - use Ptr on BaseLearner. Having Ptr on
 * Learner is inconvenient cause Learner is also parametrised by TScorer
 * and inheritance tree is a forest
 */
template<class TElement, class TScorer>
class Learner : public BaseLearner<TElement> {
  public:
  explicit Learner(const string& alias) : BaseLearner<TElement>(alias) {}

  /**
   * Is for being sure Scorer::Ptrs outputted by makeScorerPtr() are Ptrs on
   * different (physically) scorers
   * @returns a concrete scorer
   */
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
  feature_converters_.push_back(p_FeatureConverter);
}

template< class TElement >
void BaseLearner< TElement >::learn(const DataSet<TElement>& data) {
  DataSet<TElement> sourceData = data.deepCopy();
  DataSet<TElement> convertedData;
  for (size_t featureConverterIdx = 0;
      featureConverterIdx < feature_converters_.size();
      ++featureConverterIdx) {
        ltr::utility::ApplyFeatureConverter(
          feature_converters_[featureConverterIdx],
          sourceData,
          &convertedData);
    sourceData = convertedData;
  }
  learnImpl(sourceData);
}
};
#endif  // LTR_LEARNERS_LEARNER_H_
