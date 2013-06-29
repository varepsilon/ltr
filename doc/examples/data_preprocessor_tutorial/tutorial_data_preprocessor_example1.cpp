// Copyright 2012 Yandex

#include <iostream> // NOLINT

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"
#include "ltr/data_preprocessors/data_preprocessor.h"
#include "ltr/data_preprocessors/data_sampler.h"
#include "ltr/utility/indices.h"

using std::cout;
using std::endl;

using ltr::Object;
using ltr::DataSet;
using ltr::DataSampler;

using ltr::utility::Indices;
using ltr::utility::IndicesPtr;

int main() {
  Object object1, object2, object3;

  object1 << 5.234 << 1.533 << -5.378;
  object2 << 2.213 << -1.234 << 0.323;
  object3 << -2.123 << 0.207 << -6.085;

  Indices indices;
  indices.push_back(0);
  indices.push_back(2);

  DataSet<Object> data, preprocessed_data;
  data.add(object1);
  data.add(object2);
  data.add(object3);

  DataSampler<Object>::Ptr data_sampler(new DataSampler<Object>);
  data_sampler->set_indices(indices);
  data_sampler->apply(data, &preprocessed_data);

  cout << "Before:" << endl << data << endl;
  cout << "After:" << endl << preprocessed_data << endl;

  return 0;
}
