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
 * Always produces fake feature converter
 */
template <typename TElement>
class FakeFeatureConverterLearner
    : public FeatureConverterLearner<TElement, FakeFeatureConverter> {
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

  void learn(const DataSet<TElement>& data_set) {
    converter_.setFeatureInfo(data_set.featureInfo());
  }
  FakeFeatureConverter make() const;
  string toString() const;
  private:
  FakeFeatureConverter converter_;
};

// template realizations
template <typename TElement>
FakeFeatureConverter FakeFeatureConverterLearner<TElement>::make() const {
  return converter_;
}

template <typename TElement>
string FakeFeatureConverterLearner<TElement>::toString() const {
  return "Fake feature converter learner";
}
};
#endif  // LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_LEARNER_H_
