// Copyright 2012 Yandex

#include <gtest/gtest.h>
#include <string>
#include <sstream>

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

using std::endl;
using std::stringstream;

TEST(CrossValidatorTest, SimpleTest) {
  CrossValidator<ObjectList> cv_instance;

  DataSet<ObjectList> train_dataset;

  ObjectList obj_list_instance;

  Object obj_instance;

  for (int train_index = 0; train_index < 100; ++train_index) {
    for (int count = 0; count < 5; ++count) {
      obj_instance << train_index;
    }
    obj_instance.set_actual_label(train_index);
    obj_list_instance.add(obj_instance);

    obj_instance.clear();
  }

  train_dataset.add(obj_list_instance);

  obj_list_instance.clear();

  for (int train_index = 0; train_index < 100; ++train_index) {
    for (int count = 0; count < 5; ++count) {
      obj_instance << -train_index;
    }
    obj_instance.set_actual_label(train_index);
    obj_list_instance.add(obj_instance);

    obj_instance.clear();
  }

  train_dataset.add(obj_list_instance);

  cv_instance.add_data_set(train_dataset);

  KFoldSimpleSplitter<ObjectList>::Ptr splitter_ptr =
      KFoldSimpleSplitter<ObjectList>::Ptr(
        new KFoldSimpleSplitter<ObjectList>(2));

  cv_instance.add_splitter(splitter_ptr);

  Measure<ObjectList>::Ptr measure_ptr(new DCG());

  cv_instance.add_measure(measure_ptr);

  BestFeatureLearner<ObjectList>::Ptr learner_ptr =
      BestFeatureLearner<ObjectList>::Ptr(
        new BestFeatureLearner<ObjectList>(measure_ptr));

  cv_instance.add_learner(learner_ptr);

  cv_instance.launch();
  string testString = cv_instance.toString();
  string::iterator newEnd = remove_if(testString.begin(),
                                      testString.end(), isspace);
  testString = string(testString.begin(), newEnd);

  stringstream result;
  result << "Measure:DCGDataSet:DataSetTableROW:LearnerCOLUMN:" <<
            "SplitterKFoldSimpleSplitterBestFeatureLeaner" <<
            5.83307e+29;
  if (result.str() != testString) {
    cout << result.str() << "\n\n" << testString << "\n\n";
  }

  EXPECT_EQ(result.str(), testString);
}

