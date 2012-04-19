// Copyright 2012 Yandex

#ifndef LTR_SERIALIZATION_TEST_TESTER_TESTER_UTILITY_H_
#define LTR_SERIALIZATION_TEST_TESTER_TESTER_UTILITY_H_

#include <vector>
#include <string>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

using std::vector;
using std::string;
using ltr::DataSet;
using ltr::Object;

namespace serialization_test {
  const double COMPARE_EPS = 1e-10;

  vector<double> ApplySerializatedScorerToDataSet(
    const DataSet<Object>& dataset,
    double (*scorer)(const vector<double>& features));

  bool Equal(const vector<double>& left, const vector<double>& right);
};

#endif  // LTR_SERIALIZATION_TEST_TESTER_TESTER_UTILITY_H_
