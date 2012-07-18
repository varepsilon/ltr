// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_SPLITTING_QUALITY_H_
#define LTR_LEARNERS_DECISION_TREE_SPLITTING_QUALITY_H_

#include "ltr/utility/shared_ptr.h"

#include <vector>

#include "ltr/data/data_set.h"
#include "ltr/interfaces/parameterized.h"

using ltr::DataSet;

namespace ltr {
namespace decision_tree {

class SplittingQuality : public Parameterized {
  public:
    typedef ltr::utility::shared_ptr<SplittingQuality> Ptr;
    typedef ltr::utility::shared_ptr<SplittingQuality> BasePtr;

    virtual double value(DataSet<ltr::Object> data,
                         vector<DataSet<ltr::Object> > split) const = 0;

    virtual void setDefaultParameters() const {}
    virtual void checkParameters() const {}
};

class FakeSplittingQuality : public SplittingQuality {
  public:
    typedef ltr::utility::shared_ptr<FakeSplittingQuality> Ptr;

    double value(DataSet<ltr::Object> data,
                         vector<DataSet<ltr::Object> > split) const {
      return 0;
    }
};
}
}

#endif  // LTR_LEARNERS_DECISION_TREE_SPLITTING_QUALITY_H_
