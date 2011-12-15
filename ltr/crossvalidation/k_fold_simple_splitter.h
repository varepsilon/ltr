// Copyright 2011 Yandex

#include <vector>

#include "crossvalidation/splitter.h"

using std::vector;

namespace ltr {
  namespace cv {
    /**
     * Simply splits by indexes (not using random).
     */
    template<class TElement>
    class KFoldSimpleSplitter : public Splitter<TElement> {
    public:
      KFoldSimpleSplitter(int in_k = 10)
          :k(in_k) {
        if(k < 2) {
          throw std::logic_error("k should be grater then 1!");
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
    int KFoldSimpleSplitter<TElement>::splitCount(const DataSet<TElement>& base_set) const {
      return k;
    }

    template<class TElement>
    void KFoldSimpleSplitter<TElement>::splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        vector<size_t>* train_set_indexes,
        vector<size_t>* test_set_indexes) const {
      if(split_index < 0 || split_index >= splitCount(base_set)) {
        throw std::logic_error("index should be in range [0..k-1]");
      }
  
      train_set_indexes->clear();
      test_set_indexes->clear();
  
      size_t fold_size = base_set.size() / k;
      for (size_t index = 0; index < fold_size * split_index; ++index) {
        train_set_indexes->push_back(index);
      }
      for(size_t index = fold_size * split_index; index < fold_size * (split_index + 1); ++index)
      {
        test_set_indexes->push_back(index);
      }
      for(size_t index = fold_size * (split_index + 1); index < base_set.size(); ++index)
      {
        train_set_indexes->push_back(index);
      }
    };
  };
};
