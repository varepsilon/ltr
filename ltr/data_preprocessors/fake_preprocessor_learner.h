// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_FAKE_PREPROCESSOR_LEARNER_H_
#define LTR_DATA_PREPROCESSORS_FAKE_PREPROCESSOR_LEARNER_H_


#include "ltr/data_preprocessors/data_preprocessor_learner.h"
#include "ltr/data_preprocessors/fake_data_preprocessor.h"

using ltr::DataPreprocessorLearner;
using ltr::FakeDataPreprocessor;

namespace ltr {
/**
 * Always produces fake data preprocessor
 */
template <typename TElement>
class FakePreprocessorLearner
    : public DataPreprocessorLearner<TElement, FakeDataPreprocessor> {
  public:
  typedef boost::shared_ptr<FakePreprocessorLearner> Ptr;

  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit FakePreprocessorLearner(
      const ParametersContainer& parameters = ParametersContainer()) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
    this->checkParameters();
  }

  void learn(const DataSet<TElement>& data_set) {}
  FakeDataPreprocessor<TElement> make() const;

  private:
  FakeDataPreprocessor<TElement> preprocessor_;
};

// template realizations
template <typename TElement>
FakeDataPreprocessor<TElement>
    FakePreprocessorLearner<TElement>::make() const {
  return preprocessor_;
}
};

#endif  // LTR_DATA_PREPROCESSORS_FAKE_PREPROCESSOR_LEARNER_H_
