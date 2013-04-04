// Copyright 2011 Yandex

#include <cstdlib>

#include "gtest/gtest.h"

#include "ltr/data/data_set.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/scorers/fake_scorer.h"
#include "ltr/utility/boost/lexical_cast.h"
#include "ltr/utility/boost/path.h"
#include "ltr/utility/numerical.h"
#include "ltr/utility/random_number_generator.h"

using std::string;

using ltr::DataSet;
using ltr::FakeScorer;
using ltr::io_utility::loadDataSet;
using ltr::io_utility::saveDataSet;
using ltr::io_utility::savePredictions;
using ltr::Object;
using ltr::ObjectList;
using ltr::ObjectPair;
using ltr::Scorer;
using ltr::utility::FixPathSeparators;
using ltr::utility::lexical_cast;
using ltr::utility::NaN;
using ltr::utility::randomizer;

int last_random = 2981984;

// The fixture for testing (contains data for tests).
class IOUtilityTest : public ::testing::Test {
  protected:
  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
    svm_arff_test_file_name =
        FixPathSeparators("data/tests/svm/arff_test.txt");
    arff_arff_test_file_name =
        FixPathSeparators("data/tests/arff/arff_test.txt");
    arff_input_test_file_name =
        FixPathSeparators("data/tests/arff/arff_in_data.txt");
    arff_output_test_file_name =
        FixPathSeparators("data/tests/arff/arff_out_data.txt");
    tmp_file_name =
        FixPathSeparators("tmp_file");
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  string svm_arff_test_file_name;
  string arff_arff_test_file_name;
  string arff_input_test_file_name;
  string arff_output_test_file_name;
  string tmp_file_name;
};

TEST_F(IOUtilityTest, TestingSVMParser) {
  DataSet<Object> data;

  const int OBJECT_COUNT = 145;
  const int FEATURE_COUNT = 3;
  for (int object_index = 0; object_index < OBJECT_COUNT; ++object_index) {
    Object obj;
    for (int feature_index = 0; feature_index < FEATURE_COUNT; feature_index++)
      obj << static_cast<double>(randomizer.rand()) / 15332;
    obj.set_actual_label(static_cast<double>(randomizer.rand()) / 15332);
    data << obj;
  }
  saveDataSet(data, tmp_file_name, "SVMLIGHT");

  DataSet<Object> data2 = loadDataSet<Object>(tmp_file_name, "SVMLIGHT");
  EXPECT_TRUE(data2 == data);
  const int max_list_size = 15;
  const int min_list_size = 5;
  DataSet<ObjectList> list_data;
  for (int query_id = 0; query_id < OBJECT_COUNT; ++query_id) {
    int list_size = randomizer.rand() %
      (1 + max_list_size - min_list_size) + min_list_size;
    ObjectList lst;
    for (int object_index = 0; object_index < list_size; ++object_index) {
      Object obj;
      obj.setMetaInfo("queryId", lexical_cast<string>(query_id));
      for (int feature_index = 0;
           feature_index < FEATURE_COUNT;
           ++feature_index) {
        obj << static_cast<double>(randomizer.rand()) / 15332;
      }
      obj.set_actual_label(0);
      lst << obj;
    }
    list_data << lst;
  }

  saveDataSet(list_data, tmp_file_name, "SVMLIGHT");
  EXPECT_TRUE(
      loadDataSet<ObjectList>(tmp_file_name, "SVMLIGHT") == list_data);
}

TEST_F(IOUtilityTest, TestingARFFParser) {
  DataSet<Object> data;
  const int OBJECT_COUNT = 145;
  const int FEATURE_COUNT = 3;
  for (int object_index = 0; object_index < OBJECT_COUNT; ++object_index) {
    Object obj;
    for (int feature_index = 0;
         feature_index < FEATURE_COUNT;
         ++feature_index) {
      obj << static_cast<double>(randomizer.rand()) / 15332;
    }
    obj.set_actual_label(static_cast<double>(randomizer.rand()) / 15332);
    data << obj;
  }

  saveDataSet(data, tmp_file_name, "ARFF");
  DataSet<Object> data2 = loadDataSet<Object>(tmp_file_name, "ARFF");
  EXPECT_TRUE(data2 == data);
}

TEST_F(IOUtilityTest, TestingDSVParser) {
  DataSet<Object> data;
  const int OBJECT_COUNT = 145;
  const int FEATURE_COUNT = 3;
  for (int object_index = 0; object_index < OBJECT_COUNT; ++object_index) {
    Object obj;
    for (int feature_index = 0;
         feature_index < FEATURE_COUNT;
         ++feature_index) {
      obj << static_cast<double>(randomizer.rand()) / 15332;
    }
    data << obj;
  }

  saveDataSet(data, tmp_file_name, "CSV");
  DataSet<Object> data2 = loadDataSet<Object>(tmp_file_name, "CSV");
  EXPECT_TRUE(data2 == data);

  saveDataSet(data, tmp_file_name, "TSV");
  data2 = loadDataSet<Object>(tmp_file_name, "TSV");
  EXPECT_TRUE(data2 == data);

  saveDataSet(data, tmp_file_name, ":SV");
  data2 = loadDataSet<Object>(tmp_file_name, ":SV");
  EXPECT_TRUE(data2 == data);
}

TEST_F(IOUtilityTest, TestingYandexParser) {
  DataSet<Object> data;
  const int OBJECT_COUNT = 145;
  const int FEATURE_COUNT = 3;
  for (int object_index = 0; object_index < OBJECT_COUNT; ++object_index) {
    Object obj;
    for (int feature_index = 0;
         feature_index < FEATURE_COUNT;
         feature_index++) {
      obj << static_cast<double>(randomizer.rand()) / 15332;
    }
    obj.set_actual_label(static_cast<double>(randomizer.rand()) / 15332);
    data << obj;
  }
  saveDataSet(data, tmp_file_name, "yandex");
  EXPECT_EQ(loadDataSet<Object>(tmp_file_name, "yandex"), data);

  const int max_list_size = 15;
  const int min_list_size = 5;
  DataSet<ObjectList> list_data;
  for (int query_id = 0; query_id < OBJECT_COUNT; ++query_id) {
    int list_size = randomizer.rand() %
      (1 + max_list_size - min_list_size) + min_list_size;
    ObjectList lst;
    for (int object_index = 0; object_index < list_size; ++object_index) {
      Object obj;
      obj.setMetaInfo("queryId",
                      lexical_cast<string>(query_id));
      for (int feature_index = 0;
           feature_index < FEATURE_COUNT;
           ++feature_index) {
        obj << static_cast<double>(randomizer.rand()) / 15332;
      }
      obj.set_actual_label(0);
      lst << obj;
    }
    list_data << lst;
  }

  saveDataSet(list_data, tmp_file_name, "yandex");
  EXPECT_EQ(loadDataSet<ObjectList>(tmp_file_name, "yandex"),
      list_data);
}

TEST_F(IOUtilityTest, TestingSavePredictions) {
  DataSet<Object> data;
  const int OBJECT_COUNT = 145;
  const int FEATURE_COUNT = 3;
  for (int object_index = 0; object_index < OBJECT_COUNT; ++object_index) {
    Object obj;
    for (int feature_index = 0;
         feature_index < FEATURE_COUNT;
         ++feature_index) {
      obj << static_cast<double>(randomizer.rand()) / 15332;
    }
    data << obj;
  }
  EXPECT_NO_THROW(savePredictions(data,
      Scorer::Ptr(new FakeScorer()), tmp_file_name));
}
