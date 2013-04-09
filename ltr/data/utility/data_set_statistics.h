// Copyright 2012 Yandex

#ifndef LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_
#define LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_

#include <vector>

#include "ltr/data/utility/data_set_label_statistics.h"
#include "ltr/data/data_set.h"
#include "ltr/data/per_object_accessor.h"

using std::vector;

namespace ltr {
namespace utility {
/**
 * \brief Calculates min and max features for given dataset.
 * \param dataset input dataset for analisys
 * \param min_features_values vector of minimum values 
 * \param max_features_values vector of maximum values 
 */
template <typename TElement>
void getFeaturesMinMaxValues(const DataSet<TElement>& dataset,
                             vector<double>* min_features_values,
                             vector<double>* max_features_values);

/**
 * \brief Calculates average value of each feature for given dataset.
 * \param dataset input dataset for analisys.
 * \param average_values vector of average values.
 */
template <typename TElement>
void getFeaturesAverageValues(const DataSet<TElement>& dataset,
                              vector<double>* average_values) {
  average_values->resize(dataset.feature_count());
  fill(average_values->begin(), average_values->end(), 0);

  if ((int)dataset.feature_count() == 0) {
    return;
  }

  for (int element_index = 0;
       element_index < (int)dataset.size();
       ++element_index) {
    PerObjectAccessor<const TElement>
      per_object_accessor(&dataset[element_index]);
    for (int object_index = 0;
         object_index < (int)per_object_accessor.object_count();
         ++object_index) {
      for (int feature_index = 0;
           feature_index < (int)dataset.feature_count();
           ++feature_index) {
        average_values->at(feature_index) +=
            per_object_accessor.object(object_index)[feature_index];
      }
    }
  }

  for (int feature_index = 0;
       feature_index < (int)dataset.feature_count();
       ++feature_index) {
    average_values->at(feature_index) /= (int)dataset.size();
  }
}

/**
 * \brief Calculates number of ltr::Object in dataset.
 * \param data_set input dataset.
 */
template <class TElement>
int getDataSetObjectCount(const DataSet<TElement>& data_set) {
  int object_count = 0;
  for (int element_index = 0;
       element_index < data_set.size();
       ++element_index) {
    object_count += PerObjectAccessor<const TElement>(&data_set[element_index]).
      object_count();
  }
  return object_count;
}

double getDataSetEntropy(const LabelStatisticComputer& label_stat_computer);

template <typename TElement>
double getDataSetEntropy(const DataSet<TElement>& data);
};
};

#endif  // LTR_DATA_UTILITY_DATA_SET_STATISTICS_H_
