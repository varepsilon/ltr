#include <iostream>

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/pca_learner.h"

using std::cout;
using std::endl;

using ltr::Object;
using ltr::DataSet;
using ltr::FeatureConverter;
using ltr::PCALearner;
using ltr::FixedNumberOfComponentsRule;

int main() {
  Object object1, object2, object3;

  object1 << 1 << 0 << 1;
  object2 << 0 << 1 << 1;
  object3 << 0 << 0 << 1;

  DataSet<Object> data, converted_data;
  data.add(object1);
  data.add(object2);
  data.add(object3);

  PCALearner<Object> pca_learner;
  pca_learner.setNumberOfComponentsRule(new FixedNumberOfComponentsRule(2));
  pca_learner.learn(data);
  FeatureConverter::Ptr converter = pca_learner.make();

  converter->apply(data, &converted_data);

  cout << "Before:" << endl << data << endl;
  cout << "Learner:" << endl << pca_learner.report() << endl;
  cout << "Feature converter:" << endl << (*converter) << endl;
  cout << "After:" << endl << converted_data << endl;

  return 0;
}

