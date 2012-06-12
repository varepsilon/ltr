// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_DATA_SAMPLER_H_
#define LTR_DATA_PREPROCESSORS_DATA_SAMPLER_H_

#include <algorithm>
#include <vector>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

#include <boost/lexical_cast.hpp>  //NOLINT
#include <boost/shared_ptr.hpp>  //NOLINT

#include "ltr/data/data_set.h"
#include "ltr/data_preprocessors/data_preprocessor.h"
#include "ltr/utility/indices.h"

using std::set;
using std::string;
using std::vector;
using std::logic_error;
using std::max_element;

using ltr::utility::lightSubset;
using ltr::utility::Indices;
using ltr::utility::IndicesPtr;

namespace ltr {
/**
* \brief Samples elements with specififed indices from input DataSet.
* Duplication of indices leads to duplication of elements in the result sample.
*/
template <class TElement>
class DataSampler : public DataPreprocessor<TElement> {
 public:
  typedef boost::shared_ptr<DataSampler> Ptr;

  /**
  * \param indices indices of elements to sample,
  * duplication of indices leads to duplication of elements in the result sample
  */
  explicit DataSampler(IndicesPtr indices = IndicesPtr(new Indices));
  explicit DataSampler(const ParametersContainer& parameters);

  virtual void setDefaultParameters();
  virtual void checkParameters() const;

  virtual string toString() const;

  GET_SET(IndicesPtr, indices);
 private:
  virtual void applyImpl(const DataSet<TElement>& input,
                               DataSet<TElement>* output) const;
  virtual void setParametersImpl(const ParametersContainer& parameters);
  IndicesPtr indices_;
};

// template realizations

template <typename TElement>
DataSampler<TElement>::DataSampler(IndicesPtr indices) {
  set_indices(indices);
}
template <typename TElement>
DataSampler<TElement>::DataSampler(const ParametersContainer& parameters) {
  setParameters(parameters);
}

template <typename TElement>
void DataSampler<TElement>::setDefaultParameters() {
  indices_ = IndicesPtr(new Indices);
}

template <typename TElement>
void DataSampler<TElement>::checkParameters() const {
  // DON'T NEED
}

template <typename TElement>
void DataSampler<TElement>::setParametersImpl(
    const ParametersContainer& parameters) {
  // \TODO(sameg) Create indices from string representation like [1:3], 5, 6
  indices_ = parameters.Get<IndicesPtr>("INDICES");
}

template <class TElement>
void DataSampler<TElement>::applyImpl(const DataSet<TElement>& input,
    DataSet<TElement>* output) const {
  // \TODO(sameg) Is it logic?
  if (indices_->size() != 0) {
    *output = lightSubset(input, *indices_);
  } else {
    *output = input;
  }
}

template <typename TElement>
string DataSampler<TElement>::toString() const {
  std::stringstream str;
  str << "DataSampler: indices = [";
  for (int i = 0; i < indices_->size(); ++i) {
    if (i != 0) {
      str << ", ";
    }
    str << indices_->at(i);
  }
  str << "]";
  return str.str();
}
};

#endif  // LTR_DATA_PREPROCESSORS_DATA_SAMPLER_H_
