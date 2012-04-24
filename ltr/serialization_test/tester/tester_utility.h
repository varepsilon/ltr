// Copyright 2012 Yandex

#ifndef LTR_SERIALIZATION_TEST_TESTER_TESTER_UTILITY_H_
#define LTR_SERIALIZATION_TEST_TESTER_TESTER_UTILITY_H_

#include <ostream>
#include <vector>
#include <string>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

using std::vector;
using std::string;
using std::ostream;
using ltr::DataSet;
using ltr::Object;

namespace serialization_test {
  const double COMPARE_EPS = 1e-10;

  vector<double> ApplySerializatedScorerToDataSet(
    const DataSet<Object>& dataset,
    double (*scorer)(const vector<double>& features));

  bool Equal(const vector<double>& left, const vector<double>& right);

  struct Report {
    vector<double> first;
    vector<double> second;
    Report(const vector<double>& in_first, const vector<double>& in_second)
      : first(in_first), second(in_second) {}
  };

  ostream& operator<<(ostream& output, const Report& rep);
};

#endif  // LTR_SERIALIZATION_TEST_TESTER_TESTER_UTILITY_H_
