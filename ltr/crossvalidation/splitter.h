// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_SPLITTER_H_
#define LTR_CROSSVALIDATION_SPLITTER_H_

#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>

#include "ltr/interfaces/printable.h"
#include "ltr/data/data_set.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/aliaser.h"

using std::vector;
using std::string;
using ltr::Aliaser;
using ltr::utility::lightSubset;

namespace ltr {
  namespace cv {
    template<class TElement>
    /**
     * Contains two datasets, one for training, another for testing
     */
    struct SplittedDataSet {
      DataSet<TElement> train_set;
      DataSet<TElement> test_set;

      explicit SplittedDataSet(const FeatureInfo& featureInfo = FeatureInfo()) :
        train_set(featureInfo),
        test_set(featureInfo) {}
    };

    /**
     * Splits data set into training + testing sets.
     */
    template<class TElement>
    class Splitter : public Parameterized, public Aliaser, public Printable {
    public:
      typedef boost::shared_ptr<Splitter> Ptr;
      typedef boost::shared_ptr<Splitter> BasePtr;
      /**
       * Sets splitter's alias
       */
      explicit Splitter(const string& alias) : Aliaser(alias) {}
      /**
       * Total number of possible splits for an input dataset.
       */
      virtual int splitCount(const DataSet<TElement>& base_set) const = 0;
      /**
       * Perform split into training + testing sets
       * @param split_index - index of split (0..splitCount()-1)
       * @param base_set - set to be splitted
       */
      SplittedDataSet<TElement> split(int split_index,
        const DataSet<TElement>& base_set) const;

      virtual ~Splitter() {}

    protected:
      /**
       * Perform split into training + testing sets by operating objects' indices
       * @param split_index - index of split (0..splitCount()-1)
       * @param base_set - set to be splitted
       * @param train_set_indexes - indices of objects from input dataset to
       * be splitted as train set
       * @param test_set_indexes - indices of objects from input dataset to
       * be splitted as test set
       */
      virtual void splitImpl(
        int split_index,
        const DataSet<TElement>& base_set,
        vector<int>* train_set_indexes,
        vector<int>* test_set_indexes) const = 0;
    };

    // template realization
    template <class TElement>
    SplittedDataSet<TElement> Splitter<TElement>::split(int split_index,
        const DataSet<TElement>& base_set) const {
      vector<int> train_set_indexes;
      vector<int> test_set_indexes;
      splitImpl(split_index, base_set, &train_set_indexes, &test_set_indexes);

      SplittedDataSet<TElement> output(base_set.featureInfo());
      output.train_set = lightSubset(base_set, train_set_indexes);
      output.test_set = lightSubset(base_set, test_set_indexes);

      return output;
    }
  };
};

#endif  // LTR_CROSSVALIDATION_SPLITTER_H_
