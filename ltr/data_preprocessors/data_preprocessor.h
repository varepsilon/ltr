// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_H_
#define LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_H_

#include <boost/shared_ptr.hpp>
#include <string>

#include "ltr/interfaces/printable.h"
#include "ltr/data/data_set.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/aliaser.h"

using std::string;

namespace ltr {
  /**
   * Preprocesses dataset without changing FeatureInfo. Shouldn't convert
   * object's features. Can delete or replicate elements in dataset. In
   * more complicated case can estimate probability distribution of elements
   * and generate new elements from it
   */
  template <class TElement>
  class DataPreprocessor : public Parameterized,
    public Printable,
    public Aliaser {
  public:
    typedef boost::shared_ptr<DataPreprocessor> Ptr;
    /**
     * Sets DataPreprocessor's alias
     */
    explicit DataPreprocessor(const string& alias) : Aliaser(alias) {}

    virtual ~DataPreprocessor() {}
    /**
     * Preprocesses dataset
     * @param input - dataset to be preprocessed
     * @param output - output preprocessed dataset
     */
    virtual void apply(const DataSet<TElement>& input,
      DataSet<TElement>* output) const = 0;
  };
};

#endif  // LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_H_
