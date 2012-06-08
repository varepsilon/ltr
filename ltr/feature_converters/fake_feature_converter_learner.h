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

  void learn(const DataSet<TElement>& data_set);
  virtual FakeFeatureConverter::Ptr makeSpecific() const;
  virtual string toString() const;
 private:
  FeatureInfo feature_info_;
};

// template realizations

template <class TElement>
FakeFeatureConverter::Ptr
FakeFeatureConverterLearner<TElement>::makeSpecific() const {
  return FakeFeatureConverter::Ptr(new FakeFeatureConverter(feature_info_));
}

template <class TElement>
void FakeFeatureConverterLearner<TElement>::learn(
  const DataSet<TElement>& data_set) {
  feature_info_ = data_set.feature_info();
}

template <class TElement>
string FakeFeatureConverterLearner<TElement>::toString() const {
  return "FakeFeatureConverterLearner";
}
};
#endif  // LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_LEARNER_H_
