// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_NAN_TO_ZERO_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_NAN_TO_ZERO_LEARNER_H_

#include <string>

#include "ltr/feature_converters/nan_to_neutral_converter.h"
#include "ltr/feature_converters/feature_converter_learner.h"

namespace ltr {
/**
 * \brief Produces NanToNeutralConverter with zero neutral object
 */
template <typename TElement>
class NanToZeroConverterLearner
  : public BaseFeatureConverterLearner<TElement, NanToNeutralConverter> {
 public:
  explicit NanToZeroConverterLearner(const ParametersContainer& parameters) {
  }

  NanToZeroConverterLearner() {
  }

  virtual void learnImpl(const DataSet<TElement>& data_set,
                         NanToNeutralConverter* feature_converter);

  virtual string toString() const;

 private:
  virtual string getDefaultAlias() const;
};

template <typename TElement>
void NanToZeroConverterLearner<TElement>::learnImpl(
  const DataSet<TElement>& data_set, NanToNeutralConverter* feature_converter) {
    Object zero_features_object;
    zero_features_object.features().assign(data_set.feature_count(), 0.0);
    feature_converter->set_neutral_object(zero_features_object);
}

template <typename TElement>
string NanToZeroConverterLearner<TElement>::toString() const {
  return "NanToZeroConverterLearner";
}

template <typename TElement>
string NanToZeroConverterLearner<TElement>::getDefaultAlias() const {
  return "NanToZeroConverterLearner";
}
};
#endif  // LTR_FEATURE_CONVERTERS_NAN_TO_ZERO_LEARNER_H_
