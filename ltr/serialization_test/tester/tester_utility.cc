// Copyright 2012 Yandex

#include <cmath>
#include <iostream>

#include "ltr/serialization_test/tester/tester_utility.h"

using std::fabs;
using std::cout;

namespace serialization_test {
bool Equal(const vector<double>& left, const vector<double>& right) {
  if (left.size() != right.size()) {
    return false;
  }

  for (int i = 0; i < (int)left.size(); ++i) {
    if (fabs(left[i] - right[i]) > COMPARE_EPS) {
      return false;
    }
  }

  return true;
}

bool Equal(
    const vector<vector<double> >& left,
    const vector<vector<double> >& right) {
  if (left.size() != right.size()) {
    return false;
  }

  for (int i = 0; i < (int)left.size(); ++i) {
    if (left[i].size() != right[i].size())
      return false;
    for (int j = 0; j < (int)left[i].size(); ++j) {
      if (fabs(left[i][j] - right[i][j]) > COMPARE_EPS) {
        return false;
      }
    }
  }

  return true;
}
};
