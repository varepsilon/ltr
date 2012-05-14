// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_FAKE_PREPROCESSOR_H_
#define LTR_DATA_PREPROCESSORS_FAKE_PREPROCESSOR_H_

#include <boost/shared_ptr.hpp>
#include <string>

#include "ltr/data_preprocessors/data_preprocessor.h"
#include "ltr/data/data_set.h"

using std::string;

namespace ltr {
  /**
   * Fake data preprocessor. Does nothing with inputted dataset
   */
  template <class TElement>
  class FakePreprocessor : public DataPreprocessor<TElement> {
  public:
    typedef boost::shared_ptr<FakePreprocessor> Ptr;

    explicit FakePreprocessor(
        const ParametersContainer& parameters = ParametersContainer())
        : DataPreprocessor<TElement>("FakePreprocessor") {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }

    void apply(const DataSet<TElement>& input_dataset,
        DataSet<TElement>* output_dataset) const {
      *output_dataset = input_dataset;
    }

    string toString() const {
      return "Fake data preprocessor";
    }
  };
};

#endif  // LTR_DATA_PREPROCESSORS_FAKE_PREPROCESSOR_H_
