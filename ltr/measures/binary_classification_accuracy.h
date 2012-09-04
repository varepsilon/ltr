// Copyright 2012 Yandex

#ifndef LTR_MEASURES_BINARY_CLASSIFICATION_ACCURACY_H_
#define LTR_MEASURES_BINARY_CLASSIFICATION_ACCURACY_H_

#include <string>

#include "ltr/measures/measure.h"

using std::string;

namespace ltr {
/**
 * Measure for binary classification.
 * Actual labels of input data must be -1 and 1.
 * Returns 1, if predicted label have equal sign with actual label.
 * Returns 0 otherwise.
 */
class BinaryClassificationAccuracy : public PointwiseMeasure {
 public:
  explicit BinaryClassificationAccuracy() {}

  explicit BinaryClassificationAccuracy(
    const ParametersContainer& parameters) {
    // DO NOTHING
  }

  double best() const {
    return 1.;
  }

  double worst() const {
    return 0.;
  }

  string toString() const {
    return "Pointwise binary classification accuracy measure";
  }
 private:
  double get_measure(const Object& object) const {
    if (object.actual_label() * object.predicted_label() > 0) {
      return 1;
    } else {
      return 0;
    }
  }
  virtual string getDefaultAlias() const {
    return "BinaryClassificationAccuracy";
  }
};
};
#endif  // LTR_MEASURES_BINARY_CLASSIFICATION_ACCURACY_H_
