// Copyright 2012 Yandex

#ifndef LTR_UTILITY_STATISTICS_CALCULATION_H_
#define LTR_UTILITY_STATISTICS_CALCULATION_H_

#include <map>
#include <vector>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/utility/indices.h"

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
    ++result[data[element_index].actual_label()];
  }
  return result;
}

/**
 * Function splits data set to particular data sets by their actual labels
 */
template<class TElement>
void SplitDataSetByActualLabel(
    const DataSet<TElement>& data,
    vector<DataSet<TElement> > *splitted_data_set) {
  map<double, Indices> subsets_indices;
  for (int element_index = 0; element_index < data.size(); ++element_index) {
    int label = data[element_index].actual_label();
    subsets_indices[label].push_back(element_index);
  }

  splitted_data_set->clear();
  for (map<double, Indices>::iterator subsets_indices_iterator =
         subsets_indices.begin();
       subsets_indices_iterator != subsets_indices.end();
       ++subsets_indices_iterator) {
    splitted_data_set->
      push_back(data.lightSubset(subsets_indices_iterator->second));
  }
}

/**
 * Function calculates the overall mean of the given data set
 */
template<class TElement>
void CalculateFeaturesMean(const DataSet<TElement>& data,
                           vector<double>* mean) {
  int number_of_features = data[0].feature_count();
  mean->assign(number_of_features, 0);
  for (int element_index = 0; element_index < data.size(); ++element_index) {
    for (int feature_index = 0;
         feature_index < number_of_features;
         ++feature_index) {
      (*mean)[feature_index] += data[element_index][feature_index];
    }
  }

  for (int feature_index = 0;
       feature_index < number_of_features;
       ++feature_index) {
    (*mean)[feature_index] /= data.size();
  }
}

/**
 * Function calulates the overall variance of the given data set
 */
template<class TElement>
void CalculateFeaturesVariance(const DataSet<TElement>& data,
                               vector<double>* variance) {
  vector<double> mean;
  CalculateFeaturesMean(data, &mean);

  int number_of_features = data[0].feature_count();
  variance->assign(number_of_features, 0);
  for (int element_index = 0; element_index < data.size(); ++element_index) {
    for (int feature_index = 0;
         feature_index < number_of_features;
         ++feature_index) {
      (*variance)[feature_index] +=
        pow(data[element_index][feature_index] - mean[feature_index], 2.0);
    }
  }

  for (int feature_index = 0;
       feature_index < number_of_features;
       ++feature_index) {
    (*variance)[feature_index] /= data.size();
  }
}

/**
 * Function calculates means of every class in the given data set
 */
template<class TElement>
static void CalculateActualLabelToFeaturesMean(
    const DataSet<TElement>& data,
    map<double, vector<double> >* result) {

  vector<DataSet<TElement> > splitted_data;
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
