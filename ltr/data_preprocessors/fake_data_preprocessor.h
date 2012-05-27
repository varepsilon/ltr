// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_FAKE_DATA_PREPROCESSOR_H_
#define LTR_DATA_PREPROCESSORS_FAKE_DATA_PREPROCESSOR_H_

#include <string>

#include <boost/shared_ptr.hpp>  //NOLINT

#include "ltr/data/data_set.h"
#include "ltr/data_preprocessors/data_preprocessor.h"

using std::string;

namespace ltr {
/**
* Does nothing with input DataSet.
*/
template <class TElement>
class FakePreprocessor : public DataPreprocessor<TElement> {
 public:
  typedef boost::shared_ptr<FakePreprocessor> Ptr;
  explicit FakePreprocessor()
    : DataPreprocessor<TElement>("FakeDataPreprocessor") {}
  virtual void apply(const DataSet<TElement>& old_dataset,
                           DataSet<TElement>* new_dataset) const {
    *new_dataset = old_dataset;
  }
  virtual string toString() const {
    return "Fake data preprocessor";
  }
};
};

#endif  // LTR_DATA_PREPROCESSORS_FAKE_DATA_PREPROCESSOR_H_
