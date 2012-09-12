// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_NOMINAL_TO_BOOL_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_NOMINAL_TO_BOOL_LEARNER_H_

#include <string>

#include "ltr/feature_converters/nominal_to_bool_converter.h"
#include "ltr/feature_converters/feature_converter_learner.h"

namespace ltr {
/**
 * Produces NominalToBoolConverter
 */
template <typename TElement>
class NominalToBoolConverterLearner
  : public BaseFeatureConverterLearner<TElement, NominalToBoolConverter> {
 public:
  explicit NominalToBoolConverterLearner(
    const ParametersContainer& parameters) {
  }

  NominalToBoolConverterLearner() {
  }

  virtual void learnImpl(const DataSet<TElement>& data_set,
                         NominalToBoolConverter* feature_converter);

  virtual string toString() const;

 private:
  virtual string getDefaultAlias() const;
};

template <typename TElement>
void NominalToBoolConverterLearner<TElement>::learnImpl(
  const DataSet<TElement>& data_set,
  NominalToBoolConverter* feature_converter) {
    // DO NOTHING
}

template <typename TElement>
string NominalToBoolConverterLearner<TElement>::toString() const {
  return "NominalToBoolConverterLearner";
}

template <typename TElement>
string NominalToBoolConverterLearner<TElement>::getDefaultAlias() const {
  return "NominalToBoolConverterLearner";
}
};
#endif  // LTR_FEATURE_CONVERTERS_NOMINAL_TO_BOOL_LEARNER_H_
