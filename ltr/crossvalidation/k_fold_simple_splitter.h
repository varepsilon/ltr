// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_K_FOLD_SIMPLE_SPLITTER_H_
#define LTR_CROSSVALIDATION_K_FOLD_SIMPLE_SPLITTER_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "crossvalidation/splitter.h"

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
          (const ParametersContainer& parameters = ParametersContainer())
          : Splitter<TElement>("KFoldSimpleSplitter") {
        this->setDefaultParameters();
        this->copyParameters(parameters);
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

    protected:
      virtual void splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        vector<int>* train_set_indexes,
        vector<int>* test_set_indexes) const;
    };

    // template realizations
    template<class TElement>
    void KFoldSimpleSplitter<TElement>::setDefaultParameters() {
      this->clearParameters();
      this->addIntParameter("K", 10);
    }

    template<class TElement>
    void KFoldSimpleSplitter<TElement>::checkParameters() const {
      CHECK_INT_PARAMETER("K", X >= 2);
    }

    template<class TElement>
    int KFoldSimpleSplitter<TElement>::splitCount(
        const DataSet<TElement>& base_set) const {
      return this->getIntParameter("K");
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

      int block_size = base_set.size() / this->getIntParameter("K");
      int extra_length = base_set.size() % this->getIntParameter("K");

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
