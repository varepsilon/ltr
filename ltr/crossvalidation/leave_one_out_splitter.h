// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_LEAVE_ONE_OUT_SPLITTER_H_
#define LTR_CROSSVALIDATION_LEAVE_ONE_OUT_SPLITTER_H_

#include <vector>
#include <stdexcept>

#include "ltr/crossvalidation/splitter.h"

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
      virtual int splitCount(const DataSet<TElement>& base_set) const;

    protected:
      virtual void splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        std::vector<size_t>* train_set_indexes,
        std::vector<size_t>* test_set_indexes) const;
    };

    template<class TElement>
    int LeaveOneOutSplitter<TElement>::splitCount(
        const DataSet<TElement>& base_set) const {
      return base_set.size();
    }

    template<class TElement>
    void LeaveOneOutSplitter<TElement>::splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        vector<size_t>* train_set_indexes,
        vector<size_t>* test_set_indexes) const {
      if (split_index < 0 || split_index >= splitCount(base_set)) {
        throw logic_error("index should be in range [0..dataset_size-1]");
      }

      train_set_indexes->clear();
      test_set_indexes->clear();

      test_set_indexes->push_back(split_index);
      for (size_t index = 0; index < split_index; ++index) {
        train_set_indexes->push_back(index);
      }
      for (size_t index = split_index + 1; index < base_set.size(); ++index) {
        train_set_indexes->push_back(index);
      }
    }
  };
};

#endif  // LTR_CROSSVALIDATION_LEAVE_ONE_OUT_SPLITTER_H_
