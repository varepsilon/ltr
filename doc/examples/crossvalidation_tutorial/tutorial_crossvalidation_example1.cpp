#include <iostream>
#include <vector>

#include "ltr/crossvalidation/crossvalidation.h"
#include "ltr/crossvalidation/leave_one_out_splitter.h"
#include "ltr/crossvalidation/validation_result.h"
#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/measures/abs_error.h"
#include "ltr/measures/measure.h"
#include "ltr/scorers/fake_scorer.h"

using std::cout;
using std::endl;
using std::vector;

using ltr::AbsError;
using ltr::BestFeatureLearner;
using ltr::cv::LeaveOneOutSplitter;
using ltr::cv::Validate;
using ltr::cv::ValidationResult;
using ltr::FeatureInfo;
using ltr::FakeScorer;
using ltr::Object;
using ltr::PointwiseMeasure;

const int data_length = 5;
const FeatureInfo fi(3);

int main() {
  ltr::Log log;

  DataSet<Object> data(fi);
  for (int i = 0; i < data_length; ++i) {
    Object object;
    object << 0.7 * i + 0.3 << 0.2 * i * i * i - 0.8 << i * i * 2.7 - 3.4 * i;
    data.add(object);
  }
  FakeScorer::Ptr fscorer(new FakeScorer());
  fscorer->predict(data);

  AbsError::Ptr ab_measure(new AbsError);
  BestFeatureLearner<Object>::Ptr bfl(
    new BestFeatureLearner<Object>(ab_measure));

  vector<PointwiseMeasure::Ptr> abm_vector;
  abm_vector.push_back(ab_measure);

  LeaveOneOutSplitter<Object>::Ptr spl(new LeaveOneOutSplitter<Object>);

  ValidationResult vr = Validate(data, abm_vector, bfl, spl);

  cout << vr.getSplitCount() << endl;
  cout << vr.getMeasureValues(0).size() << endl;
  cout << vr.getMeasureNames().size() << endl;
  for (int measure_index = 0;
       measure_index < vr.getMeasureNames().size();
       ++measure_index) {
     cout << vr.getMeasureNames()[measure_index] << endl;
  }

  for (int split = 0; split < vr.getSplitCount(); ++split) {
    Object test_object;
    test_object << 1;
    cout << vr.getScorer(split)->score(test_object) << " "
         << vr.getMeasureValues(split).at(0) << endl;
  }
};