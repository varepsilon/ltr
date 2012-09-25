// Copyright 2012 Yandex

#ifndef LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_
#define LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_

#include <vector>

#include "ltr/data/data_set.h"

#include "ltr/data/utility/data_set_label_statistics.h"

using std::vector;

namespace ltr {
namespace utility {
/**
 * This function calculates min and max features for
 * given dataset.
 */
template <typename TElement>
void getFeaturesMinMaxValues(const DataSet<TElement>& dataset,
                             vector<double>* min_features_values,
                             vector<double>* max_features_values);

double getDataSetEntropy(const LabelStatisticComputer& label_stat_computer);

template <typename TElement>
double getDataSetEntropy(const DataSet<TElement>& data);
};
};

#endif  // LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_
