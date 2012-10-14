#include <iostream>
#include <vector>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/learners/linear_learner/linear_learner.h"
#include "ltr/measures/abs_error.h"
#include "ltr/scorers/linear_scorer.h"

using std::cout;
using std::vector;
using std::endl;

int main() {
  ltr::Log log("log.txt");

  ltr::Object object1, object2, object3;
  object1 << 1 << 2;
  object1.set_actual_label(10 + (1 + 2));
  object2 << 20 << 30;
  object2.set_actual_label(10 + (20 + 30));
  object3 << 300 << 400;
  object3.set_actual_label(10 + (300 + 400));

  ltr::DataSet<Object> data;
  data.add(object1);
  data.add(object2);
  data.add(object3);
  cout << "Train data:\n" << data << endl;

  ltr::LinearLearner<Object> learner;    // learn linear regression
  learner.learn(data);

  ltr::Scorer::Ptr scorer = learner.make();           // make scorer
  cout << scorer->toString() << endl;                 // human-readable representation of scorer
  cout << endl;
  cout << scorer->generateCppCode("score") << endl;   // serialize scorer to cpp code

  cout << "score of object1 = " << scorer->value(object1)
       << endl;
  scorer->predict(data);                                  // compute predicted labels
  cout << "\nPredicted train data:\n" << data;

  ltr::AbsError abs_error;                                // test quality
  cout << "\nAbsError(data) = " << abs_error(data)
       << endl;

  // use makeSpecific if you want to analyze result scorer in details
  ltr::LinearScorer::Ptr linear_scorer = learner.makeSpecific();
  cout << "f(x, y) = " << linear_scorer->weight(0)
       << " + "  << linear_scorer->weight(1) << " * x"
       << " + "  << linear_scorer->weight(2) << " * y"
       << endl;

  return 0;
}
