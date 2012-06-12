// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_LEARNER_H_

#include <boost/shared_ptr.hpp>
#include <string>

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/fake_feature_converter.h"

using std::string;

namespace ltr {
/**
 * Produces FakeFeatureConverter
 */
template <class TElement>
class FakeFeatureConverterLearner
: public BaseFeatureConverterLearner<TElement, FakeFeatureConverter> {
 public:
  typedef boost::shared_ptr<FakeFeatureConverterLearner> Ptr;
  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit FakeFeatureConverterLearner(const ParametersContainer& parameters =
      ParametersContainer()) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
    this->checkParameters();
  }

  virtual string toString() const;
 private:
  virtual void learnImpl(const DataSet<TElement>& data_set,
                         FakeFeatureConverter *feature_converter);
};

// template realizations

template <class TElement>
void FakeFeatureConverterLearner<TElement>::learnImpl(
    const DataSet<TElement>& data_set,
    FakeFeatureConverter* feature_converter) {
  // DO NOTHING
}

template <class TElement>
string FakeFeatureConverterLearner<TElement>::toString() const {
  return "FakeFeatureConverterLearner";
}
};
#endif  // LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_LEARNER_H_
