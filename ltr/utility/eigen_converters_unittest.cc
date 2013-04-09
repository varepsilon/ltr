// Copyright 2013 Yandex

#include <gtest/gtest.h>

#include <limits>

#include "ltr/utility/eigen_converters.h"
#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/utility/numerical.h"

using std::numeric_limits;

using ltr::utility::StdVectorToEigenVector;
using ltr::utility::EigenVectorToStdVector;
using ltr::utility::DataSetToEigenMatrix;
using ltr::utility::InitEigenVector;
using ltr::utility::InitEigenMatrix;
using ltr::utility::DoubleEqual;
using ltr::DataSet;

TEST(EigenConvertersTest, TestingStdVectorToEigenVector) {
  vector<double> input(5);
  input[0] = 1.0;
  input[1] = -1.0;
  input[2] = 0.0;
  input[3] = numeric_limits<double>::max();
  input[4] = -numeric_limits<double>::max();

  VectorXd result = StdVectorToEigenVector(input);
  EXPECT_EQ(input.size(), result.size());
  EXPECT_TRUE(DoubleEqual(input[0], result[0]));
  EXPECT_TRUE(DoubleEqual(input[1], result[1]));
  EXPECT_TRUE(DoubleEqual(input[2], result[2]));
  EXPECT_TRUE(DoubleEqual(input[3], result[3]));
  EXPECT_TRUE(DoubleEqual(input[4], result[4]));

  input = vector<double>(0);
  result = StdVectorToEigenVector(input);
  EXPECT_EQ(0, result.size());
}

TEST(EigenConvertersTest, TestingEigenVectorToStdVector) {
  VectorXd input(5);
  input[0] = 1.0;
  input[1] = -1.0;
  input[2] = 0.0;
  input[3] = numeric_limits<double>::max();
  input[4] = -numeric_limits<double>::max();

  vector<double> result;
  EigenVectorToStdVector(input, &result);
  EXPECT_EQ(input.size(), result.size());
  EXPECT_TRUE(DoubleEqual(input[0], result[0]));
  EXPECT_TRUE(DoubleEqual(input[1], result[1]));
  EXPECT_TRUE(DoubleEqual(input[2], result[2]));
  EXPECT_TRUE(DoubleEqual(input[3], result[3]));
  EXPECT_TRUE(DoubleEqual(input[4], result[4]));

  input = VectorXd();
  EigenVectorToStdVector(input, &result);
  EXPECT_EQ(0, result.size());
}

TEST(EigenConvertersTest, TestingObjectDataSetToEigenMatrix) {
  DataSet<Object> input;
  Object object1, object2;
  object1 << 1.0 << 0.0 << -1.0 << 0.213;
  object2 << numeric_limits<double>::max()
          << -numeric_limits<double>::max()
          << numeric_limits<double>::min()
          << -numeric_limits<double>::min();
  input.add(object1);
  input.add(object2);

  MatrixXd result = DataSetToEigenMatrix(input);
  EXPECT_EQ(result.rows(), 2);
  EXPECT_EQ(result.cols(), 4);
  EXPECT_TRUE(DoubleEqual(result(0, 0), object1[0]));
  EXPECT_TRUE(DoubleEqual(result(0, 1), object1[1]));
  EXPECT_TRUE(DoubleEqual(result(0, 2), object1[2]));
  EXPECT_TRUE(DoubleEqual(result(0, 3), object1[3]));
  EXPECT_TRUE(DoubleEqual(result(1, 0), object2[0]));
  EXPECT_TRUE(DoubleEqual(result(1, 1), object2[1]));
  EXPECT_TRUE(DoubleEqual(result(1, 2), object2[2]));
  EXPECT_TRUE(DoubleEqual(result(1, 3), object2[3]));

  DataSet<Object> empty_input;
  result = DataSetToEigenMatrix(empty_input);
  EXPECT_EQ(result.rows(), 0);
  EXPECT_EQ(result.cols(), 0);
}

