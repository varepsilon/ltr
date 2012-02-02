// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_SPLITTER_H_
#define LTR_CROSSVALIDATION_SPLITTER_H_

#include <boost/shared_ptr.hpp>

#include <vector>

#include "ltr/data/data_set.h"

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
       typedef boost::shared_ptr<Splitter> Ptr;
       typedef boost::shared_ptr<Splitter> BasePtr;
      /**
       * Total number of possible splits for an input dataset.
       */
       virtual int splitCount(const DataSet<TElement>& base_set) const = 0;
      /**
       * Perform split.
       * @param split_index - index of split (0..splitCount()-1).
       * @param base_set - set to be splitted
       */
      SplittedDataSet<TElement> split(int split_index,
        const DataSet<TElement>& base_set) const;

      virtual ~Splitter() {}
     protected:
      virtual void splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        vector<size_t>* train_set_indexes,
        vector<size_t>* test_set_indexes) const = 0;
    };

    template <class TElement>
    SplittedDataSet<TElement> Splitter<TElement>::split(int split_index,
        const DataSet<TElement>& base_set) const {
      vector<size_t> train_set_indexes;
      vector<size_t> test_set_indexes;
      splitImpl(split_index, base_set, &train_set_indexes, &test_set_indexes);

      SplittedDataSet<TElement> output(base_set.featureInfo());
      for (int train = 0; train < train_set_indexes.size(); ++train) {
        output.train_set.add(base_set[train_set_indexes[train]],
          base_set.getWeight(train_set_indexes[train]));
      }
      for (int test = 0; test < test_set_indexes.size(); ++test) {
        output.test_set.add(base_set[test_set_indexes[test]],
          base_set.getWeight(test_set_indexes[test]));
      }

      return output;
    }
  };
};

#endif  // LTR_CROSSVALIDATION_SPLITTER_H_
