// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_NAN_TO_ZERO_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_NAN_TO_ZERO_CONVERTER_H_

#include <string>

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/feature_converter_learner.h"

namespace ltr {
/**
 * Converts NaN features to zero values.
 */
class NanToZeroConverter : public FeatureConverter {
 public:
  typedef ltr::utility::shared_ptr<NanToZeroConverter> Ptr;

  NanToZeroConverter(const FeatureInfo& feature_info = FeatureInfo())
    : FeatureConverter(feature_info) {}

  virtual FeatureInfo convertFeatureInfo() const;

  virtual string generateCppCode(const string& function_name) const;

 private:
  virtual void applyImpl(const Object& input, Object* output) const;

  virtual string getDefaultAlias() const;
};

template <typename TElement>
class NanToZeroConverterLearner
  : public BaseFeatureConverterLearner<TElement, NanToZeroConverter> {
 public:
  virtual void learnImpl(const DataSet<TElement>& data_set,
                         NanToZeroConverter* feature_converter) {
    // DO NOTHING
  }

  virtual string toString() const {
    return "NanToZeroConverterLearner";
  }

 private:
  virtual string getDefaultAlias() const {
    return "NanToZeroConverterLearner";
  }
};
};

#endif  // LTR_FEATURE_CONVERTERS_NAN_TO_ZERO_CONVERTER_H_
