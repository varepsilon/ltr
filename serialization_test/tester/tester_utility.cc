// Copyright 2012 Yandex

#include <cmath>
#include <iostream>

#include "serialization_test/tester/tester_utility.h"

using std::fabs;
using std::cout;

namespace serialization_test {
vector<double> ApplySerializatedScorerToDataSet(const DataSet<Object>& dataset,
    double (*scorer)(const vector<double>& features)) {
  vector<double> output;
  for (int i = 0; i < dataset.size(); ++i) {
    output.push_back((*scorer)(dataset[i].features()));
  }
  return output;
}

bool Equal(const vector<double>& left, const vector<double>& right) {
  if (left.size() != right.size()) {
    return false;
  }

  for (int i = 0; i < left.size(); ++i) {
    if (fabs(left[i] - right[i]) > COMPARE_EPS) {
      return false;
    }
  }

  return true;
}

bool CompareReport(const vector<double>& expected,
                   const vector<double>& actual,
                   const string& message) {
  bool result = Equal(expected, actual);
  if (!result) {
    cout << "[FAILED]";
    if (expected.size() != actual.size()) {
      cout << "Compared vectors have different sizes\n";
    } else {
      cout << "Expected Actual" << "\n";
      for (int i = 0; i < expected.size(); ++i) {
        cout << expected[i] << " " << actual[i] << "\n";
      }
    }
  } else {
    cout << "[OK]";
  }
  cout << " " << message << "\n";
  return result;
}

int TotalReport(bool ok) {
  if (ok) {
    cout << "\n[TOTAL:OK]\n";
    return 0;
  } else {
    cout << "\n[TOTAL:FAILED]\n";
    return 1;
  }
}
};
