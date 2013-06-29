// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_REMOVE_NOMINAL_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_REMOVE_NOMINAL_LEARNER_H_

#include <string>

#include "ltr/feature_converters/remove_nominal_converter.h"
#include "ltr/feature_converters/feature_converter_learner.h"

#include "ltr/parameters_container/parameters_container.h"

using std::string;

using ltr::ParametersContainer;

namespace ltr {
/**
 * \brief Produces RemoveNominalConverter
 */
template <typename TElement>
class RemoveNominalConverterLearner
  : public BaseFeatureConverterLearner<TElement, RemoveNominalConverter> {
  ALLOW_SHARED_PTR_ONLY_CREATION(RemoveNominalConverterLearner)
 public:
  explicit RemoveNominalConverterLearner(
    const ParametersContainer& parameters) {
  }

  RemoveNominalConverterLearner() {
  }

  virtual void learnImpl(const DataSet<TElement>& data_set,
                         RemoveNominalConverter::Ptr* feature_converter);

  virtual string toString() const;

 private:
  virtual string getDefaultAlias() const;
};

template <typename TElement>
void RemoveNominalConverterLearner<TElement>::learnImpl(
  const DataSet<TElement>& data_set,
  RemoveNominalConverter::Ptr* feature_converter) {
    // DO NOTHING
}

template <typename TElement>
string RemoveNominalConverterLearner<TElement>::toString() const {
  return "RemoveNominalConverterLearner";
}

template <typename TElement>
string RemoveNominalConverterLearner<TElement>::getDefaultAlias() const {
  return "RemoveNominalConverterLearner";
}
};
#endif  // LTR_FEATURE_CONVERTERS_REMOVE_NOMINAL_LEARNER_H_
