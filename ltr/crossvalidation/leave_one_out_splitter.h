// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_LEAVE_ONE_OUT_SPLITTER_H_
#define LTR_CROSSVALIDATION_LEAVE_ONE_OUT_SPLITTER_H_

#include "ltr/utility/shared_ptr.h"
#include <vector>
#include <stdexcept>
#include <string>

#include "ltr/crossvalidation/splitter.h"

using std::string;
using std::vector;
using std::logic_error;

namespace ltr {
namespace cv {
/**
 * Splits leaving only one test object
 */
template<class TElement>
class LeaveOneOutSplitter : public Splitter<TElement> {
 public:
  typedef ltr::utility::shared_ptr<LeaveOneOutSplitter> Ptr;

  virtual int splitCount(const DataSet<TElement>& base_set) const;
  /**
   * @param parameters Standart LTR parameter container with no parameters
   * (LeaveOneOutSplitter has no parameters at all)
   */

  explicit LeaveOneOutSplitter() {
    this->setDefaultParameters();
  }

  explicit LeaveOneOutSplitter
      (const ParametersContainer& parameters) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
  }

  string toString() const;
 protected:
  virtual void splitImpl(
    int split_index,
    const DataSet<TElement>& base_set,
    std::vector<int>* train_set_indexes,
    std::vector<int>* test_set_indexes) const;
  virtual string getDefaultAlias() const {return "LeaveOneOutSplitter";}
};

// template realizations
template<class TElement>
string LeaveOneOutSplitter<TElement>::toString() const {
  return "Leave-one-out splitter";
}

template<class TElement>
int LeaveOneOutSplitter<TElement>::splitCount(
    const DataSet<TElement>& base_set) const {
  return base_set.size();
}

template<class TElement>
void LeaveOneOutSplitter<TElement>::splitImpl(
    int split_index,
    const DataSet<TElement>& base_set,
    vector<int>* train_set_indexes,
    vector<int>* test_set_indexes) const {
  if (split_index < 0 || split_index >= splitCount(base_set)) {
    throw logic_error(this->alias() +
      " index should be in range [0..dataset_size-1]");
  }

  train_set_indexes->clear();
  test_set_indexes->clear();

  test_set_indexes->push_back(split_index);
  for (int index = 0; index < split_index; ++index) {
    train_set_indexes->push_back(index);
  }
  for (int index = split_index + 1; index < base_set.size(); ++index) {
    train_set_indexes->push_back(index);
  }
}
};
};

#endif  // LTR_CROSSVALIDATION_LEAVE_ONE_OUT_SPLITTER_H_
