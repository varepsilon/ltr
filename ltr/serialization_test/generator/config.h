// Copyright 2012 Yandex

#ifndef LTR_SERIALIZATION_TEST_GENERATOR_CONFIG_H_
#define LTR_SERIALIZATION_TEST_GENERATOR_CONFIG_H_

#include <string>

#include "ltr/utility/boost/path.h"

using std::string;
using ltr::utility::FixPathSeparators;

namespace serialization_test {
  /**
   * \brief Retuns path of train dataset.
   */
  string TrainDataPath() {
    return FixPathSeparators("data/serialization_tests/svm1.txt");
  }
  /**
   * \brief Retuns path of test dataset.
   */
  string TestDataPath() {
    return FixPathSeparators("data/serialization_tests/svm2.txt");
  }
};

#endif  // LTR_SERIALIZATION_TEST_GENERATOR_CONFIG_H_
