// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_BEGGING_PREPROCESSOR_H_
#define LTR_DATA_PREPROCESSORS_BEGGING_PREPROCESSOR_H_

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

using std::string;
using std::vector;
using std::random_shuffle;
using std::copy;
using ltr::utility::lightSubset;

namespace ltr {
/**
 * Acts as SubsetPreprocessor with random indices each new applying
 * (duplication allowed)
 */
template <typename TElement>
class BeggingPreprocessor : public DataPreprocessor<TElement> {
  public:
  typedef boost::shared_ptr<BeggingPreprocessor> Ptr;

  /**
   * @param parameters Standart LTR parameter container with double parameter
   * SELECTED_PART, bool parameter WITH_REPLACE and int parameter RANDOM_SEED.
   * WITH_REPLACE is true if dublication in indices of produced SimpleSubset
   * Preprocessor is allowed, false otherwise. SELECTED_PART is a part of elements
   * chosen by produced SubsetPreprocessor. Upper rounding is used, so
   * never produces SubsetPreprocessor with 0 features. RANDOM_SEED is for
   * manual control of random behavior of BeggingPreprocessorLearner
   * By default SELECTED_PART = 0.3, WITH_REPLACE = true, RANDOM_SEED = 237
   */
  explicit BeggingPreprocessor(
      const ParametersContainer& parameters = ParametersContainer())
      : DataPreprocessor<TElement>("BeggingPreprocessor") {
    this->setDefaultParameters();
    this->copyParameters(parameters);
    srand(this->parameters().template Get<int>("RANDOM_SEED"));
  }

  void setDefaultParameters();
  void checkParameters() const;
  void apply(const DataSet<TElement>& input,
      DataSet<TElement>* output) const;

 private:
  template <class T>
  struct Belongs: public std::unary_function<T, bool> {
    Belongs(const T &min, const T &max): min_(min), max_(max) { }
    bool operator()(const T& x) const {
      return x > min_ && x <= max_;
    }
   private:
    const T &min_;
    const T &max_;
  };

  string toString() const;
};

// template realizations
template <typename TElement>
string BeggingPreprocessor<TElement>::toString() const {
  std::stringstream str;
  std::fixed(str);
  str.precision(2);
  str << "Begging data preprocessor with parameters: SELECTED_PART = ";
  str << this->parameters().template Get<double>("SELECTED_PART");
  str << ", WITH_REPLACE = ";
  str << this->parameters().template Get<bool>("WITH_REPLACE");
  str << ", RANDOM_SEED = ";
  str << this->parameters().template Get<int>("RANDOM_SEED");
  return str.str();
}

template <typename TElement>
void BeggingPreprocessor<TElement>::setDefaultParameters() {
  this->clearParameters();
  this->addNewParam("SELECTED_PART", 0.3);
  this->addNewParam("WITH_REPLACE", true);
  this->addNewParam("RANDOM_SEED", 237);
}

template <typename TElement>
void BeggingPreprocessor<TElement>::checkParameters() const {
  if (this->parameters().template Get<bool>("WITH_REPLACE")) {
      Parameterized::checkParameter<double>("SELECTED_PART",
                                  std::bind2nd(std::greater_equal<double>(),
                                               0));
  } else {
    Parameterized::checkParameter<double>("SELECTED_PART",
                                                         Belongs<double>(0, 1));
  }
  Parameterized::checkParameter<int>("RANDOM_SEED",
                                         std::bind2nd(std::greater<int>(), 0));
}

template <typename TElement>
void BeggingPreprocessor<TElement>::apply(
      const DataSet<TElement>& input_dataset,
      DataSet<TElement>* output_dataset) const {
  const ParametersContainer &params = this->parameters();
  int size = static_cast<int>(ceil(input_dataset.size()
    * params.Get<double>("SELECTED_PART")));

  boost::mt19937 generator;
  boost::random_number_generator<boost::mt19937> random(generator);

  if (size != 0) {
    vector<int> indices(size);
    if (params.Get<bool>("WITH_REPLACE")) {
      for (int i = 0; i < indices.size(); ++i) {
        indices[i] = random(input_dataset.size());
      }
    } else {
      vector<int> all_used(input_dataset.size());
      for (int index = 0; index < all_used.size(); ++index) {
        all_used[index] = index;
      }
      random_shuffle(all_used.begin(), all_used.end());
      copy(all_used.begin(), all_used.begin() + size, indices.begin());
    }
    *output_dataset = lightSubset(input_dataset, indices);
  } else {
    *output_dataset = input_dataset;
  }
}
};

#endif  // LTR_DATA_PREPROCESSORS_BEGGING_PREPROCESSOR_H_
