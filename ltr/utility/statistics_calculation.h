// Copyright 2012 Yandex

#ifndef LTR_UTILITY_STATISTICS_CALCULATION_H_
#define LTR_UTILITY_STATISTICS_CALCULATION_H_

#include <map>
#include <vector>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/utility/indices.h"
#include "ltr/data/per_object_accessor.h"

using std::map;
using std::vector;

using ltr::Object;
using ltr::DataSet;
using ltr::utility::Indices;

namespace ltr {
namespace utility {

typedef map<double, int> LabelToCapacity;

/**
 * Function calculates number of objects with the same label
 * in the given data set
 */
template<class TElement>
LabelToCapacity CalculateLabelsCapacity(const DataSet<TElement>& data) {
  map<double, int> result;
  for (int element_index = 0; element_index < data.size(); ++element_index) {
    PerObjectAccessor<const TElement> per_object_accessor(&data[element_index]);
    for (int object_index = 0;
         object_index < per_object_accessor.object_count();
         ++object_index) {
      ++result[per_object_accessor.object(object_index).actual_label()];
    }
  }
  return result;
}

/**
 * Function splits data set to particular data sets by their actual labels
 */
template<class TElement>
void SplitDataSetByActualLabel(
    const DataSet<TElement>& data,
    vector<DataSet<Object> > *splitted_data_set) {
  map<double, DataSet<Object> > subsets_elements;
  for (int element_index = 0; element_index < data.size(); ++element_index) {
    PerObjectAccessor<const TElement> per_object_accessor(&data[element_index]);
    for (int object_index = 0;
         object_index < per_object_accessor.object_count();
         ++object_index) {
      double label = per_object_accessor.object(object_index).actual_label();
      subsets_elements[label].add(per_object_accessor.object(object_index));
    }
  }

  splitted_data_set->clear();
  for (map<double, DataSet<Object> >::iterator subsets_elements_iterator =
         subsets_elements.begin();
       subsets_elements_iterator != subsets_elements.end();
       ++subsets_elements_iterator) {
    splitted_data_set->push_back(subsets_elements_iterator->second);
  }
}

/**
 * Function calculates the overall mean of the given data set
 */
template<class TElement>
void CalculateFeaturesMean(const DataSet<TElement>& data,
                           VectorXd* mean) {
  PerObjectAccessor<const TElement> per_object_accessor(&data[0]);
  int number_of_features = per_object_accessor.object(0).feature_count();

  mean->setZero(number_of_features);
  int data_size = 0;

  for (int element_index = 0; element_index < data.size(); ++element_index) {
    PerObjectAccessor<const TElement> per_object_accessor(&data[element_index]);
    for (int object_index = 0;
         object_index < per_object_accessor.object_count();
         ++object_index) {
      ++data_size;
      for (int feature_index = 0;
           feature_index < number_of_features;
           ++feature_index) {
        (*mean)[feature_index] +=
          per_object_accessor.object(object_index)[feature_index];
      }
    }
  }

  for (int feature_index = 0;
       feature_index < number_of_features;
       ++feature_index) {
    (*mean)[feature_index] /= data_size;
  }
}

/**
 * Function calulates the overall variance of the given data set
 */
template<class TElement>
void CalculateFeaturesVariance(const DataSet<TElement>& data,
                               VectorXd* variance) {
  VectorXd mean;
  CalculateFeaturesMean(data, &mean);

  int number_of_features = data[0].feature_count();
  variance->setZero(number_of_features);
  int data_size = 0;

  for (int element_index = 0; element_index < data.size(); ++element_index) {
    PerObjectAccessor<const TElement> per_object_accessor(&data[element_index]);
    for (int object_index = 0;
         object_index < per_object_accessor.object_count();
         ++object_index) {
      ++data_size;
      for (int feature_index = 0;
           feature_index < number_of_features;
           ++feature_index) {
        (*variance)[feature_index] +=
          pow(per_object_accessor.object(object_index)[feature_index] -
                mean[feature_index], 2.0);
      }
    }
  }

  for (int feature_index = 0;
       feature_index < number_of_features;
       ++feature_index) {
    (*variance)[feature_index] /= data_size;
  }
}

/**
 * Function calculates means of every class in the given data set
 */
template<class TElement>
static void CalculateActualLabelToFeaturesMean(
    const DataSet<TElement>& data,
    map<double, VectorXd >* result) {

  vector<DataSet<Object> > splitted_data;
  SplitDataSetByActualLabel(data, &splitted_data);

  result->clear();
  for (int data_set_index = 0;
       data_set_index < splitted_data.size();
       ++data_set_index) {
    CalculateFeaturesMean(
      splitted_data[data_set_index],
      (&(*result)[splitted_data[data_set_index][0].actual_label()]));
  }
}
};
};

#endif  // LTR_UTILITY_STATISTICS_CALCULATION_H_
