// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_REMOVE_NOMINAL_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_REMOVE_NOMINAL_CONVERTER_H_

#include <string>

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/feature_converter_learner.h"

namespace ltr {
class RemoveNominalConverter : public FeatureConverter {
 public:
  typedef boost::shared_ptr<RemoveNominalConverter> Ptr;

  RemoveNominalConverter(const FeatureInfo& feature_info = FeatureInfo())
      : FeatureConverter(feature_info) {
    fillOutputFeatureInfo();
  }
  string generateCppCode(const std::string &) const;
  virtual void fillOutputFeatureInfo();
 private:
  virtual void applyImpl(const Object& argument, Object* value) const;
};

template <typename TElement>
class RemoveNominalConverterLearner
    : public FeatureConverterLearner<TElement, RemoveNominalConverter> {
 public:
  virtual void learnImpl(const DataSet<TElement>& data_set, 
                         RemoveNominalConverter* feature_converter) {
    // DO NOTHING                      
  }
  virtual string toString() const {
    return "RemoveNominalConverterLearner";
  }
};
}

#endif  // LTR_FEATURE_CONVERTERS_REMOVE_NOMINAL_CONVERTER_H_

