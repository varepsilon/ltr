// Copyright 2012 Yandex

#ifndef LTR_DATA_PREPROCESSORS_DATA_SAMPLER_H_
#define LTR_DATA_PREPROCESSORS_DATA_SAMPLER_H_

#include <algorithm>
#include <vector>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

#include "ltr/utility/boost/shared_ptr.h"  //NOLINT

#include "ltr/data/data_set.h"
#include "ltr/data_preprocessors/data_preprocessor.h"
#include "ltr/utility/indices.h"

using std::set;
using std::string;
using std::vector;
using std::logic_error;
using std::max_element;

using ltr::utility::Indices;
using ltr::utility::IndicesPtr;

namespace ltr {
/**
* \brief Samples elements with specififed indices from input DataSet.
* Duplication of indices leads to duplication of elements in the result sample.
*/
template <class TElement>
class DataSampler : public DataPreprocessor<TElement> {
  ALLOW_SHARED_PTR_ONLY_CREATION(DataSampler)
 public:
  /**
  * \param indices indices of elements to sample,
  * duplication of indices leads to duplication of elements in the result sample
  */
  explicit DataSampler(Indices indices = Indices());
  explicit DataSampler(const ParametersContainer& parameters);

  virtual void setDefaultParameters();
  virtual void checkParameters() const;

  virtual string toString() const;

  GET_SET_VECTOR(int, indices);
 private:
  virtual void applyImpl(const DataSet<TElement>& input,
                               DataSet<TElement>* output) const;
  virtual void setParametersImpl(const ParametersContainer& parameters);
  virtual string getDefaultAlias() const {return "DataSampler";}
  Indices indices_;
};

// template realizations

template <typename TElement>
DataSampler<TElement>::DataSampler(Indices indices) {
  set_indices(indices);
}
template <typename TElement>
DataSampler<TElement>::DataSampler(const ParametersContainer& parameters) {
  this->setParameters(parameters);
}

template <typename TElement>
void DataSampler<TElement>::setDefaultParameters() {
  indices_ = Indices();
}

template <typename TElement>
void DataSampler<TElement>::checkParameters() const {
  // \TODO(freopen) Check indices to be in [0, data_set.size()-1] range
}

template <typename TElement>
void DataSampler<TElement>::setParametersImpl(
    const ParametersContainer& parameters) {
  // \TODO(sameg) Create indices from string representation like [1:3], 5, 6
  indices_ = parameters.Get<Indices>("INDICES");
}

template <class TElement>
void DataSampler<TElement>::applyImpl(const DataSet<TElement>& input,
    DataSet<TElement>* output) const {
  // \TODO(sameg) Is it logic?
  if (indices_.size() != 0) {
    *output = input.lightSubset(indices_);
  } else {
    *output = input;
  }
}

template <typename TElement>
string DataSampler<TElement>::toString() const {
  std::stringstream str;
  str << "DataSampler: indices = [";
  for (int index = 0; index < (int)indices_.size(); ++index) {
    if (index != 0) {
      str << ", ";
    }
    str << indices_.at(index);
  }
  str << "]";
  return str.str();
}
};

#endif  // LTR_DATA_PREPROCESSORS_DATA_SAMPLER_H_
