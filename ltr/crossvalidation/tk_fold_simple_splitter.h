// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_TK_FOLD_SIMPLE_SPLITTER_H_
#define LTR_CROSSVALIDATION_TK_FOLD_SIMPLE_SPLITTER_H_

#include <boost/shared_ptr.hpp>
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
          (const ParametersContainer& parameters = ParametersContainer())
          : Splitter<TElement>("TKFoldSimpleSplitter") {
        this->setDefaultParameters();
        this->copyParameters(parameters);
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

    protected:
      virtual void splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        vector<size_t>* train_set_indexes,
        vector<size_t>* test_set_indexes) const;

    private:
      typedef vector<size_t> Permutation;
      /**
       * Gets a random permutation of length dataset_size
       * @param index - number of permutation
       * @param dataset_size - length of permutation
       */
      Permutation getRandomPermutation(int index, int dataset_size) const;
    };

    // template realizations
    template<class TElement>
    void TKFoldSimpleSplitter<TElement>::setDefaultParameters() {
      this->clearParameters();
      this->addIntParameter("K", 10);
      this->addIntParameter("T", 10);
    }

    template<class TElement>
    void TKFoldSimpleSplitter<TElement>::checkParameters() const {
      if (this->getIntParameter("K") < 2) {
        throw logic_error(this->alias() + " k should be grater then 1!");
      }
      if (this->getIntParameter("T") < 1) {
        throw logic_error(this->alias() + " T should be positive!");
      }
    }

    template<class TElement>
    int TKFoldSimpleSplitter<TElement>::splitCount(
        const DataSet<TElement>& base_set) const {
      return this->getIntParameter("K") * this->getIntParameter("T");
    }

    template<class TElement>
    void TKFoldSimpleSplitter<TElement>::splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        vector<size_t>* train_set_indexes,
        vector<size_t>* test_set_indexes) const {
      if (split_index < 0 || split_index >= splitCount(base_set)) {
        throw logic_error(this-> alias() +
          " index should be in range [0..T*k-1]");
      }

      train_set_indexes->clear();
      test_set_indexes->clear();

      int blocksplit_index = split_index / this->getIntParameter("K");
      int block_index = split_index % this->getIntParameter("K");

      Permutation current_perm =
        getRandomPermutation(blocksplit_index, base_set.size());

      int block_size = base_set.size() / this->getIntParameter("K");
      int extra_length = base_set.size() % this->getIntParameter("K");

      int test_begin = block_size * block_index +
        std::min(block_index, extra_length);
      int test_end = block_size * (block_index + 1) +
        std::min(block_index + 1, extra_length);

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
    typename TKFoldSimpleSplitter<TElement>::Permutation
      TKFoldSimpleSplitter<TElement>::getRandomPermutation(
        int index, int dataset_size) const {
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
