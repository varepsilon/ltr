// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_H_
#define LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_H_

#include <boost/shared_ptr.hpp>

#include "ltr/data/data_set.h"

using ltr::DataSet;

namespace ltr {
  template <class TElement>
  class DataPreprocessor {
  public:
    typedef boost::shared_ptr<DataPreprocessor> Ptr;

    virtual ~DataPreprocessor() {}

    virtual void apply(const DataSet<TElement>& input,
      DataSet<TElement>* output) const = 0;
  };
};

#endif  // LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_H_
