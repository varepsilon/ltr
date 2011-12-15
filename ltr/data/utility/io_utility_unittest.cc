// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>
#include <cstdlib>

#include "gtest/gtest.h"

#include "ltr/data/utility/io_utility.h"
#include "ltr/data/utility/nominal_feature_handler.h"
#include "ltr/scorers/fake_scorer.h"

using ltr::io_utility::loadDataSet;
using ltr::io_utility::saveDataSet;
using ltr::DataSet;
using ltr::Object;
using ltr::ObjectList;
using ltr::ObjectPair;

int last_random = 2981984;

int rand_r() {
  return last_random = (last_random * 11 + 23421) % 138274123;
}

// The fixture for testing (contains data for tests).
class IOUtilityTest : public ::testing::Test {
    protected:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
};

TEST_F(IOUtilityTest, TestingSVMPArser) {
  DataSet<Object> data;
  const int N = 145;
  const int featureN = 3;
  for (int i = 0; i < N; i++) {
    Object obj;
    for (int j = 0; j < featureN; j++)
      obj << static_cast<double>(rand_r()) / 15332;
    data << obj;
  }

  saveDataSet(data, "tmp_file", "svmlite");
  assert(data == loadDataSet<ltr::Object>("tmp_file", "svmlite"));

  const int max_list_size = 15;
  const int min_list_size = 5;
  DataSet<ObjectList> list_data;
  for (int i = 0; i < N; i++) {
    int cn = rand_r() % (1 + max_list_size - min_list_size) + min_list_size;
    ObjectList lst;
    for (int j = 0; j < cn; j++) {
      Object obj;
      obj.setMetaInfo("queryId", boost::lexical_cast<std::string>(i));
      for (int j = 0; j < featureN; j++)
        obj << static_cast<double>(rand_r()) / 15332;
      lst << obj;
    }
    list_data << lst;
  }

  saveDataSet(list_data, "tmp_file", "svmlite");
  assert(list_data == loadDataSet<ObjectList>("tmp_file", "svmlite"));
}

TEST_F(IOUtilityTest, TestingYandexPArser) {
  DataSet<Object> data;
  const int N = 145;
  const int featureN = 3;
  for (int i = 0; i < N; i++) {
    Object obj;
    for (int j = 0; j < featureN; j++)
      obj << static_cast<double>(rand_r()) / 15332;
    data << obj;
  }
  saveDataSet(data, "tmp_file", "yandex");
  assert(data == loadDataSet<ltr::Object>("tmp_file", "yandex"));

  const int max_list_size = 15;
  const int min_list_size = 5;
  DataSet<ObjectList> list_data;
  for (int i = 0; i < N; i++) {
    int cn = rand_r() % (1 + max_list_size - min_list_size) + min_list_size;
    ObjectList lst;
    for (int j = 0; j < cn; j++) {
      Object obj;
      obj.setMetaInfo("queryId", boost::lexical_cast<std::string>(i));
      for (int j = 0; j < featureN; j++)
        obj << static_cast<double>(rand_r()) / 15332;
      lst << obj;
    }
    list_data << lst;
  }

  saveDataSet(list_data, "tmp_file", "yandex");
  assert(list_data == loadDataSet<ObjectList>("tmp_file", "yandex"));
}
