// Copyright 2012 Yandex

#ifndef LTR_MEASURES_AUC_H_
#define LTR_MEASURES_AUC_H_

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <utility>

#include "ltr/utility/numerical.h"
#include "ltr/measures/utils/measure_utility.h"
#include "ltr/measures/measure.h"
#include "ltr/data/object.h"
#include "ltr/data/data_set.h"

using std::fabs;
using std::string;
using std::pair;
using std::make_pair;
using std::sort;
using std::vector;

using ltr::Object;
using ltr::DataSet;
using ltr::utility::Inf;
using ltr::utility::PredictedAndActualLabels;
using ltr::utility::PredictedDecreasingActualIncreasing;
using ltr::utility::ExtractLabels;
using ltr::utility::DoubleEqual;

namespace ltr {
/**
 * AUC pointwise measure
 */
class AUC : public PointwiseMeasure {
 public:
  explicit AUC() {}

  explicit AUC(const ParametersContainer& parameters) {
    // DO NOTHING
  }

  double worst() const {
    return 0.0;
  }
  double best() const {
    return 1.0;
  }

  string toString() const;

  virtual double operator() (const DataSet<Object>& data_set) const;

 private:
  double get_measure(const Object& element) const {
    // not defined for one object
    return 0.;
  }

  virtual string getDefaultAlias() const {return "AUC";}
};
};
#endif  // LTR_MEASURES_AUC_H_
