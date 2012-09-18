// Copyright 2012 Yandex

#ifndef LTR_MEASURES_BINARY_CLASSIFICATION_MARGIN_H_
#define LTR_MEASURES_BINARY_CLASSIFICATION_MARGIN_H_

#include <string>

#include "ltr/measures/measure.h"
#include "ltr/utility/numerical.h"

using std::string;
using ltr::utility::Inf;

namespace ltr {
/**
 * Measure for binary classification.
 * Actual labels of input data must be -1 and 1.
 * Returns predicted label times actual label
 */
class BinaryClassificationMargin : public PointwiseMeasure {
 public:
  explicit BinaryClassificationMargin() {}

  explicit BinaryClassificationMargin(
    const ParametersContainer& parameters) {
    // DO NOTHING
  }

  double best() const {
    return Inf;
  }

  double worst() const {
    return -Inf;
  }

  string toString() const {
    return "Pointwise binary classification margin measure";
  }
 private:
  double get_measure(const Object& object) const {
    return object.actual_label() * object.predicted_label();
  }
  virtual string getDefaultAlias() const {
    return "BinaryClassificationMargin";
  }
};
};
#endif  // LTR_MEASURES_BINARY_CLASSIFICATION_MARGIN_H_
