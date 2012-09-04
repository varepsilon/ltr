// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_SQR_ERROR_QUALITY_H_
#define LTR_LEARNERS_DECISION_TREE_SQR_ERROR_QUALITY_H_

#include <vector>

#include "ltr/learners/decision_tree/splitting_quality.h"

namespace ltr {
namespace decision_tree {
class SqrErrorQuality : public SplittingQuality {
 public:
  typedef ltr::utility::shared_ptr<SqrErrorQuality> Ptr;

  explicit SqrErrorQuality(
    const ParametersContainer& parameters = ParametersContainer()) {
      this->setDefaultParameters();
  }

  virtual double value(
    DataSet<ltr::Object> data, vector<DataSet<ltr::Object> > split) const {
      INFO("Getting value of data");
      if (split.size() <= 1) {
        INFO("Split size is less or equal then one");
        return -1e9;
      }

      double result = 0;
      for (int i = 0; i < (int)split.size(); ++i) {
        result += split[i].size() * 1.0 / data.size() * sqrError(split[i]);
      }
      return result;
  }
 private:
  double sqrError(DataSet<ltr::Object> data) const {
    INFO("Getting sqrError of data");

    double weight_sum = 0;
    double sum_label = 0;
    for (int i = 0; i < (int)data.size(); ++i) {
      PerObjectAccessor<ltr::Object> per_object_accessor(&data[i]);
      sum_label +=
        per_object_accessor.object(0).actual_label() * data.getWeight(i);
      weight_sum += data.getWeight(i);
    }

    double average = sum_label / weight_sum;
    double error = 0;
    for (int i = 0; i < (int)data.size(); ++i) {
      PerObjectAccessor<ltr::Object> per_object_accessor(&data[i]);
      double label = per_object_accessor.object(0).actual_label();
      error += (label - average) * (label - average) * data.getWeight(i);
    }
    return -error / weight_sum;
  }
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_SQR_ERROR_QUALITY_H_
