// Copyright 2012 Yandex

#ifndef SERIALIZATION_TEST_GENERATOR_GENERATOR_UTILITY_H_
#define SERIALIZATION_TEST_GENERATOR_GENERATOR_UTILITY_H_

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
    void setMainCode();
    void setScorerTest(BaseLearner<Object>::Ptr learner,
      string error_message);

    const DataSet<Object> train_data;
    const DataSet<Object> test_data;
  private:
    string setIncludes() const;
    string setTestLabelsFunction(string function_name) const;
    string setMessageFunction(string function_name,
      string error_message) const;

    string tester_code;
    int scorers_to_test;

    const string tab;
  };
};

#endif  // SERIALIZATION_TEST_GENERATOR_GENERATOR_UTILITY_H_
