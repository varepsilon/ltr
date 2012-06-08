// Copyright 2012 Yandex

#include "ltr/feature_converters/utility/utility.h"

namespace ltr {
namespace utility {

void ApplyFeatureConverter(FeatureConverter::ConstPtr converter,
                           const PointwiseDataSet& source_dataset,
                           PointwiseDataSet* converted_dataset) {
  PointwiseDataSet result(converter->output_feature_info());
  for (int object_index = 0;
       object_index < (int)source_dataset.size();
       ++object_index) {
      Object converted_object;
      converter->apply(source_dataset[object_index][0], &converted_object);
      result.add(converted_object, source_dataset.getWeight(object_index));
  }
  *converted_dataset = result;
}

void ApplyFeatureConverter(FeatureConverter::ConstPtr converter,
                           const PairwiseDataSet& source_dataset,
                           PairwiseDataSet* converted_dataset) {
  PairwiseDataSet result(converter->output_feature_info());
  for (int pair_index = 0; pair_index < source_dataset.size(); ++pair_index) {
    Object first_converted_object;
    Object second_converted_object;
    converter->apply(source_dataset[pair_index][0], &first_converted_object);
    converter->apply(source_dataset[pair_index][1], &second_converted_object);
    ObjectPair converted_pair(first_converted_object,
                              second_converted_object);
    result.add(converted_pair, source_dataset.getWeight(pair_index));
  }
  *converted_dataset = result;
}

void ApplyFeatureConverter(FeatureConverter::ConstPtr converter,
                           const ListwiseDataSet& source_dataset,
                           ListwiseDataSet* converted_dataset) {
  ListwiseDataSet result(converter->output_feature_info());
  for (int list_index = 0; list_index < source_dataset.size(); ++list_index) {
    ObjectList converted_list;
    for (int object_index = 0;
         object_index < (int)source_dataset[list_index].size();
         ++object_index) {
      Object converted_object;
      converter->apply(source_dataset[list_index][object_index],
                       &converted_object);
      converted_list << converted_object;
    }
    result.add(converted_list, source_dataset.getWeight(list_index));
  }
  *converted_dataset = result;
}
}
}
