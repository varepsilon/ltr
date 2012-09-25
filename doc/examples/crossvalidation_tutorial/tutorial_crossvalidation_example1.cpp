#include <iostream>
#include <vector>

#include "boost/filesystem/path.hpp"

#include "fstream"

#include "ltr/crossvalidation/crossvalidation.h"
#include "ltr/crossvalidation/leave_one_out_splitter.h"
#include "ltr/crossvalidation/validation_result.h"
#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/measures/abs_error.h"
#include "ltr/measures/measure.h"
#include "ltr/scorers/fake_scorer.h"

#include <iostream>
#include <algorithm>
#include "ltr/data/object_list.h"
#include "ltr/data/data_set.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/measures/dcg.h"
#include "ltr/measures/ndcg.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/learners/linear_learner/linear_learner.h"
#include "ltr/learners/nearest_neighbor_learner/nearest_neighbor_learner.h"
#include "ltr/learners/gp_learner/gp_learner.h"
#include "ltr/feature_converters/nan_to_zero_learner.h"
#include "ltr/crossvalidation/leave_one_out_splitter.h"
#include "ltr/crossvalidation/crossvalidator.h"

using std::cout;
using std::vector;
using std::endl;
using ltr::ObjectList;
using ltr::DataSet;
using ltr::io_utility::loadDataSet;
using ltr::Measure;
using ltr::DCG;
using ltr::NDCG;
using ltr::BestFeatureLearner;
using ltr::FeatureConverter;
using ltr::NanToZeroConverterLearner;
using ltr::cv::LeaveOneOutSplitter;
using ltr::cv::CrossValidator;
using ltr::Log;
using ltr::LinearLearner;
using ltr::NNLearner;
using ltr::gp::GPLearner;

using std::fstream;

int main() {
  Log log("log.txt");
  DataSet<ObjectList> test_data = loadDataSet<ObjectList>(
          "data/imat2009/imat2009_test_small.txt", "Yandex");
  DataSet<ObjectList> train_data = loadDataSet<ObjectList>(
          "data/imat2009/imat2009_learning_small.txt", "Yandex");

  NanToZeroConverterLearner<ObjectList> converter;
  converter.learn(train_data);
  FeatureConverter::Ptr remove_nan = converter.make();

  remove_nan->apply(train_data, &train_data);
  remove_nan->apply(test_data, &test_data);

  Measure<ObjectList>::Ptr measure_one(new DCG());
  Measure<ObjectList>::Ptr measure_two(new NDCG());
  BestFeatureLearner<ObjectList>::Ptr learner_one =
      new BestFeatureLearner<ObjectList>(measure_one);
  learner_one->learn(train_data);

  GPLearner<ObjectList>::Ptr learner_two =
      new GPLearner<ObjectList>(measure_one);
  learner_two->learn(train_data);

  LeaveOneOutSplitter<ObjectList>::Ptr splitter=
      new LeaveOneOutSplitter<ObjectList>();

  CrossValidator<ObjectList> cross_validator;
  cross_validator.addDataSet(train_data);
  cross_validator.addLearner(learner_one);
  cross_validator.addLearner(learner_two);
  cross_validator.addMeasure(measure_one);
  cross_validator.addMeasure(measure_two);
  cross_validator.addSplitter(splitter);
  cross_validator.launch();
  cout << cross_validator.toString();

  return 0;
}
