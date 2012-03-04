// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/fake_feature_converter.h"

namespace ltr {
/**
 * Always produces fake feature converter
 */
template <typename TElement>
class FakeFeatureConverterLearner : public FeatureConverterLearner<TElement> {
  public:
  typedef boost::shared_ptr<FakeFeatureConverterLearner> Ptr;

  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit FakeFeatureConverterLearner(const ParametersContainer& parameters =
      ParametersContainer())
      : converter_(new FakeFeatureConverter) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
    this->checkParameters();
  }

  void learn(const DataSet<TElement>& data_set) {}
  FeatureConverter::Ptr make() const;

  private:
  FakeFeatureConverter::Ptr converter_;
};

// template realizations
template <typename TElement>
FeatureConverter::Ptr FakeFeatureConverterLearner<TElement>::make() const {
  FakeFeatureConverter::Ptr output(new FakeFeatureConverter(*converter_));
  return output;
}
};
#endif  // LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_LEARNER_H_
