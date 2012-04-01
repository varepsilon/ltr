// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_WRAPPER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_WRAPPER_H_

#include "ltr/feature_converters/feature_converter_learner.h"

namespace ltr {
template <class TElement>
class FeatureConverterWrapper : public BaseFeatureConverterLearner<TElement> {
  private:
    FeatureConverter::Ptr converter_;

  public:
    typedef boost::shared_ptr<FeatureConverterWrapper> Ptr;

    explicit FeatureConverterWrapper(FeatureConverter::Ptr converter) :
      converter_(converter) {}

    void setConverter(FeatureConverter::Ptr converter) {
      converter_ = converter;
    }

    virtual void learn(const DataSet<TElement>& data_set) {
      converter_->setFeatureInfo(data_set.featureInfo());
    }

    FeatureConverter::Ptr makePtr() const {
      return converter_;
    }

    void setDefaultParameters() {}
    void checkParameters() const {}
};
}

#endif  // LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_WRAPPER_H_
