// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_
#define LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_

#include <vector>

#include "ltr/data/data_set.h"

using std::vector;

namespace ltr {
namespace utility {
/** This function calculates min and max features for
 * given dataset. Used in normalizing converter and decision trees.
 */
template <typename TElement>
void getFeaturesValuesRanges(const DataSet<TElement>& dataset,
                             vector<double>* min_features_values,
                             vector<double>* max_features_values);
}
}
#endif  // LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_
