#include <iostream>
#include <vector>

#include "ltr\data\data_set.h"
#include "ltr\data\object.h"
#include "ltr\learners\linear_learner\linear_learner.h"
#include "ltr\scorers\linear_scorer.h"

using std::cout;
using std::vector;
using std::endl;

using ltr::DataSet;
using ltr::Object;

int main() {
  ltr::Log log;

  DataSet<Object> data;
  Object train_object1, train_object2, train_object3;

  train_object1 << 1.2 << 12.9;                                                 // create new object from training data
  train_object1.set_actual_label(1);                                            // set new actual label
  train_object2 << 2.4 << 25.8; 
  train_object2.set_actual_label(2);
  train_object3 << 0.3 << 3.225;
  train_object3.set_actual_label(0.25);
  data.add(train_object1);                                                      // add objects to training DataSet
  data.add(train_object2);
  data.add(train_object3);

  ltr::LinearLearner<Object> learner;                                           // create linear learner
  learner.learn(data);                                                          // learning process

  ltr::LinearScorer::Ptr scorer = learner.makeSpecific();                       // scorer based on the learner

  cout << "train_object1 score = " << scorer->score(train_object1) << endl;     // output scores for objects from training data
  cout << "train_object2 score = " << scorer->score(train_object2) << endl;
  cout << "train_object3 score = " << scorer->score(train_object3) << endl;

  Object test_object1;
  test_object1 << 12 << 129;                                                    // create object from testing data

  cout << "test_object1 score = " << scorer->score(test_object1) << endl;       // output score for object from testing data

  return 0;
}
