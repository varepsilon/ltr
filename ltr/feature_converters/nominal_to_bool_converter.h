// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_NOMINAL_TO_BOOL_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_NOMINAL_TO_BOOL_CONVERTER_H_

#include <string>

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/feature_converter_learner.h"

namespace ltr {
class NominalToBoolConverter : public FeatureConverter {
 public:
  typedef boost::shared_ptr<NominalToBoolConverter> Ptr;

  NominalToBoolConverter(const FeatureInfo& feature_info = FeatureInfo())
      : FeatureConverter(feature_info) {
    fillOutputFeatureInfo();
  }
  virtual void fillOutputFeatureInfo();
  string generateCppCode(const std::string &) const;
 private:
  virtual void applyImpl(const Object& argument, Object* value) const;
};

template <typename TElement>
class NominalToBoolConverterLearner
    : public FeatureConverterLearner<TElement, NominalToBoolConverter> {
 public:
  virtual void learnImpl(const DataSet<TElement>& data_set, 
                         NominalToBoolConverter* feature_converter) {
    // DO NOTHING                      
  }
  virtual string toString() const {
    return "NominalToBoolConverterLearner";
  }
};
}

#endif  // LTR_FEATURE_CONVERTERS_NOMINAL_TO_BOOL_CONVERTER_H_

