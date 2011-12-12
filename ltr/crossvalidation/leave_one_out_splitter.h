// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_LEAVE_ONE_OUT_SPLITTER_H_
#define LTR_CROSSVALIDATION_LEAVE_ONE_OUT_SPLITTER_H_

#include <vector>

#include "crossvalidation/splitter.h"

using std::vector;

namespace ltr {
  namespace cv {
    /**
     * Splits leaving only one test object
     */
    template<class TElement>
    class LeaveOneOutSplitter : public Splitter<class TElement> {
    public:
      explicit LeaveOneOutSplitter(size_t dataset_size)
        : Splitter(dataset_size) {}

    private:
      virtual void splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        std::vector<size_t>* train_set_indexes,
        std::vector<size_t>* test_set_indexes) const;
    };

    template<class TElement>
    void LeaveOneOutSplitter<TElement>::splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        vector<size_t>* train_set_indexes,
        vector<size_t>* test_set_indexes) const {
      if (split_index < 0 || split_index >= splitCount()) {
        throw std::logic_error("index should be in range [0..dataset_size-1]");
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