TEST(EigenConvertersTest, TestingObjectPairDataSetToEigenMatrix) {
  DataSet<ObjectPair> input;
  Object object1, object2;
  object1 << 1.0 << 0.0 << -1.0 << 0.213;
  object2 << numeric_limits<double>::max()
          << -numeric_limits<double>::max()
          << numeric_limits<double>::min()
          << -numeric_limits<double>::min();
  ObjectPair object_pair(object1, object2);
  input.add(object_pair);

  MatrixXd result = DataSetToEigenMatrix(input);
  EXPECT_EQ(result.rows(), 2);
  EXPECT_EQ(result.cols(), 4);
  EXPECT_TRUE(DoubleEqual(result(0, 0), object1[0]));
  EXPECT_TRUE(DoubleEqual(result(0, 1), object1[1]));
  EXPECT_TRUE(DoubleEqual(result(0, 2), object1[2]));
  EXPECT_TRUE(DoubleEqual(result(0, 3), object1[3]));
  EXPECT_TRUE(DoubleEqual(result(1, 0), object2[0]));
  EXPECT_TRUE(DoubleEqual(result(1, 1), object2[1]));
  EXPECT_TRUE(DoubleEqual(result(1, 2), object2[2]));
  EXPECT_TRUE(DoubleEqual(result(1, 3), object2[3]));

  DataSet<ObjectPair> empty_input;
  result = DataSetToEigenMatrix(empty_input);
  EXPECT_EQ(result.rows(), 0);
  EXPECT_EQ(result.cols(), 0);
}

TEST(EigenConvertersTest, TestingObjectListDataSetToEigenMatrix) {
  DataSet<ObjectList> input;
  Object object1, object2;
  object1 << 1.0 << 0.0 << -1.0 << 0.213;
  object2 << numeric_limits<double>::max()
          << -numeric_limits<double>::max()
          << numeric_limits<double>::min()
          << -numeric_limits<double>::min();
  ObjectList object_list, empty_object_list;
  object_list.add(object1);
  object_list.add(object2);
  input.add(object_list);
  input.add(empty_object_list);

  MatrixXd result = DataSetToEigenMatrix(input);
  EXPECT_EQ(result.rows(), 2);
  EXPECT_EQ(result.cols(), 4);
  EXPECT_TRUE(DoubleEqual(result(0, 0), object1[0]));
  EXPECT_TRUE(DoubleEqual(result(0, 1), object1[1]));
  EXPECT_TRUE(DoubleEqual(result(0, 2), object1[2]));
  EXPECT_TRUE(DoubleEqual(result(0, 3), object1[3]));
  EXPECT_TRUE(DoubleEqual(result(1, 0), object2[0]));
  EXPECT_TRUE(DoubleEqual(result(1, 1), object2[1]));
  EXPECT_TRUE(DoubleEqual(result(1, 2), object2[2]));
  EXPECT_TRUE(DoubleEqual(result(1, 3), object2[3]));

  DataSet<ObjectList> empty_input;
  result = DataSetToEigenMatrix(empty_input);
  EXPECT_EQ(result.rows(), 0);
  EXPECT_EQ(result.cols(), 0);
}

TEST(EigenConvertersTest, TestingInitEigenMatrix) {
  MatrixXd result(2, 1);
  InitEigenMatrix(&result);

  EXPECT_EQ(result.rows(), 2);
  EXPECT_EQ(result.cols(), 1);
  EXPECT_TRUE(DoubleEqual(result(0, 0), 0.0));
  EXPECT_TRUE(DoubleEqual(result(1, 0), 0.0));

  MatrixXd empty_result(0, 0);
  InitEigenMatrix(&empty_result);

  EXPECT_EQ(empty_result.rows(), 0);
  EXPECT_EQ(empty_result.cols(), 0);
}

TEST(EigenConvertersTest, TestingInitEigenVector) {
  VectorXd result(2);
  InitEigenVector(&result);

  EXPECT_EQ(result.size(), 2);
  EXPECT_TRUE(DoubleEqual(result[0], 0.0));
  EXPECT_TRUE(DoubleEqual(result[1], 0.0));

  VectorXd empty_result;
  InitEigenVector(&empty_result);

  EXPECT_EQ(empty_result.size(), 0);
}
