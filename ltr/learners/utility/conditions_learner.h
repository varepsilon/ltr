// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_UTILITY_CONDITIONS_LEARNER_H_
#define LTR_LEARNERS_UTILITY_CONDITIONS_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <vector>

#include "ltr/data/data_set.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/decision_tree/condition.h"

using std::vector;

namespace ltr {
namespace decision_tree {

class ConditionsLearner : public Parameterized {
  protected:
    DataSet<Object> data_;

  private:
    virtual void init() = 0;

  public:
    typedef boost::shared_ptr<ConditionsLearner> Ptr;
    typedef boost::shared_ptr<ConditionsLearner> BasePtr;

    /**
     Function used to gen the next conditions set.
     Returns 0 if can't generate or 1 if it could.
     @param result - pointer to the vector to save result in.
     */
    virtual int getNextConditions(vector<Condition::Ptr>* result) = 0;
    /**
     Function used to set data set to generate conditions.
     */
    void setDataSet(DataSet<Object> data) {
      data_ = data;
      init();
    }
    /** Function used as a callback to tell ConditionsLearner
    the quality of the last conditions set.
     */
    virtual void setLastSplittingQuality(double quality) {}
};

class FakeConditionsLearner : public ConditionsLearner {
  private:
    virtual void init() {}

  public:
    virtual int getNextConditions(vector<Condition::Ptr>* result) {
      return 0;
    }
};
}
}

#endif  // LTR_LEARNERS_UTILITY_CONDITIONS_LEARNER_H_
