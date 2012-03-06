// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_
#define LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_

#include <vector>

#include "ltr/data/data_set.h"

using std::vector;

namespace ltr {
namespace utility {

template <typename TElement>
void calcMinMaxStatistics(const DataSet<TElement>& data_set,
    vector<double>* pMinFeatureValue,
    vector<double>* pMaxFeatureValue);
}
}
#endif  // LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_
