// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_SIMPLE_SUBSET_PREPROCESSOR_H_
#define LTR_DATA_PREPROCESSORS_SIMPLE_SUBSET_PREPROCESSOR_H_

#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "ltr/data/data_set.h"
#include "ltr/data_preprocessors/data_preprocessor.h"

using std::max_element;
using std::vector;
using std::logic_error;
using std::string;
using ltr::DataSet;
using ltr::DataPreprocessor;
using ltr::utility::lightSubset;

namespace ltr {
  template <class TElement>
  class SimpleSubsetPreprocessor : public DataPreprocessor<TElement> {
  public:
    typedef boost::shared_ptr<SimpleSubsetPreprocessor> Ptr;

    explicit SimpleSubsetPreprocessor(const vector<size_t>& input_indices)
        : indices_(input_indices) {
      max_used_element_ = *max_element(indices_.begin(), indices_.end());
    }

    void setChoosedElementsIndices(const vector<size_t>& input_indices) {
      indices_ = input_indices;
      max_used_element_ = *max_element(indices_.begin(), indices_.end());
    }
    const vector<size_t>& getChoosedElementsIndices() const {
      return indices_;
    }
    size_t getChoosedElementsCount() const {
      return indices_.size();
    }

    void apply(const DataSet<TElement>& input_dataset,
      DataSet<TElement>* output_dataset) const;

  private:
    vector<size_t> indices_;
    int max_used_element_;
  };

  // template realizations
  template <class TElement>
  void SimpleSubsetPreprocessor<TElement>::apply(
      const DataSet<TElement>& input_dataset,
      DataSet<TElement>* output_dataset) const {
    if (max_used_element_ > input_dataset.size()) {
      throw logic_error("Current dataset has "
      + boost::lexical_cast<string>(input_dataset.size())
      + " elements while number "
      + boost::lexical_cast<string>(max_used_element_)
      + " was requested");
    } else {
      *output_dataset = lightSubset(input_dataset, indices_);
    }
  }
};

#endif  // LTR_DATA_PREPROCESSORS_SIMPLE_SUBSET_PREPROCESSOR_H_
