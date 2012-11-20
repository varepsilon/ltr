// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_LEARNER_H_

#include <string>

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/fake_feature_converter.h"

#include "ltr/utility/boost/shared_ptr.h"

using std::string;

namespace ltr {
/**
 * Produces FakeFeatureConverter
 */
template <class TElement>
class FakeFeatureConverterLearner
: public BaseFeatureConverterLearner<TElement, FakeFeatureConverter> {
 public:
  typedef ltr::utility::shared_ptr<FakeFeatureConverterLearner> Ptr;

  virtual string toString() const;

  explicit FakeFeatureConverterLearner(const ParametersContainer& parameters) {
  }

  FakeFeatureConverterLearner() {
  }

 private:
  virtual void learnImpl(const DataSet<TElement>& data_set,
                         FakeFeatureConverter *feature_converter);

  virtual string getDefaultAlias() const;
};

// template realizations

template <class TElement>
string FakeFeatureConverterLearner<TElement>::toString() const {
  return "FakeFeatureConverterLearner";
}

template <class TElement>
void FakeFeatureConverterLearner<TElement>::learnImpl(
  const DataSet<TElement>& data_set,
  FakeFeatureConverter* feature_converter) {
    // DO NOTHING
}

template <class TElement>
string FakeFeatureConverterLearner<TElement>::getDefaultAlias() const {
  return "FakeFeatureConverterLearner";
}
};
#endif  // LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_LEARNER_H_
