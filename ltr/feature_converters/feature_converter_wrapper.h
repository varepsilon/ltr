// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_WRAPPER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_WRAPPER_H_

#include <string>

#include "ltr/feature_converters/feature_converter_learner.h"

using std::string;

namespace ltr {
template <class TElement, class TFeatureConverter>
class FeatureConverterWrapper
    : public FeatureConverterLearner<TElement, TFeatureConverter> {
  public:
    typedef boost::shared_ptr<FeatureConverterWrapper> Ptr;

    virtual void learn(const DataSet<TElement>& data_set) {
      feature_info_ = data_set.feature_info();
    }

    virtual typename TFeatureConverter::Ptr makeSpecific() const {
      return typename TFeatureConverter::Ptr(
          new TFeatureConverter(feature_info_));
    }

    void setDefaultParameters() {}
    void checkParameters() const {}
    string toString() const {
      string s = "Feature converter learner (wrapper)";
      return s;
    }
  private:
    FeatureInfo feature_info_;
};
}

#endif  // LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_WRAPPER_H_
