// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_H_
#define LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_H_

#include <string>
#include <vector>

#include "ltr/utility/boost/shared_ptr.h"  //NOLINT

#include "ltr/data/data_set.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/printable.h"
#include "ltr/parameters_container/parameters_container.h"

using std::string;
using std::vector;

namespace ltr {
/**
* \brief A base class for data preprocessors.
*
* Preprocesses DataSet, e.g. delete or replicate elements in DataSet.
* This can be usefull for better learning (see Learner<TElement>::addDataPreprocessor ).
* Should not change Object features and FeatureInfo as it won't be saved in Scorer.
* Use FeatureConverter for feature convertions.
*
* \sa FeatureConverter, BaseLearner
*/
template <class TElement>
class DataPreprocessor : public Parameterized,
                         public Printable,
                         public Aliaser {
 public:
  typedef ltr::utility::shared_ptr<DataPreprocessor> BasePtr;
  typedef ltr::utility::shared_ptr<DataPreprocessor> Ptr;
  virtual ~DataPreprocessor() {}
  /**
   * Preprocesses DataSet.
   * \param input DataSet to be preprocessed
   * \param output preprocessed DataSet
   * \param check_parameters whether perform Parameterized::checkParameters() before launch
  */
  void apply(const DataSet<TElement>& input,
                   DataSet<TElement>* output,
             bool check_parameters = true) const;
 private:
  virtual void applyImpl(const DataSet<TElement>& input,
                               DataSet<TElement>* output) const = 0;
};

// template realization

template <class TElement>
void DataPreprocessor<TElement>::apply(const DataSet<TElement>& input,
                                             DataSet<TElement>* output,
                                       bool check_parameters) const {
  if (check_parameters) {
    checkParameters();
  }
  applyImpl(input, output);
}
};
#endif  // LTR_DATA_PREPROCESSORS_DATA_PREPROCESSOR_H_
