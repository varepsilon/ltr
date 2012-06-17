// Copyright 2011 Yandex

#ifndef LTR_MEASURES_ACCURACY_H_
#define LTR_MEASURES_ACCURACY_H_

#include <string>

#include "ltr/measures/measure.h"

using std::string;

namespace ltr {
/**
 * Accuracy measure. Can be poinwise and pairwise, but not listwise.
 * Returns 1 if actual score is equal (almost) to predicted one
 * (or for pairs if actual and predicted pair orders are equal)
 * and 0 otherwise
 */
template <class TElement>
class Accuracy : public Measure<TElement> {
 public:
  Accuracy(const ParametersContainer& parameters = ParametersContainer()) {
      this->setDefaultParameters();
      this->copyParameters(parameters);
  }
  double best() const {
    return 1.0;
  }
  double worst() const {
    return 0.0;
  }
  string toString() const;
 private:
  double get_measure(const TElement& object) const;
  virtual string getDefaultAlias() const {return "Accuracy";}
};
}

#endif  // LTR_MEASURES_ACCURACY_H_
