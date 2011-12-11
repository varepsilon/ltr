// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_LEAVE_ONE_OUT_SPLITTER_H_
#define LTR_CROSSVALIDATION_LEAVE_ONE_OUT_SPLITTER_H_

#include "crossvalidation/splitter.h"

#include <vector>

namespace ltr {
  namespace cv {
    /**
     * Splits leaving only one test object
     */
    template<class TElement>
    class LeaveOneOutSplitter : public Splitter<class TElement> {
    public:
      explicit LeaveOneOutSplitter(size_t dataset_size);

      virtual int size() const;

    private:
      virtual void splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        std::vector<size_t>* train_set_indexes,
        std::vector<size_t>* test_set_indexes) const;

      const size_t dataset_size_;
    };

    template<class TElement>
    LeaveOneOutSplitter<TElement>::LeaveOneOutSplitter(size_t dataset_size)
      : dataset_size_(dataset_size) {
    }

    template<class TElement>
    int LeaveOneOutSplitter<TElement>::size() const {
      return dataset_size_;
    }

    template<class TElement>
    void LeaveOneOutSplitter<TElement>::splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        std::vector<size_t>* train_set_indexes,
        std::vector<size_t>* test_set_indexes) const {
      if (split_index < 0 || split_index >= size()) {
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
