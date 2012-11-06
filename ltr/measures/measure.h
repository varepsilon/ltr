// Copyright 2012 Yandex

#ifndef LTR_MEASURES_MEASURE_H_
#define LTR_MEASURES_MEASURE_H_

#include <rlog/rlog.h>

#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <stdexcept>
#include <string>

#include "ltr/data/object.h"
#include "ltr/data/object_list.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/data_set.h"

#include "ltr/interfaces/aliaser.h"
#include "ltr/interfaces/printable.h"
#include "ltr/interfaces/parameterized.h"

#include "ltr/parameters_container/parameters_container.h"

#include "ltr/utility/numerical.h"
#include "ltr/utility/shared_ptr.h"

using std::string;
using std::logic_error;
using std::min;
using std::max;

using ltr::utility::DoubleLess;
using ltr::utility::DoubleMore;
using ltr::utility::isNaN;

namespace ltr {
/**
 * @section DESCRIPTION
 * This class allows to calculate most famous Measures on the ranking lists, pairs or single objects.
 * To learn more about these Measures visit http://romip.ru/romip2010/20_appendix_a_metrics.pdf (Russian) or
 * http://en.wikipedia.org/wiki/Learning_to_rank#Evaluation_measures (English)
 */
template <class TElement>
class Measure : public Aliaser, public Parameterized, public Printable {
  public:
  typedef ltr::utility::shared_ptr<Measure> Ptr;
  typedef ltr::utility::shared_ptr<Measure> BasePtr;

  /**
   * Value of measure(quality of algorithm)
   * @param element - element(single object, pair or list) for counting measure on
   */
  virtual double operator()(const TElement& element) const;
  virtual double operator()(const DataSet<TElement>& data_set) const;
  /**
   * Value of measure(quality of algorithm)
   * @param element - element(single object, pair or list) for counting measure on
   */
  double value(const TElement& element) const {
    return operator()(element);
  }

  /**
   * Gets average measure value of all elements (using for counting average weights from dataset)
   * @param set - input dataset of objects, to count average measure on
   */
  double average(const DataSet<TElement>& set) const;
  /**
   * Returns if expected better value of measure is really better than expected worse one.
   * There are two types of measures: when bigger measure is better (e.g. accuracy)
   * and when smaller measure is better (e.g. abs error)
   */
  bool better(double expected_better, double expected_worse) const;

  /**
   * Returns the worst measure's possible value or -ltr::Inf if unlimited
   */
  virtual double worst() const = 0;
  /**
   * Returns the best measure's possible value or ltr::Inf if unlimited
   */
  virtual double best() const = 0;

  private:
  /**
   * Throws if measure's result is not between best() and worst()
   */
  void checkResult(double result) const;
  /**
   * Implementation. Returns value of measure(quality of algorithm)
   * @param element - element(single object, pair or list) for counting measure on
   */
  virtual double get_measure(const TElement& element) const = 0;
};

typedef Measure<Object> PointwiseMeasure;
typedef Measure<ObjectPair> PairwiseMeasure;
typedef Measure<ObjectList> ListwiseMeasure;

// template realizations
template<class TElement>
bool Measure<TElement>::better(double expected_better,
    double expected_worse) const {
  if (worst() < best()) {
    return expected_worse < expected_better;
  } else {
    return expected_worse > expected_better;
  }
}

template<class TElement>
void Measure<TElement>::checkResult(double result) const {
  if (isNaN(result)) {
    throw logic_error(alias() + " NaN calculated");
  } else if (DoubleMore(result, max(best(), worst()))) {
    throw logic_error(alias() + " calculated > " +
      boost::lexical_cast<string>(max(best(), worst())));
  } else if (DoubleLess(result, min(best(), worst()))) {
    throw logic_error(alias() + " calculated < " +
      boost::lexical_cast<string>(min(best(), worst())));
  }
}

template<class TElement>
double Measure<TElement>::operator()(const TElement& element) const {
  PerObjectAccessor<const TElement> per_object_accessor(&element);
  if (per_object_accessor.object_count() == 0) {
    throw logic_error(alias() + " gained empty list");
  }
  double measure = get_measure(element);
  checkResult(measure);
  return measure;
};

template<class TElement>
double Measure<TElement>::operator()(const DataSet<TElement>& data_set) const {
  return this->average(data_set);
}

template<class TElement>
double Measure<TElement>::average(
    const DataSet<TElement>& set) const {
  if (set.size() == 0) {
    throw logic_error(alias() + " gained empty set");
  }

  double sum = 0.0;
  double weights = 0.0;

  for (int i = 0; i < set.size(); ++i) {
    double w = set.getWeight(i);
    double val;
    val = w * this->operator()(set[i]);
    weights += w;
    sum += val;
  }
  return sum / weights;
}
};

#endif  // LTR_MEASURES_MEASURE_H_
