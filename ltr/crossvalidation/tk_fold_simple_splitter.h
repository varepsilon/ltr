// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_TK_FOLD_SIMPLE_SPLITTER_H_
#define LTR_CROSSVALIDATION_TK_FOLD_SIMPLE_SPLITTER_H_

#include <boost/shared_ptr.hpp>
#include <algorithm>
#include <vector>
#include <functional>
#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <string>

#include "ltr/crossvalidation/splitter.h"

using std::string;
using std::vector;
using std::random_shuffle;
using std::logic_error;

namespace ltr {
namespace cv {
/**
 * Splits by indices of objects in dataset using random.
 * Knows T random permutations of indices (pseudo-random used)
 * and represets a k-fold splitter for every of that permutations
 */
template<class TElement>
class TKFoldSimpleSplitter : public Splitter<TElement> {
 public:
  typedef boost::shared_ptr<TKFoldSimpleSplitter> Ptr;
  /**
   * @param parameters Standart LTR parameter container with int parameters
   * K and T, by default K = T = 10
   */
  explicit TKFoldSimpleSplitter
      (const ParametersContainer& parameters) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
  }

  explicit TKFoldSimpleSplitter
      (const int K = 10,
       const int T = 10) {
    K_ = K;
    T_ = T;
  }

  /**
   * Clears parameters container and sets ints K = 10 and T = 10
   */
  void setDefaultParameters();
  /**
   * Checks if K >= 2 and T >= 1 (that should be true)
   */
  void checkParameters() const;

  int splitCount(const DataSet<TElement>& base_set) const;

  string toString() const;
 protected:
  virtual void setParametersImpl(const ParametersContainer& parameters);
  virtual void splitImpl(
    int split_index,
    const DataSet<TElement>& base_set,
    vector<int>* train_set_indexes,
    vector<int>* test_set_indexes) const;

  GET_SET(int, K);
  GET_SET(int, T);
 private:
  typedef vector<int> Permutation;
  /**
   * Gets a random permutation of length dataset_size
   * @param index - number of permutation
   * @param dataset_size - length of permutation
   */
  Permutation getRandomPermutation(int index, int dataset_size) const;
  virtual string getDefaultAlias() const {return "TKFoldSimpleSplitter";}
  int K_;
  int T_;
};

// template realizations
template<class TElement>
string TKFoldSimpleSplitter<TElement>::toString() const {
  std::stringstream str;
  str << "TK-fold splitter with parameters: T = ";
  str << T_;
  str << ", K = ";
  str << K_;
  return str.str();
}

template<class TElement>
void TKFoldSimpleSplitter<TElement>::setDefaultParameters() {
  K_ = 10;
  T_ = 10;
}

template<class TElement>
void TKFoldSimpleSplitter<TElement>::checkParameters() const {
  CHECK(K_ > 1);
  CHECK(T_ > 0);
}

template <class TElement>
void TKFoldSimpleSplitter<TElement>::setParametersImpl(
    const ParametersContainer& parameters) {
  K_ = parameters.Get<int>("K");
  T_ = parameters.Get<int>("T");
}

template<class TElement>
int TKFoldSimpleSplitter<TElement>::splitCount(
    const DataSet<TElement>& base_set) const {
  return K_ * T_;
}

template<class TElement>
void TKFoldSimpleSplitter<TElement>::splitImpl(
    int split_index,
    const DataSet<TElement>& base_set,
    vector<int>* train_set_indexes,
    vector<int>* test_set_indexes) const {
  if (split_index < 0 || split_index >= splitCount(base_set)) {
    throw logic_error(this-> alias() +
      " index should be in range [0..T*k-1]");
  }

  train_set_indexes->clear();
  test_set_indexes->clear();

  const ParametersContainer &params = this->parameters();

  int blocksplit_index = split_index / K_;
  int block_index = split_index % K_;

  Permutation current_perm =
    getRandomPermutation(blocksplit_index, base_set.size());

  int block_size = base_set.size() / K_;
  int extra_length = base_set.size() % K_;

  int test_begin = block_size * block_index +
    std::min(block_index, extra_length);
  int test_end = block_size * (block_index + 1) +
    std::min(block_index + 1, extra_length);

  for (int index = 0; index < test_begin; ++index) {
    train_set_indexes->push_back(current_perm[index]);
  }
  for (int index = test_begin; index < test_end;  ++index) {
    test_set_indexes->push_back(current_perm[index]);
  }
  for (int index = test_end; index < base_set.size(); ++index) {
    train_set_indexes->push_back(current_perm[index]);
  }
}

template<class TElement>
typename TKFoldSimpleSplitter<TElement>::Permutation
  TKFoldSimpleSplitter<TElement>::getRandomPermutation(
    int index, int dataset_size) const {
  // one can use any other shift instead 23
  srand(index + 23);
  Permutation output;
  for (int index = 0; index < dataset_size; ++index) {
    output.push_back(index);
  }
  random_shuffle(output.begin(), output.end());

  return output;
}
};
};

#endif  // LTR_CROSSVALIDATION_TK_FOLD_SIMPLE_SPLITTER_H_
