// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include "ltr/data/data_set.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/interfaces/parameterized.h"

namespace ltr {

template < typename TElement >
class IFeatureConverterLearner : public Parameterized {
  public:
  virtual void learn(const DataSet<TElement>& data_set) = 0;
  virtual FeatureConverter::Ptr make() const = 0;
};
}

#endif  // LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_
