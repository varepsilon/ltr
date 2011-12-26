// Copyright 2011 Yandex

#include "ltr/data/utility/data_set_statistics.h"

namespace ltr {
namespace utility {

template <typename TElement>
void calcMinMaxStatistics(const DataSet<TElement>& data_set,
    std::vector<double>* pMinFeatureValue,
    std::vector<double>* pMaxFeatureValue) {

  pMinFeatureValue->resize(data_set.featureCount());
  pMaxFeatureValue->resize(data_set.featureCount());

  std::fill(pMinFeatureValue->begin(),
      pMinFeatureValue->end(),
      std::numeric_limits<double>::max());

  std::fill(pMaxFeatureValue->begin(),
      pMaxFeatureValue->end(),
      std::numeric_limits<double>::min());

  for (size_t element_idx = 0; element_idx < data_set.size(); ++element_idx) {
    for (size_t obj_idx = 0;
        obj_idx < data_set[element_idx].size();
        ++obj_idx) {
      for (size_t feature_idx = 0;
          feature_idx < data_set.featureCount();
          ++feature_idx) {
        pMinFeatureValue->at(feature_idx) = std::min(
            pMinFeatureValue->at(feature_idx),
            data_set[element_idx][obj_idx].features()[feature_idx]);
        pMaxFeatureValue->at(feature_idx) = std::max(
            pMaxFeatureValue->at(feature_idx),
            data_set[element_idx][obj_idx].features()[feature_idx]);
      }
    }
  }
}

template void calcMinMaxStatistics<Object>(const DataSet<Object>& data_set,
    std::vector<double>* pMinFeatureValue,
    std::vector<double>* pMaxFeatureValue);

template void calcMinMaxStatistics<ObjectPair>(
    const DataSet<ObjectPair>& data_set,
    std::vector<double>* pMinFeatureValue,
    std::vector<double>* pMaxFeatureValue);

template void calcMinMaxStatistics<ObjectList>(
    const DataSet<ObjectList>& data_set,
    std::vector<double>* pMinFeatureValue,
    std::vector<double>* pMaxFeatureValue);
}
}
