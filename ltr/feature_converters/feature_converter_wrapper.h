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
  private:
    FeatureInfo feature_info_;

  public:
    typedef boost::shared_ptr<FeatureConverterWrapper> Ptr;

    virtual void learn(const DataSet<TElement>& data_set) {
      feature_info_ = data_set.feature_info();
    }

    virtual TFeatureConverter make() const {
      return TFeatureConverter(feature_info_);
    }

    void setDefaultParameters() {}
    void checkParameters() const {}
    string toString() const {
      string s = "Feature converter learner (wrapper) for ";
      s.append(TFeatureConverter().alias());
      return s;
    }
};
}

#endif  // LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_WRAPPER_H_
