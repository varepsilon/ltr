// Copyright 2011 Yandex

#include <cstdlib>

#include "gtest/gtest.h"

#include "ltr/data/data_set.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/scorers/fake_scorer.h"
#include "ltr/utility/random_number_generator.h"
#include "ltr/utility/boost/lexical_cast.h"
#include "ltr/utility/boost/path.h"

using ltr::DataSet;
using ltr::io_utility::loadDataSet;
using ltr::io_utility::saveDataSet;
using ltr::Object;
using ltr::ObjectList;
using ltr::ObjectPair;
using ltr::utility::randomizer;
using ltr::utility::FixPathSeparators;

int last_random = 2981984;

// The fixture for testing (contains data for tests).
class IOUtilityTest : public ::testing::Test {
  protected:
  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
    svm_arff_test_file_name = FixPathSeparators("data/tests/svm/arff_test.txt");
    arff_arff_test_file_name = FixPathSeparators("data/tests/arff/arff_test.txt");
    tmp_file_name = FixPathSeparators("tmp_file");
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  std::string svm_arff_test_file_name;
  std::string arff_arff_test_file_name;
  std::string tmp_file_name;
};

TEST_F(IOUtilityTest, TestingSVMPArser) {
  DataSet<Object> data;
  const int N = 145;
  const int featureN = 3;
  for (int i = 0; i < N; ++i) {
    Object obj;
    for (int j = 0; j < featureN; j++)
      obj << static_cast<double>(randomizer.rand()) / 15332;
    data << obj;
  }

  saveDataSet(data, tmp_file_name, "SVMLIGHT");
  DataSet<Object> data2 = loadDataSet<ltr::Object>(tmp_file_name, "SVMLIGHT");
  EXPECT_EQ(data2, data);

  const int max_list_size = 15;
  const int min_list_size = 5;
  DataSet<ObjectList> list_data;
  for (int i = 0; i < N; ++i) {
    int cn = randomizer.rand() %
      (1 + max_list_size - min_list_size) + min_list_size;
    ObjectList lst;
    for (int j = 0; j < cn; j++) {
      Object obj;
      obj.setMetaInfo("queryId", ltr::utility::lexical_cast<std::string>(i));
      for (int j = 0; j < featureN; j++)
        obj << static_cast<double>(randomizer.rand()) / 15332;
      lst << obj;
    }
    list_data << lst;
  }

  saveDataSet(list_data, tmp_file_name, "SVMLIGHT");
  EXPECT_EQ(loadDataSet<ltr::ObjectList>(tmp_file_name, "SVMLIGHT"), list_data);
}

TEST_F(IOUtilityTest, TestingYandexPArser) {
  DataSet<Object> data;
  const int N = 145;
  const int featureN = 3;
  for (int i = 0; i < N; ++i) {
    Object obj;
    for (int j = 0; j < featureN; j++)
      obj << static_cast<double>(randomizer.rand()) / 15332;
    data << obj;
  }
  saveDataSet(data, tmp_file_name, "yandex");
  EXPECT_EQ(loadDataSet<ltr::Object>(tmp_file_name, "yandex"), data);

  const int max_list_size = 15;
  const int min_list_size = 5;
  DataSet<ObjectList> list_data;
  for (int i = 0; i < N; ++i) {
    int cn = randomizer.rand() %
      (1 + max_list_size - min_list_size) + min_list_size;
    ObjectList lst;
    for (int j = 0; j < cn; j++) {
      Object obj;
      obj.setMetaInfo("queryId", ltr::utility::lexical_cast<std::string>(i));
      for (int j = 0; j < featureN; j++)
        obj << static_cast<double>(randomizer.rand()) / 15332;
      lst << obj;
    }
    list_data << lst;
  }

  saveDataSet(list_data, tmp_file_name, "yandex");
  EXPECT_EQ(loadDataSet<ltr::ObjectList>(tmp_file_name, "yandex"),
      list_data);
}

TEST_F(IOUtilityTest, TestingARFFPArser) {
  DataSet<Object> arff_data =
      loadDataSet<ltr::Object>(arff_arff_test_file_name, "arff");

  DataSet<Object> svm_data =
      loadDataSet<ltr::Object>(svm_arff_test_file_name, "SVMLIGHT");

  EXPECT_EQ(svm_data, arff_data);
}

TEST_F(IOUtilityTest, TestingSavePredictions) {
  DataSet<Object> data;
  const int N = 145;
  const int featureN = 3;
  for (int i = 0; i < N; ++i) {
    Object obj;
    for (int j = 0; j < featureN; j++)
      obj << static_cast<double>(randomizer.rand()) / 15332;
    data << obj;
  }
  EXPECT_NO_THROW(ltr::io_utility::savePredictions(data,
      ltr::Scorer::Ptr(new ltr::FakeScorer()), tmp_file_name));
}
