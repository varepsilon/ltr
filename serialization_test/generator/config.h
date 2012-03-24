// Copyright 2012 Yandex

#ifndef SERIALIZATION_TEST_GENERATOR_CONFIG_H_
#define SERIALIZATION_TEST_GENERATOR_CONFIG_H_

#include <boost/filesystem/path.hpp>

#include <string>

using std::string;

namespace serialization_test {
  string TrainDataPath() {
    boost::filesystem::path data_path("data/svm1.txt");
    return data_path.string();
  }
  string TestDataPath() {
    boost::filesystem::path data_path("data/svm2.txt");
    return data_path.string();
  }
};

#endif  // SERIALIZATION_TEST_GENERATOR_CONFIG_H_
