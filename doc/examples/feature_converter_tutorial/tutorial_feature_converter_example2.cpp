#include <iostream>

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/feature_normalizer_learner.h"
#include "ltr/utility/indices.h"

using std::cout;
using std::endl;

using ltr::Object;
using ltr::DataSet;
using ltr::FeatureConverter;
using ltr::FeatureNormalizerLearner;

using ltr::utility::Indices;
using ltr::utility::IndicesPtr;

int main() {
  Object object1, object2, object3;

  object1 << 5.234 << 1.533 << -5.378;
  object2 << 2.213 << -1.234 << 0.323;
  object3 << -2.123 << 0.207 << -6.085;

  DataSet<Object> data, converted_data;
  data.add(object1);
  data.add(object2);
  data.add(object3);

  FeatureNormalizerLearner<Object> feature_normalizer_learner;
  feature_normalizer_learner.set_min(-1.0);
  feature_normalizer_learner.set_max(1.0);
  feature_normalizer_learner.learn(data);

  FeatureConverter::Ptr converter = feature_normalizer_learner.make();
  converter->apply(data, &converted_data);

  cout << "Before:" << endl << data << endl;
  cout << "After:" << endl << converted_data << endl;

  return 0;
}