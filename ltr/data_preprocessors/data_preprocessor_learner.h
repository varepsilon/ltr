// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_LEARNER_H_
#define LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include "ltr/data/data_set.h"
#include "ltr/data_preprocessors/data_preprocessor.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/interfaces/parameterized.h"

namespace ltr {

template <typename TElement>
class IDataPreprocessorLearner : public Parameterized {
  public:
  virtual void learn(const DataSet<TElement>& data_set) = 0;
  virtual typename DataPreprocessor<TElement>::Ptr make() const = 0;
};
}

#endif  // LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_LEARNER_H_
