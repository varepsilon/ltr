// Copyright 2011 Yandex

#include <limits>
#include <vector>

#include "ltr/data/utility/data_set_statistics.h"

using std::vector;
using std::fill;
using std::numeric_limits;
using std::min;
using std::max;

namespace ltr {
namespace utility {

template <typename TElement>
void calcMinMaxStatistics(const DataSet<TElement>& data_set,
    vector<double>* pMinFeatureValue,
    vector<double>* pMaxFeatureValue) {

  pMinFeatureValue->resize(data_set.feature_count());
  pMaxFeatureValue->resize(data_set.feature_count());

  fill(pMinFeatureValue->begin(),
      pMinFeatureValue->end(),
      numeric_limits<double>::max());

  fill(pMaxFeatureValue->begin(),
      pMaxFeatureValue->end(),
      numeric_limits<double>::min());

  for (size_t element_idx = 0; element_idx < data_set.size(); ++element_idx) {
    for (size_t obj_idx = 0;
        obj_idx < data_set[element_idx].size();
        ++obj_idx) {
      for (size_t feature_index = 0;
          feature_index < data_set.feature_count();
          ++feature_index) {
        pMinFeatureValue->at(feature_index) = min(
            pMinFeatureValue->at(feature_index),
            data_set[element_idx][obj_idx].features()[feature_index]);
        pMaxFeatureValue->at(feature_index) = max(
            pMaxFeatureValue->at(feature_index),
            data_set[element_idx][obj_idx].features()[feature_index]);
      }
    }
  }
}

template void calcMinMaxStatistics<Object>(const DataSet<Object>& data_set,
    vector<double>* pMinFeatureValue,
    vector<double>* pMaxFeatureValue);

template void calcMinMaxStatistics<ObjectPair>(
    const DataSet<ObjectPair>& data_set,
    vector<double>* pMinFeatureValue,
    vector<double>* pMaxFeatureValue);

template void calcMinMaxStatistics<ObjectList>(
    const DataSet<ObjectList>& data_set,
    vector<double>* pMinFeatureValue,
    vector<double>* pMaxFeatureValue);
}
}
