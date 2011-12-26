// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_TK_FOLD_SIMPLE_SPLITTER_H_
#define LTR_CROSSVALIDATION_TK_FOLD_SIMPLE_SPLITTER_H_

#include <algorithm>
#include <vector>
#include <cstdlib>
#include <stdexcept>

#include "crossvalidation/splitter.h"

using std::vector;
using std::random_shuffle;
using std::logic_error;

namespace ltr {
  namespace cv {
    /**
     * Splits by indexes and shuffles them
     */
    template<class TElement>
    class TKFoldSimpleSplitter : public Splitter<TElement> {
    public:
      TKFoldSimpleSplitter(size_t in_k = 10, size_t in_T = 10)
          : k(in_k), T(in_T) {
        if (k < 2) {
          throw logic_error("k should be grater then 1!");
        }
        if (T < 1) {
          throw logic_error("T should be positive!");
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
      typedef vector<size_t> Permutation;
      Permutation getRandomPermutation(int index, int dataset_size) const;

      const size_t T;
      const size_t k;
    };

    template<class TElement>
    int TKFoldSimpleSplitter<TElement>::splitCount(
        const DataSet<TElement>& base_set) const {
      return k * T;
    }

    template<class TElement>
    void TKFoldSimpleSplitter<TElement>::splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        vector<size_t>* train_set_indexes,
        vector<size_t>* test_set_indexes) const {
      if (split_index < 0 || split_index >= splitCount(base_set)) {
        throw logic_error("index should be in range [0..T*k-1]");
      }

      train_set_indexes->clear();
      test_set_indexes->clear();

      int blocksplit_index = split_index / k;
      int block_index = split_index % k;

      Permutation current_perm =
        getRandomPermutation(blocksplit_index, base_set.size());

      int block_size = base_set.size() / k;
      int extra_length = base_set.size() % k;

      int test_begin = block_size * block_index + std::min(block_index, extra_length);
      int test_end = block_size * (block_index + 1) + std::min(block_index + 1, extra_length);

      for (size_t index = 0; index < test_begin; ++index) {
        train_set_indexes->push_back(current_perm[index]);
      }
      for (size_t index = test_begin; index < test_end;  ++index) {
        test_set_indexes->push_back(current_perm[index]);
      }
      for (size_t index = test_end; index < base_set.size(); ++index) {
        train_set_indexes->push_back(current_perm[index]);
      }
    }

    template<class TElement>
    typename TKFoldSimpleSplitter<TElement>::Permutation TKFoldSimpleSplitter<TElement>::
        getRandomPermutation(int index, int dataset_size) const {
      // one can use any other shift instead 23
      srand(index + 23);
      Permutation output;
      for (size_t index = 0; index < dataset_size; ++index) {
        output.push_back(index);
      }
      random_shuffle(output.begin(), output.end());

      return output;
    }
  };
};

#endif  // LTR_CROSSVALIDATION_TK_FOLD_SIMPLE_SPLITTER_H_
