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
* Samples elements with specififed indices from input DataSet.
* Duplication of indices leads to duplication of elements in the result sample.
*/
template <class TElement>
class DataSampler : public DataPreprocessor<TElement> {
 public:
  typedef boost::shared_ptr<DataSampler> Ptr;

  explicit DataSampler() : DataPreprocessor<TElement>("DataSampler") {
    this->setDefaultParameters();
  }
  explicit DataSampler(const ParametersContainer& parameters)
      : DataPreprocessor<TElement>("DataSampler") {
    this->setParameters(parameters);
  }

  virtual void setDefaultParameters();
  virtual void setParametersImpl(const ParametersContainer& parameters);
  virtual void checkParameters() const;

  virtual void apply(const DataSet<TElement>& old_dataset,
                           DataSet<TElement>* new_dataset) const;
  virtual string toString() const;

  MAKE_GET_SET(IndicesPtr, indices);
 private:
  IndicesPtr indices_;
};

// template realizations
template <class TElement>
void DataSampler<TElement>::apply(const DataSet<TElement>& old_dataset,
                                        DataSet<TElement>* new_dataset) const {
  if (indices_->size() != 0) {
    *new_dataset = lightSubset(old_dataset, *indices_);
  } else {
    *new_dataset = old_dataset;
  }
}

template <typename TElement>
string DataSampler<TElement>::toString() const {
  std::stringstream str;
  str << "Subset data preprocessor with parameter INDICES = {";
  for (int i = 0; i < indices_->size(); ++i) {
    if (i != 0) {
      str << ", ";
    }
    str << indices_->at(i);
  }
  str << "}";
  return str.str();
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
};

#endif  // LTR_DATA_PREPROCESSORS_DATA_SAMPLER_H_
