// Copyright 2012 Yandex

#ifndef SERIALIZATION_TEST_TESTER_TESTER_UTILITY_H_
#define SERIALIZATION_TEST_TESTER_TESTER_UTILITY_H_

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

  bool CompareReport(const vector<double>& expected,
    const vector<double>& actual,
    const string& message);

  int TotalReport(bool ok);
};

#endif  // SERIALIZATION_TEST_TESTER_TESTER_UTILITY_H_
