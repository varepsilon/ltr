// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_K_FOLD_SIMPLE_SPLITTER_H_
#define LTR_CROSSVALIDATION_K_FOLD_SIMPLE_SPLITTER_H_

#include <vector>
#include <stdexcept>

#include "crossvalidation/splitter.h"

using std::vector;
using std::logic_error;

namespace ltr {
  namespace cv {
    /**
     * Simply splits by indexes (not using random).
     */
    template<class TElement>
    class KFoldSimpleSplitter : public Splitter<TElement> {
    public:
      explicit KFoldSimpleSplitter(int in_k = 10)
          :k(in_k) {
        if (k < 2) {
          throw logic_error("k should be grater then 1!");
        }
      }

      virtual int splitCount(const DataSet<TElement>& base_set) const;

    protected:
      virtual void splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        vector<size_t>* train_set_indexes,
        vector<size_t>* test_set_indexes) const;
    private:
      const int k;
    };

    template<class TElement>
    int KFoldSimpleSplitter<TElement>::splitCount(
        const DataSet<TElement>& base_set) const {
      return k;
    }

    template<class TElement>
    void KFoldSimpleSplitter<TElement>::splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        vector<size_t>* train_set_indexes,
        vector<size_t>* test_set_indexes) const {
      if (split_index < 0 || split_index >= splitCount(base_set)) {
        throw logic_error("index should be in range [0..k-1]");
      }

      train_set_indexes->clear();
      test_set_indexes->clear();

      int block_size = base_set.size() / k;
      int extra_length = base_set.size() % k;

      int test_begin = block_size * split_index + std::min(split_index, extra_length);
      int test_end = block_size * (split_index + 1) + std::min(split_index + 1, extra_length);

      for (size_t index = 0; index < test_begin; ++index) {
        train_set_indexes->push_back(index);
      }
      for (size_t index = test_begin; index < test_end;  ++index) {
        test_set_indexes->push_back(index);
      }
      for (size_t index = test_end; index < base_set.size(); ++index) {
        train_set_indexes->push_back(index);
      }
    };
  };
};

#endif  // LTR_CROSSVALIDATION_K_FOLD_SIMPLE_SPLITTER_H_
