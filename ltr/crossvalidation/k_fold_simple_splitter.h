// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_K_FOLD_SIMPLE_SPLITTER_H_
#define LTR_CROSSVALIDATION_K_FOLD_SIMPLE_SPLITTER_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <string>

#include "ltr/crossvalidation/splitter.h"

using std::string;
using std::vector;
using std::logic_error;

namespace ltr {
namespace cv {
/**
 * Simply splits by indices of objects in dataset (not using random).
 * Is in fact leave-one-out for granulated into k pieces dataset
 */
template<class TElement>
class KFoldSimpleSplitter : public Splitter<TElement> {
 public:
  typedef boost::shared_ptr<KFoldSimpleSplitter> Ptr;
  /**
   * @param parameters Standart LTR parameter container with int parameter
   * K, by default K = 10
   */
  explicit KFoldSimpleSplitter
      (const ParametersContainer& parameters) {
    this->setParameters(parameters);
  }

  explicit KFoldSimpleSplitter
      (const int K = 10) {
    K_ = K;
  }

  /**
   * Clears parameters container and sets int K = 10
   */
  void setDefaultParameters();
  /**
   * Checks if K >= 2 (that should be true)
   */
  void checkParameters() const;

  int splitCount(const DataSet<TElement>& base_set) const;

  string toString() const;
  GET_SET(int, K);
 protected:
  virtual void setParametersImpl(const ParametersContainer& parameters);
  virtual void splitImpl(
    int split_index,
    const DataSet<TElement>& base_set,
    vector<int>* train_set_indexes,
    vector<int>* test_set_indexes) const;
  virtual string getDefaultAlias() const {return "KFoldSimpleSplitter";}
  int K_;
};

// template realizations
template<class TElement>
string KFoldSimpleSplitter<TElement>::toString() const {
  std::stringstream str;
  str << this->parameters().template Get<int>("K");
  str << "-fold sequent splitter";
  return str.str();
}

template<class TElement>
void KFoldSimpleSplitter<TElement>::setDefaultParameters() {
  this->K_ = 10;
}

template<class TElement>
void KFoldSimpleSplitter<TElement>::checkParameters() const {
  CHECK(this->K_ >= 2); // NOLINT
}

template <class TElement>
void KFoldSimpleSplitter<TElement>::setParametersImpl(
    const ParametersContainer& parameters) {
  K_ = parameters.Get<int>("K");
}

template<class TElement>
int KFoldSimpleSplitter<TElement>::splitCount(
    const DataSet<TElement>& base_set) const {
  return K_;
}

template<class TElement>
void KFoldSimpleSplitter<TElement>::splitImpl(
    int split_index,
    const DataSet<TElement>& base_set,
    vector<int>* train_set_indexes,
    vector<int>* test_set_indexes) const {
  if (split_index < 0 || split_index >= splitCount(base_set)) {
    throw logic_error(this->alias() +
      " index should be in range [0..k-1]");
  }

  train_set_indexes->clear();
  test_set_indexes->clear();

  const ParametersContainer &params = this->parameters();

  int block_size = base_set.size() / K_;
  int extra_length = base_set.size() % K_;

  int test_begin = block_size * split_index +
    std::min(split_index, extra_length);
  int test_end = block_size * (split_index + 1) +
    std::min(split_index + 1, extra_length);

  for (int index = 0; index < test_begin; ++index) {
    train_set_indexes->push_back(index);
  }
  for (int index = test_begin; index < test_end;  ++index) {
    test_set_indexes->push_back(index);
  }
  for (int index = test_end; index < base_set.size(); ++index) {
    train_set_indexes->push_back(index);
  }
};
};
};

#endif  // LTR_CROSSVALIDATION_K_FOLD_SIMPLE_SPLITTER_H_
