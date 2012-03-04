// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include "ltr/data/data_set.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/interfaces/parameterized.h"

namespace ltr {
/**
 * An object which learns on inputted dataset and produces
 * a FeatureConverter. May produce different converters (using random)
 * for one inputted dataset as in RSM (random subset method).
 */
template <class TElement>
class FeatureConverterLearner : public Parameterized {
  public:
  typedef boost::shared_ptr<FeatureConverterLearner> Ptr;

  /**
   * Learns from inputted dataset. E. g. remembers number of features
   * in dataset and e.t.c.
   */
  virtual void learn(const DataSet<TElement>& data_set) = 0;
  /**
   * Produces a FeatureConverter. Must have been learned before calling make()
   */
  virtual FeatureConverter::Ptr make() const = 0;
};
}

#endif  // LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_
