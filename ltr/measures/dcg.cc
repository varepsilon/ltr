// Copyright 2011 Yandex

#include <cmath>

#include "ltr/measures/dcg.h"

using std::pow;
using std::log;


namespace ltr {
  double DCGFormula::operator()(double relevance, size_t position) const {
    return (pow(2, relevance) - 1) / log(position + 2.0);
  }

  double YandexDCGFormula::operator()(double relevance, size_t position) const {
    return relevance / (log(position + 1.0) + 1);
  }
};
