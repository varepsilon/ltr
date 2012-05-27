// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_DATA_RANDOM_SAMPLER_H_
#define LTR_DATA_PREPROCESSORS_DATA_RANDOM_SAMPLER_H_

#include <algorithm>
#include <ctime>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include <functional>

#include "boost/random.hpp"  //NOLINT
#include "boost/random/random_number_generator.hpp"  //NOLINT
#include "boost/shared_ptr.hpp"  //NOLINT

#include "ltr/data_preprocessors/data_preprocessor.h"
#include "ltr/utility/indices.h"

using std::vector;
using std::string;
using std::vector;

using ltr::utility::lightSubset;
using ltr::utility::Indices;
using ltr::utility::getRandomIndices;

namespace ltr {
/**
 * Acts as DataSampler with random indices each new applying
 * (duplication allowed)
 */
template <typename TElement>
class DataRandomSampler : public DataPreprocessor<TElement> {
  public:
  typedef boost::shared_ptr<DataRandomSampler> Ptr;

  explicit DataRandomSampler()
      : DataPreprocessor<TElement>("DataRandomSampler") {
    this->setDefaultParameters();
  }
  explicit DataRandomSampler(const ParametersContainer& parameters)
      : DataPreprocessor<TElement>("DataRandomSampler") {
    this->setParameters(parameters);
  }

  virtual void setDefaultParameters();
  virtual void setParametersImpl(const ParametersContainer& parameters);
  virtual void checkParameters() const;
  virtual void apply(const DataSet<TElement>& old_dataset,
                           DataSet<TElement>* new_dataset) const;
  virtual string toString() const;

  MAKE_GET_SET(double, sampling_fraction);
  MAKE_GET_SET(bool, with_replacement);
  MAKE_GET(int, seed);
  void set_seed(int seed);
 private:
  double sampling_fraction_;
  bool with_replacement_;
  int seed_;
  // \TODO(sameg) May be make global random generator object?
  mutable boost::mt19937 generator_;
};

// template realizations
template <typename TElement>
string DataRandomSampler<TElement>::toString() const {
  std::stringstream str;
  std::fixed(str);
  str.precision(2);
  str << "Begging data preprocessor with parameters: sampling_fraction = ";
  str << sampling_fraction_ << ", with_replacment = " << with_replacement_;
  str << ", seed = " << seed_;
  return str.str();
}

template <typename TElement>
void DataRandomSampler<TElement>::setDefaultParameters() {
  sampling_fraction_ = 0.3;
  with_replacement_ = true;
  set_seed(42);
}

template <typename TElement>
void DataRandomSampler<TElement>::checkParameters() const {
  if (with_replacement_) {
    CHECK(sampling_fraction_ > 0.);
  } else {
    CHECK(sampling_fraction_ > 0. && sampling_fraction_ <= 1.);
  }
  CHECK(seed_ >= 0); //NOLINT
}

template <typename TElement>
void DataRandomSampler<TElement>::apply(const DataSet<TElement>& old_dataset,
    DataSet<TElement>* new_dataset) const {
  int sample_size = ceilf(old_dataset.size() * sampling_fraction_);
  Indices indices(sample_size);
  if (with_replacement_) {
    boost::random_number_generator<boost::mt19937> random(generator_);
    for (int i = 0; i < indices.size(); ++i) {
      indices[i] = random(old_dataset.size());
    }
  } else {
    // TODO(sameg) Not thread safe.
    // Lets think about using custom random numbers generator
    getRandomIndices(&indices, old_dataset.size(), sample_size);
  }
  *new_dataset = lightSubset(old_dataset, indices);
}

template <typename TElement>
void DataRandomSampler<TElement>::set_seed(int seed) {
  CHECK(seed >= 0);  //NOLINT
  seed_ = seed;
  srand(seed_);
  generator_.seed(seed_);
}

template <typename TElement>
void DataRandomSampler<TElement>::setParametersImpl(
    const ParametersContainer& parameters) {
  with_replacement_ = parameters.Get<bool>("WITH_REPLACMENT");
  sampling_fraction_ = parameters.Get<double>("SAMPLING_FRACTION");
  set_seed(parameters.Get<int>("SEED"));
}
};

#endif  // LTR_DATA_PREPROCESSORS_DATA_RANDOM_SAMPLER_H_
