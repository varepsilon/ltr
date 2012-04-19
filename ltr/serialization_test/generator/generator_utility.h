// Copyright 2012 Yandex

#ifndef LTR_SERIALIZATION_TEST_GENERATOR_GENERATOR_UTILITY_H_
#define LTR_SERIALIZATION_TEST_GENERATOR_GENERATOR_UTILITY_H_

#include <string>
#include "ltr/data/data_set.h"
#include "ltr/learners/learner.h"

using std::string;
using ltr::DataSet;
using ltr::Object;
using ltr::BaseLearner;

namespace serialization_test {
  class Generator {
  public:
    Generator();
    string code() const;
    void write(const char* filename) const;
    void setScorerTest(BaseLearner<Object>::Ptr learner,
      string test_name);

    const DataSet<Object> train_data;
    const DataSet<Object> test_data;
  private:
    string setIncludes() const;
    string setFixture() const;
    string setTestLabelsFunction(string function_name) const;
    string setTestCode(int index,
      string test_name) const;
    string setBeginBlockComment(string message) const;
    string setEndBlockComment(string message) const;

    string tester_code;
    int scorers_to_test;
  };
};

#endif  // LTR_SERIALIZATION_TEST_GENERATOR_GENERATOR_UTILITY_H_
