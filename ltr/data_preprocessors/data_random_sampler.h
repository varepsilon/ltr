// Copyright 2012 Yandex

#ifndef LTR_DATA_PREPROCESSORS_DATA_RANDOM_SAMPLER_H_
#define LTR_DATA_PREPROCESSORS_DATA_RANDOM_SAMPLER_H_

#include <algorithm>
#include <ctime>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include <functional>

#include "ltr/data_preprocessors/data_preprocessor.h"
#include "ltr/utility/indices.h"
#include "ltr/utility/random_number_generator.h"

using std::vector;
using std::string;
using std::vector;

using ltr::utility::Indices;
using ltr::utility::getRandomIndices;
using ltr::utility::randomizer;

namespace ltr {
/**
 * \brief Acts as DataSampler with random indices each new applying
 * (duplication may be allowed).
 */
template <class TElement>
class DataRandomSampler : public DataPreprocessor<TElement> {
  ALLOW_SHARED_PTR_ONLY_CREATION(DataRandomSampler)
  /**
  * \param sampling_fraction ratio of chosen objects count to dataset size,
  * may be more than 1.0 in case of sampling with replacement
  * \param with_replacement determines whether duplication of objects is allowed 
  * \param seed seed for random numbers generator 
  */
 public:
  explicit DataRandomSampler(double sampling_fraction = 0.3,
                             bool with_replacement = true,
                             int seed = 42);
  explicit DataRandomSampler(const ParametersContainer& parameters);

  virtual void setDefaultParameters();
  virtual void checkParameters() const;

  virtual string toString() const;

  GET_SET(double, sampling_fraction);
  GET_SET(bool, with_replacement);
  GET(int, seed);
  void set_seed(int seed);
 private:
  virtual void setParametersImpl(const ParametersContainer& parameters);
  virtual void applyImpl(const DataSet<TElement>& input,
                               DataSet<TElement>* output) const;
  virtual string getDefaultAlias() const {return "DataRandomSampler";}
  double sampling_fraction_;
  bool with_replacement_;
  int seed_;
};

// template realizations

template <class TElement>
void DataRandomSampler<TElement>::set_seed(int seed) {
  CHECK(seed >= 0);  //NOLINT
  seed_ = seed;
  randomizer.setSeed(seed_);
}

template <class TElement>
DataRandomSampler<TElement>::DataRandomSampler(
    const ParametersContainer& parameters) {
  this->setParameters(parameters);
}
template <class TElement>
DataRandomSampler<TElement>::DataRandomSampler(double sampling_fraction,
                                               bool with_replacement,
                                               int seed) {
  set_sampling_fraction(sampling_fraction);
  set_with_replacement(with_replacement);
  set_seed(seed);
}

template <class TElement>
void DataRandomSampler<TElement>::setDefaultParameters() {
  set_sampling_fraction(0.3);
  set_with_replacement(true);
  set_seed(42);
}

template <class TElement>
void DataRandomSampler<TElement>::checkParameters() const {
  if (with_replacement_) {
    CHECK(sampling_fraction_ > 0.);
  } else {
    CHECK(sampling_fraction_ > 0. && sampling_fraction_ <= 1.);
  }
  CHECK(seed_ >= 0); //NOLINT
}

template <class TElement>
void DataRandomSampler<TElement>::setParametersImpl(
    const ParametersContainer& parameters) {
  with_replacement_ = parameters.Get<bool>("WITH_REPLACMENT");
  sampling_fraction_ = parameters.Get<double>("SAMPLING_FRACTION");
  set_seed(parameters.Get<int>("SEED"));
}


template <class TElement>
void DataRandomSampler<TElement>::applyImpl(
    const DataSet<TElement>& input,
    DataSet<TElement>* output) const {
  int sample_size = (int)ceil(input.size() * sampling_fraction_);
  Indices indices(sample_size);
  if (with_replacement_) {
    for (int i = 0; i < (int)indices.size(); ++i) {
      indices[i] = randomizer.rand(input.size());
    }
  } else {
    // \TODO(sameg) Not thread safe.
    // Lets think about using custom random numbers generator
    getRandomIndices(&indices, input.size(), sample_size);
  }
  *output = input.lightSubset(indices);
}

template <class TElement>
string DataRandomSampler<TElement>::toString() const {
  std::stringstream str;
  std::fixed(str);
  str.precision(2);
  str << "RandomSampler: sampling_fraction = ";
  str << sampling_fraction_ << ", with_replacment = " << with_replacement_;
  str << ", seed = " << seed_;
  return str.str();
}
};

#endif  // LTR_DATA_PREPROCESSORS_DATA_RANDOM_SAMPLER_H_
