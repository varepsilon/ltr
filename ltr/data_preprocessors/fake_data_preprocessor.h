// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_FAKE_DATA_PREPROCESSOR_H_
#define LTR_DATA_PREPROCESSORS_FAKE_DATA_PREPROCESSOR_H_

#include <string>

#include "ltr/utility/boost/shared_ptr.h"  //NOLINT

#include "ltr/data/data_set.h"
#include "ltr/data_preprocessors/data_preprocessor.h"

using std::string;

namespace ltr {
/**
* \brief Does nothing with input DataSet.
*/
template <class TElement>
class FakePreprocessor : public DataPreprocessor<TElement> {
  ALLOW_SHARED_PTR_ONLY_CREATION(FakePreprocessor)
 public:
  explicit FakePreprocessor(const ParametersContainer& parameters) {
  }

  FakePreprocessor() {
  }

  virtual string toString() const {
    return this->alias();
  }
 private:
  virtual void applyImpl(const DataSet<TElement>& input,
                               DataSet<TElement>* output) const {
    *output = input;
  }
  virtual string getDefaultAlias() const {return "FakePreprocessor";}
};
};

#endif  // LTR_DATA_PREPROCESSORS_FAKE_DATA_PREPROCESSOR_H_
