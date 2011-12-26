// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_
#define LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_

#include <vector>

#include "ltr/data/data_set.h"

namespace ltr {
namespace utility {

template <typename TElement>
void calcMinMaxStatistics(const DataSet<TElement>& data_set,
    std::vector<double>* pMinFeatureValue,
    std::vector<double>* pMaxFeatureValue);
}
}
#endif  // LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_
