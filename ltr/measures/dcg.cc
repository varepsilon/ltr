// Copyright 2011 Yandex

#include <cmath>

#include "ltr/measures/dcg.h"

using std::pow;
using std::log;


namespace ltr {
  double DCGFormula::operator()(double relevance, size_t position) const {
    return (pow(2, relevance) - 1) / (log(position + 2.0) / log(2.0));
  }

  void DCGFormula::checkDCGResult(double result) const {
    if (result < 0.0) {
      throw logic_error(alias() + " calculated < 0");
    }
  }

  bool DCGFormula::better(double expected_better, double expected_worse) const {
    return expected_better > expected_worse;
  }

  double YandexDCGFormula::operator()(double relevance, size_t position) const {
    return relevance / ((log(position + 1.0) / log(2.0)) + 1.0);
  }
};
