// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_UTILITY_SPLITTING_QUALITY_H_
#define LTR_LEARNERS_UTILITY_SPLITTING_QUALITY_H_

#include <boost/shared_ptr.hpp>

#include <vector>

#include "ltr/data/data_set.h"

using ltr::DataSet;

namespace ltr {
namespace decision_tree {

class FakeSplittingQuality {
  public:
    typedef boost::shared_ptr<FakeSplittingQuality> Ptr;

    double value(DataSet<ltr::Object> data,
                         vector<DataSet<ltr::Object> > split) const {
      return 0;
    }
};

class SqrErrorQuality {
  private:
    double sqrError(DataSet<ltr::Object> data) const {
      double weight_sum = 0;
      double sum_label = 0;
      for (int i = 0; i < data.size(); i++) {
        sum_label += data[i][0].actualLabel() * data.getWeight(i);
        weight_sum += data.getWeight(i);
      }
      double average = sum_label / weight_sum;
      double err = 0;
      for (int i = 0; i < data.size(); i++) {
        double label = data[i][0].actualLabel();
        err += (label - average) * (label - average) * data.getWeight(i);
      }
      return -err / weight_sum;
    }

  public:
    typedef boost::shared_ptr<SqrErrorQuality> Ptr;

    double value(DataSet<ltr::Object> data,
                         vector<DataSet<ltr::Object> > split) const {
      if (split.size() <= 1)
        return -1e9;
      double res = 0;
      for (int i = 0; i < split.size(); i++)
        res += split[i].size() * 1.0 / data.size() * sqrError(split[i]);
      return res;
    }
};
}
}

#endif  // LTR_LEARNERS_UTILITY_SPLITTING_QUALITY_H_
