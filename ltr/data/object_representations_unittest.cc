// Copyright 2013 Yandex

#include "gtest/gtest.h"

#include "ltr/data/object_representation.h"
#include "ltr/data/data_set_object_representation.h"
#include "ltr/data/stand_alone_object_representation.h"

#include "ltr/data/data_set.h"

#include "contrib/include_Eigen.h"

using ltr::StandAloneObjectRepresentation;
using ltr::DataSetObjectRepresentation;
using ltr::MetaInfo;

using ltr::InnerRepresentation;

TEST(SelectorsTest, StandAloneObjectRepresentationTest) {
  StandAloneObjectRepresentation selector(0);

  // Testing meta info
  (*selector.meta_info())["label"] = "info";

  MetaInfo info;
  info["label"] = "info2";

  EXPECT_EQ((*selector.meta_info())["label"], "info");

  selector.set_meta_info(info);

  EXPECT_EQ((*selector.meta_info())["label"], "info2");

  // Testing pushing and getting features
  selector.push_back(30.0);
  selector.push_back(10.0);
  selector.push_back(-1.1);

  EXPECT_EQ(selector.size(), 3);

  EXPECT_EQ(selector.at(2), -1.1);
  EXPECT_ANY_THROW(selector.at(3));
  EXPECT_ANY_THROW(selector.at(-1));

  selector.at(0) = 5.0;
  EXPECT_EQ(selector.at(0), 5.0);

  // Testing size and resize
  EXPECT_EQ(selector.size(), 3);
  EXPECT_ANY_THROW(selector.resize(-1));

  selector.resize(4);
  EXPECT_EQ(selector.size(), 4);
  selector.resize(2);
  EXPECT_EQ(selector.at(1), 10.0);

  // Testing actual and predicted labels
  selector.set_actual_label(2.0);
  selector.set_predicted_label(3.0);

  EXPECT_EQ(selector.actual_label(), 2.0);
  EXPECT_EQ(selector.predicted_label(), 3.0);

  // Testing light copy
  EXPECT_TRUE(selector.allowLightCopy());

  // Testing deep copy and clear
  StandAloneObjectRepresentation selector2;
  selector2.deepCopyFrom(selector);
  EXPECT_EQ(selector2.at(1), selector.at(1));
  selector.at(0) += 2.0;
  EXPECT_FALSE(selector2.at(0) == selector.at(0));

  selector.clear();
  EXPECT_EQ(selector.size(), 0);

  // Testing eigen vector
  Eigen::VectorXd eigen_vector = selector2.getEigenColumn();
  EXPECT_EQ(eigen_vector[1], selector2.at(1));

  eigen_vector.setConstant(1);
  EXPECT_FALSE(eigen_vector.isApprox(selector2.getEigenColumn()));

  selector2.setEigenColumn(eigen_vector);
  EXPECT_TRUE(eigen_vector.isApprox(selector2.getEigenColumn()));
}

TEST(SelectorsTest, DataSetObjectRepresentationTest) {
  InnerRepresentation::Ptr presentation = new InnerRepresentation(4, 3);
  presentation->feature(2, 2) = -1.1;

  DataSetObjectRepresentation selector(presentation, 2);

  // Testing meta info
  (*selector.meta_info())["label"] = "info";

  MetaInfo info;
  info["label"] = "info2";

  EXPECT_EQ((*selector.meta_info())["label"], "info");

  selector.set_meta_info(info);

  EXPECT_EQ((*selector.meta_info())["label"], "info2");

  // Testing pushing and getting features
  EXPECT_ANY_THROW(selector.push_back(30.0));

  EXPECT_EQ(selector.at(2), -1.1);
  EXPECT_ANY_THROW(selector.at(3));
  EXPECT_ANY_THROW(selector.at(-1));

  selector.at(0) = 5.0;
  EXPECT_EQ(selector.at(0), 5.0);

  // Testing size and resize
  EXPECT_EQ(selector.size(), 3);
  EXPECT_ANY_THROW(selector.resize(-1));

  EXPECT_ANY_THROW(selector.resize(4));
  EXPECT_NO_THROW(selector.resize(3));

  // Testing actual and predicted labels
  selector.set_actual_label(2.0);
  selector.set_predicted_label(3.0);

  EXPECT_EQ(selector.actual_label(), 2.0);
  EXPECT_EQ(selector.predicted_label(), 3.0);

  // Testing light copy
  EXPECT_FALSE(selector.allowLightCopy());

  // Testing deep copy and clear
  DataSetObjectRepresentation selector2(presentation, 1);
  selector2.deepCopyFrom(selector);
  EXPECT_EQ(selector2.at(0), selector.at(0));
  selector.at(0) += 2.0;
  EXPECT_FALSE(selector2.at(0) == selector.at(0));

  EXPECT_ANY_THROW(selector.clear());
  EXPECT_EQ(selector.size(), 3);

  // Testing eigen vector
  Eigen::VectorXd eigen_vector = selector2.getEigenColumn();
  EXPECT_EQ(eigen_vector[1], selector2.at(1));

  eigen_vector.setConstant(1);
  EXPECT_FALSE(eigen_vector.isApprox(selector2.getEigenColumn()));

  selector2.setEigenColumn(eigen_vector);
  EXPECT_TRUE(eigen_vector.isApprox(selector2.getEigenColumn()));
}
