// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_UTILITY_CONDITIONS_LEARNER_H_
#define LTR_LEARNERS_UTILITY_CONDITIONS_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <vector>

#include "ltr/data/data_set.h"
#include "ltr/interfaces/parameterized.h"

#include "ltr/decision_tree/condition.h"

using std::vector;
using ltr::Object;
using ltr::DataSet;

using ltr::decision_tree::Condition;

namespace ltr {
namespace decision_tree {

class ConditionsLearner {
  protected:
    DataSet<Object> data_;
    virtual void init() = 0;

  public:
    typedef boost::shared_ptr<ConditionsLearner> Ptr;
    typedef boost::shared_ptr<ConditionsLearner> BasePtr;

    void setDataSet(DataSet<Object> data) {
      data_ = data;
      init();
    }
    virtual int getNextConditions(vector<Condition::Ptr>* result) = 0;
    virtual void setLastSplittingQuality(double quality) {}
    virtual ~ConditionsLearner() {}
};
}
}

#endif  // LTR_LEARNERS_UTILITY_CONDITIONS_LEARNER_H_
