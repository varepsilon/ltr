// Copyright 2011 Yandex

#include <limits>
#include <vector>

#include "ltr/data/utility/data_set_statistics.h"

using std::fill;
using std::max;
using std::min;
using std::numeric_limits;

namespace ltr {
namespace utility {

template <typename TElement>
void getFeaturesValuesRanges(const DataSet<TElement>& dataset,
                             vector<double>* min_features_values,
                             vector<double>* max_features_values) {

  min_features_values->resize(dataset.featureCount());
  max_features_values->resize(dataset.featureCount());

  fill(min_features_values->begin(),
       min_features_values->end(),
       numeric_limits<double>::max());

  fill(max_features_values->begin(),
       max_features_values->end(),
       numeric_limits<double>::min());

  for (int element_index = 0;
       element_index < (int)dataset.size();
       ++element_index) {
    for (int object_index = 0;
         object_index < (int)dataset[element_index].size();
         ++object_index) {
      for (int feature_index = 0;
           feature_index < (int)dataset.featureCount();
           ++feature_index) {
        min_features_values->at(feature_index) = min(
            min_features_values->at(feature_index),
            dataset[element_index][object_index].features()[feature_index]);
        max_features_values->at(feature_index) = max(
            max_features_values->at(feature_index),
            dataset[element_index][object_index].features()[feature_index]);
      }
    }
  }
}

template void
  getFeaturesValuesRanges<Object>(const DataSet<Object>& data_set,
                                  vector<double>* min_features_values,
                                  vector<double>* max_features_values);

template void
  getFeaturesValuesRanges<ObjectPair>(const DataSet<ObjectPair>& data_set,
                                      vector<double>* min_features_values,
                                      vector<double>* max_features_values);

template void
  getFeaturesValuesRanges<ObjectList>(const DataSet<ObjectList>& data_set,
                                      vector<double>* min_features_values,
                                      vector<double>* max_features_values);
}
}
