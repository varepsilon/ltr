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
void getFeaturesMinMaxValues(const DataSet<TElement>& dataset,
                             VectorXd* min_features_values,
                             VectorXd* max_features_values) {
  min_features_values->resize(dataset.feature_count());
  max_features_values->resize(dataset.feature_count());

  min_features_values->setConstant(numeric_limits<double>::max());
  max_features_values->setConstant(-numeric_limits<double>::max());

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
        min_features_values->operator()(feature_index) = min(
            min_features_values->operator()(feature_index),
            per_object_accessor.object(object_index)[feature_index]);
        max_features_values->operator()(feature_index) = max(
            max_features_values->operator()(feature_index),
            per_object_accessor.object(object_index)[feature_index]);
      }
    }
  }
}

template void
  getFeaturesMinMaxValues<Object>(const DataSet<Object>& data_set,
                                  VectorXd* min_features_values,
                                  VectorXd* max_features_values);

template void
  getFeaturesMinMaxValues<ObjectPair>(const DataSet<ObjectPair>& data_set,
                                      VectorXd* min_features_values,
                                      VectorXd* max_features_values);

template void
  getFeaturesMinMaxValues<ObjectList>(const DataSet<ObjectList>& data_set,
                                      VectorXd* min_features_values,
                                      VectorXd* max_features_values);

double getDataSetEntropy(const LabelStatisticComputer& label_stat_computer) {
  double entropy = 0.0;
  double data_size = label_stat_computer.getHoldingDataSize();
  for (int label_index = 0;
       label_index < label_stat_computer.getLabelsCount(); ++label_index) {
    double label_weight = label_stat_computer.getLabelWeightById(label_index);
    if (!DoubleEqual(label_weight, 0.0)) {
      double class_prior_probability = label_weight / data_size;
      entropy -= class_prior_probability * log(class_prior_probability);
    }
  }
  return entropy;
}

template <typename TElement>
double getDataSetEntropy(const DataSet<TElement>& data) {
  return getDataSetEntropy(LabelStatisticComputer(data));
}
};
};
