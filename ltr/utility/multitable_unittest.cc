//Copyright 2012 Yandex

#include <gtest/gtest.h>

#include "ltr/crossvalidation/crossvalidator.h"
#include "ltr/crossvalidation/k_fold_simple_splitter.h"
#include "ltr/data/data_set.h"
#include "ltr/data/object_list.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/measures/dcg.h"

using ltr::cv::CrossValidator;
using ltr::cv::KFoldSimpleSplitter;
using ltr::io_utility::loadDataSet;
using ltr::BestFeatureLearner;
using ltr::DataSet;
using ltr::DCG;
using ltr::ObjectList;


class CrossValidatorTest : public ::testing::Test {
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

TEST_F(CrossValidatorTest, SimpleTest) {
  CrossValidator<ObjectList> cvInstance;

  DataSet<ObjectList> trainDataset;

  ObjectList objListInstance;

  Object objInstance;

  objInstance << 0.0 << 0.0 << 0.0 << 0.0;
  objInstance.set_actual_label(1.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  objInstance << 1.0 << 1.0 << 1.0 << 1.0;
  objInstance.set_actual_label(2.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  objInstance << 2.0 << 2.0 << 2.0 << 2.0;
  objInstance.set_actual_label(3.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  objInstance << 3.0 << 3.0 << 3.0 << 3.0;
  objInstance.set_actual_label(4.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  objInstance << 4.0 << 4.0 << 4.0 << 4.0;
  objInstance.set_actual_label(5.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  objInstance << 5.0 << 5.0 << 5.0 << 5.0;
  objInstance.set_actual_label(6.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  objInstance << 6.0 << 6.0 << 6.0 << 6.0;
  objInstance.set_actual_label(4.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  trainDataset.add(objListInstance);

  objListInstance.clear();

  objInstance << 0.0 << 0.0 << 0.0 << 0.0;
  objInstance.set_actual_label(1.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  objInstance << -1.0 << -1.0 << -1.0 << -1.0;
  objInstance.set_actual_label(2.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  objInstance << -2.0 << -2.0 << -2.0 << -2.0;
  objInstance.set_actual_label(3.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  objInstance << -3.0 << -3.0 << -3.0 << -3.0;
  objInstance.set_actual_label(4.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  objInstance << -4.0 << -4.0 << -4.0 << -4.0;
  objInstance.set_actual_label(5.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  objInstance << -5.0 << -5.0 << -5.0 << -5.0;
  objInstance.set_actual_label(6.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  objInstance << -6.0 << -6.0 << -6.0 << -6.0;
  objInstance.set_actual_label(4.0);
  objListInstance.add(objInstance);

  objInstance.clear();

  trainDataset.add(objListInstance);

  cvInstance.addDataSet(trainDataset);

  KFoldSimpleSplitter<ObjectList>::Ptr splitter_ptr =
      KFoldSimpleSplitter<ObjectList>::Ptr(
        new KFoldSimpleSplitter<ObjectList>(2));

  cvInstance.addSplitter(splitter_ptr);

  Measure<ObjectList>::Ptr measure_ptr(new DCG());

  cvInstance.addMeasure(measure_ptr);

  BestFeatureLearner<ObjectList>::Ptr learner_ptr =
      BestFeatureLearner<ObjectList>::Ptr(
        new BestFeatureLearner<ObjectList>(measure_ptr));

  cvInstance.addLearner(learner_ptr);

  cvInstance.launch();
  string testString = cvInstance.toString();

  string checkString = "Measure:DCGDataSet:datasetTableROW:LearnerCOLUMN:SplitterKFoldSimpleSplitterBestFeatureLeaner66.5525";
  string::iterator newEnd = remove_if( testString.begin(), testString.end(), isspace);
  testString = string(testString.begin(), newEnd);

  EXPECT_EQ(checkString, testString);
}

