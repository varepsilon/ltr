// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_SPLITTER_H_
#define LTR_CROSSVALIDATION_SPLITTER_H_

#include <vector>

#include "data/data_set.h"

using std::vector;

namespace ltr {
  namespace cv {
    template<class TElement>
    struct SplittedDataSet {
      DataSet<TElement> train_set;
      DataSet<TElement> test_set;

      explicit SplittedDataSet(const FeatureInfo& featureInfo = FeatureInfo()) :
        train_set(featureInfo),
        test_set(featureInfo) {}
    };

    /**
     * Splits data set into training + test sets.
     */
    template<class TElement>
    class Splitter {
     public:
       explicit Splitter(int split_count) : split_count_(split_count) {}
      /**
       * Total number of possible splits.
       */
       int splitCount() const {
         return split_count_;
       };
      /**
       * Perform split.
       * @param split_index - index of split (0..splitCount()-1).
       * @param base_set - set to be splitted
       */
      SplittedDataSet split(int split_index,
        const DataSet<TElement>& base_set) const;

      virtual ~Splitter() {}
     protected:
      virtual void splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        vector<size_t>* train_set_indexes,
        vector<size_t>* test_set_indexes) const = 0;
     private:
      int split_count_;
    };
  };
};

// !rewrite with preprocessors
template <class TElement>
SplittedDataSet Splitter<TElement>::split(int split_index,
    const DataSet<TElement>& base_set) const {

  vector<size_t> train_set_indexes;
  vector<size_t> test_set_indexes;
  splitImpl(split_index, base_set, &train_set_indexes, &test_set_indexes);

  SplittedDataSet output(base_set.featureInfo());
  // here a subset preprocessor should be used, yet not performed
  // thus this block is written manually
  for (int train = 0; train < train_set_indexes.size(); ++train) {
    output.train_set.add(base_set[train], base_set.getWeight(train));
  }
  for (int test = 0; test < test_set_indexes.size(); ++test) {
    output.test_set.add(base_set[test], base_set.getWeight(test));
  }

  return output;
}

#endif  // LTR_CROSSVALIDATION_SPLITTER_H_
