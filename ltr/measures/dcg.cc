// Copyright 2011 Yandex

#include <cmath>
#include <string>

#include "ltr/utility/numerical.h"
#include "ltr/measures/dcg.h"

using std::string;
using std::pow;
using std::log;

using ltr::utility::INFINITY;


namespace ltr {
  double DCGFormula::count(double relevance, size_t position) {
    return (pow(2, relevance) - 1) / (log(position + 2.0) / log(2.0));
  }
  string DCGFormula::alias() {
    return "DCGFormula";
  }
  double DCGFormula::best() {
    return INFINITY;
  }
  double DCGFormula::worst() {
    return 0.0;
  }

  double YandexDCGFormula::count(double relevance, size_t position) {
    return relevance / ((log(position + 1.0) / log(2.0)) + 1.0);
  }
  string YandexDCGFormula::alias() {
    return "YandexDCGFormula";
  }
  double YandexDCGFormula::best() {
    return INFINITY;
  }
  double YandexDCGFormula::worst() {
    return 0.0;
  }
};
