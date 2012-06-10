// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include "ltr/data/data_set.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/printable.h"
#include "ltr/parameters_container/parameters_container.h"

namespace ltr {
/**
 * A class which learns on input dataset and produces a FeatureConverter.
 *
 * May produce different converters (using random) for one inputted dataset
 * as in RSM (random subset method). Has a descendent FeatureConverterLearner
 * which also can produce specific FeatureConverters. Is used everywhere where
 * a Ptr on FeatureConverterLearner is needed - having Ptr on FeatureConverterLearner
 * is inconvenient cause FeatureConverterLearner is also parametrised by
 * TFeatureConverter.
 */
template <class TElement>
class BaseFeatureConverterLearner : public Parameterized,
                                    public Printable {
 public:
  typedef boost::shared_ptr<BaseFeatureConverterLearner> Ptr;
  /**
   * Learns from input dataset. E. g. remembers number of features
   * in dataset and e.t.c.
   */
  virtual void learn(const DataSet<TElement>& data_set) = 0;
  /**
   * Produces a FeatureConverter. Must have been learned before calling make()
   */
  virtual FeatureConverter::Ptr make() const = 0;
};

/**
 * \brief A class which learns on inputted dataset and produces 
 * a specific FeatureConverter. 
 *
 * May produce different converters (using random)
 * for one inputted dataset as in RSM (random subset method). Everywhere
 * where a Ptr on FeatureConverterLearner is needed - use Ptr on
 * BaseFeatureConverterLearner. Having Ptr on FeatureConverterLearner is
 * inconvenient cause FeatureConverterLearner is also parametrised by
 * TFeatureConverter and inheritance tree is a forest
 */
template <class TElement, class TFeatureConverter>
class FeatureConverterLearner : public BaseFeatureConverterLearner<TElement> {
 public:
  typedef boost::shared_ptr<FeatureConverterLearner> Ptr;
  typename TFeatureConverter::Ptr makeSpecific() const {
    return TFeatureConverter::Ptr(new TFeatureConverter(feature_converter_));
  }
  virtual FeatureConverter::Ptr make() const {
    return FeatureConverter::Ptr(makeSpecific());
  }
  virtual void learn(const DataSet<TElement>& data_set) {
    feature_converter_.set_input_feature_info(data_set.featureInfo());
    learnImpl(data_set, &feature_converter_);
    feature_converter_.fillOutputFeatureInfo();
  }
 private:
  virtual void learnImpl(const DataSet<TElement>& data_set, 
                         TFeatureConverter* feature_converter) = 0;
  TFeatureConverter feature_converter_;
};
};

#endif  // LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_
