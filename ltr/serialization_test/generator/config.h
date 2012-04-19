// Copyright 2012 Yandex

#ifndef LTR_SERIALIZATION_TEST_GENERATOR_CONFIG_H_
#define LTR_SERIALIZATION_TEST_GENERATOR_CONFIG_H_

#include <boost/filesystem/path.hpp>

#include <string>

using std::string;

namespace serialization_test {
  string TrainDataPath() {
    boost::filesystem::path data_path("data/serialization_tests/svm1.txt");
    return data_path.string();
  }
  string TestDataPath() {
    boost::filesystem::path data_path("data/serialization_tests/svm2.txt");
    return data_path.string();
  }
};

#endif  // LTR_SERIALIZATION_TEST_GENERATOR_CONFIG_H_
