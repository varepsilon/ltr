// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_SQR_ERROR_QUALITY_H_
#define LTR_LEARNERS_DECISION_TREE_SQR_ERROR_QUALITY_H_

#include <vector>

#include "ltr/learners/decision_tree/splitting_quality.h"

namespace ltr {
namespace decision_tree {

class SqrErrorQuality : public SplittingQuality {
  private:
    double sqrError(DataSet<ltr::Object> data) const {
      double weight_sum = 0;
      double sum_label = 0;
      for (int i = 0; i < data.size(); i++) {
        sum_label += data[i][0].actual_label() * data.getWeight(i);
        weight_sum += data.getWeight(i);
      }
      double average = sum_label / weight_sum;
      double err = 0;
      for (int i = 0; i < data.size(); i++) {
        double label = data[i][0].actual_label();
        err += (label - average) * (label - average) * data.getWeight(i);
      }
      return -err / weight_sum;
    }

  public:
    typedef boost::shared_ptr<SqrErrorQuality> Ptr;

    explicit SqrErrorQuality(
        const ParametersContainer& parameters = ParametersContainer()) {
      this->setDefaultParameters();
    }

    virtual double value(DataSet<ltr::Object> data,
                         vector<DataSet<ltr::Object> > split) const {
      if (split.size() <= 1) {
        return -1e9;
      }
      double result = 0;
      for (int i = 0; i < split.size(); i++) {
        result += split[i].size() * 1.0 / data.size() * sqrError(split[i]);
      }
      return result;
    }
};
}
}

#endif  // LTR_LEARNERS_DECISION_TREE_SQR_ERROR_QUALITY_H_
