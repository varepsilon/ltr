// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_LEARNER_H_
#define LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include "ltr/data/data_set.h"
#include "ltr/data_preprocessors/data_preprocessor.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/measures/measure.h"

namespace ltr {
/**
 * An object which learns on inputted dataset and produces
 * a DataPreprocessor. May produce different preprocessors (using random)
 * for one inputted dataset as in begging. May use measure
 */
template <class TElement>
class BaseDataPreprocessorLearner : public Parameterized {
  public:
  typedef boost::shared_ptr<BaseDataPreprocessorLearner> Ptr;
  /**
   * Learns from inputted dataset. E. g. remembers number of elements
   * in dataset and e.t.c.
   */
  virtual void learn(const DataSet<TElement>& data_set) = 0;
  /**
   * Produces a DataPreprocessor. Must have been learned before calling make()
   */
  virtual typename DataPreprocessor<TElement>::Ptr makePtr() const = 0;
  /**
   * Sets measure, used in DataPreprocessor. Note that many DataPreprocessors
   * don't use measures, so they ignore the mesure setted
   */
  void setMeasure(typename Measure<TElement>::Ptr in_measure) {
    measure_ = in_measure;
  }
  protected:
  typename Measure<TElement>::Ptr measure_;
};

template <class TElement, template<class> class TDataPreprocessor>
class DataPreprocessorLearner : public BaseDataPreprocessorLearner<TElement> {
  public:
  typedef boost::shared_ptr<DataPreprocessorLearner> Ptr;

  virtual TDataPreprocessor<TElement> make() const = 0;
  virtual typename DataPreprocessor<TElement>::Ptr makePtr() const {
    return DataPreprocessor<TElement>::Ptr(
      new TDataPreprocessor<TElement>(make()));
  }
};
}

#endif  // LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_LEARNER_H_
