// Copyright 2011 Yandex

#include <algorithm>
#include <vector>
#include <cstdlib>

#include "crossvalidation/splitter.h"

using std::vector;
using std::random_shuffle;

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
        if(k < 2) {
          throw std::logic_error("k should be grater then 1!");
        }
        if(T < 1) {
          throw std::logic_error("T should be positive!");
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
    int TKFoldSimpleSplitter<TElement>::splitCount(const DataSet<TElement>& base_set) const {
      return k * T;
    }
    
    template<class TElement>
    void TKFoldSimpleSplitter<TElement>::splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        vector<size_t>* train_set_indexes,
        vector<size_t>* test_set_indexes) const {
      if(split_index < 0 || split_index >= splitCount(base_set)) {
        throw std::logic_error("index should be in range [0..T*k-1]");
      }

      train_set_indexes->clear();
      test_set_indexes->clear();

      int blocksplit_index = split_index / k;
      int block_index = split_index % k;

      Permutation current_perm =
        getRandomPermutation(blocksplit_index, base_set.size());

      size_t fold_size = base_set.size() / k;
      for (size_t index = 0; index < fold_size * block_index; ++index) {
        train_set_indexes->push_back(current_perm(index));
      }
      for(size_t index = fold_size * block_index; index < fold_size * (block_index + 1); ++index)
      {
        test_set_indexes->push_back(current_perm(index));
      }
      for(size_t index = fold_size * (block_index + 1); index < base_set.size(); ++index)
      {
        train_set_indexes->push_back(current_perm(index));
      }
  
    }
    
    template<class TElement>
    Permutation TKFoldSimpleSplitter<TElement>::
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
