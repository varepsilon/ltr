// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_FAKE_DATA_PREPROCESSOR_H_
#define LTR_DATA_PREPROCESSORS_FAKE_DATA_PREPROCESSOR_H_

#include <boost/shared_ptr.hpp>

#include "ltr/data_preprocessors/data_preprocessor.h"
#include "ltr/data/data_set.h"

using ltr::DataSet;
using ltr::DataPreprocessor;

namespace ltr {
  template <class TElement>
  class FakeDataPreprocessor : public DataPreprocessor<TElement> {
  public:
    typedef boost::shared_ptr<FakeDataPreprocessor> Ptr;

    FakeDataPreprocessor() {}

    void apply(const DataSet<TElement>& input_dataset,
        DataSet<TElement>* output_dataset) const {
      *output_dataset = input_dataset;
    }
  };
};

#endif  // LTR_DATA_PREPROCESSORS_FAKE_DATA_PREPROCESSOR_H_
