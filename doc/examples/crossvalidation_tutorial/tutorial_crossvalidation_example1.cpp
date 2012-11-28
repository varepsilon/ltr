#include <iostream>
#include <vector>

#include "ltr/crossvalidation/leave_one_out_splitter.h"
#include "ltr/crossvalidation/crossvalidator.h"
#include "ltr/data/data_set.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/feature_converters/nan_to_zero_learner.h"
#include "ltr/measures/dcg.h"
#include "ltr/measures/ndcg.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/learners/gp_learner/gp_learner.h"

using std::cout;
using std::vector;
using std::endl;

using namespace ltr;
using namespace ltr::cv;
using namespace ltr::gp;
using namespace ltr::io_utility;

int main() {
  DataSet<ObjectList> test_data = loadDataSet<ObjectList>(
          "data/imat2009/imat2009_test_small.txt", "Yandex");
  DataSet<ObjectList> train_data = loadDataSet<ObjectList>(
          "data/imat2009/imat2009_learning_small.txt", "Yandex");

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
  cross_validator.add_data_set(train_data);
  cross_validator.add_learner(learner_one);
  cross_validator.add_learner(learner_two);
  cross_validator.add_measure(measure_one);
  cross_validator.add_measure(measure_two);
  cross_validator.add_splitter(splitter);
  cross_validator.launch();
  cout << cross_validator.toString();

  return 0;
}
