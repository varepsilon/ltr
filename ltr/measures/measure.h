// Copyright 2011 Yandex

#ifndef LTR_MEASURES_MEASURE_H_
#define LTR_MEASURES_MEASURE_H_

#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include <string>

#include "ltr/data/object.h"
#include "ltr/data/object_list.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/data_set.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/parameters_container/parameters_container.h"

using std::string;
using std::logic_error;

namespace ltr {
/**
 * @section DESCRIPTION
 * This class allows to calculate most famous Measures on the ranking lists, pairs or single objects.
 * To learn more about these Measures visit http://romip.ru/romip2010/20_appendix_a_metrics.pdf (Russian) or
 * http://en.wikipedia.org/wiki/Learning_to_rank#Evaluation_measures (English)
 */
template <class TElement>
class Measure : public Aliaser, public Parameterized {
  public:
  typedef boost::shared_ptr<Measure> Ptr;

  explicit Measure(const string& alias) : Aliaser(alias) {}

  /**
   * Value of Measure(quality of algorithm)
   * @param element for counting measure
   */
  double operator()(const TElement& element) const;

  /**
   * This method allows to get average value for all elements
   * @param set - learning set of objects
   */
  double average(const DataSet<TElement>& set) const;

  /**
   * Returns sums of measures for all elements proportional to their weights.
   */
  double weightedAverage(const DataSet<TElement>& set) const;

  virtual bool better(double expected_better, double expected_worse) const = 0;

  private:
  virtual double get_measure(const TElement& element) const = 0;
};

template <class TElement>
class LessIsBetterMeasure : public Measure<TElement> {
public:
  explicit LessIsBetterMeasure(const string& alias) : Measure<TElement>(alias) {}

  bool better(double expected_better, double expected_worse) const {
    return expected_better < expected_worse;
  }
};

template <class TElement>
class MoreIsBetterMeasure : public Measure<TElement> {
public:
  explicit MoreIsBetterMeasure(const string& alias) : Measure<TElement>(alias) {}

  bool better(double expected_better, double expected_worse) const {
    return expected_better > expected_worse;
  }
};

typedef Measure<Object> PointwiseMeasure;
typedef Measure<ObjectPair> PairwiseMeasure;
typedef Measure<ObjectList> ListwiseMeasure;

template< class TElement>
double Measure<TElement>::operator()(const TElement& element) const {
  if (element.size() == 0) {
    throw logic_error(alias() + " gained empty list");
  }
  double measure;
  measure = get_measure(element);
  if (measure < 0) {
    throw logic_error(alias() + " negative calculated");
  };
  return measure;
};

template< class TElement>
double Measure<TElement>::average(const DataSet<TElement>& set) const {
  if (set.size() == 0) {
    throw logic_error(alias() + " measure gained empty set");
  }

  double sum = 0.0;
  int query_proceed = 0;

  for (size_t i = 0; i < set.size(); ++i) {
    query_proceed += 1;
    double val;
    val = this->operator()(set[i]);
    sum += val;
  }

  sum /= query_proceed;
  return sum;
}

template< class TElement>
double Measure<TElement>::weightedAverage(
    const DataSet<TElement>& set) const {
  if (set.size() == 0) {
    throw logic_error(alias() + " gained empty set");
  }

  double sum = 0.0;
  double weights = 0.0;

  for (size_t i = 0; i < set.size(); ++i) {
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
