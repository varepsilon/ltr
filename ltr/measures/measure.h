// Copyright 2011 Yandex

#ifndef LTR_MEASURES_MEASURE_H_
#define LTR_MEASURES_MEASURE_H_

#pragma once

#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include <string>

#include "data/object.h"
#include "data/object_list.h"
#include "data/object_pair.h"
#include "data/data_set.h"
#include "interfaces.h"

using std::string;
using std::logic_error;

 /**
   * @section DESCRIPTION
   * This class allows to calculate most famous Measures on the ranking lists.
   * To learn more about these Measures visit http://romip.ru/romip2010/20_appendix_a_metrics.pdf (Russian) or
   * http://en.wikipedia.org/wiki/Learning_to_rank#Evaluation_measures (English)
   */

namespace ltr {

  template <class TElement>
  class Measure : public Aliaser {
  public:
    typedef boost::shared_ptr<Measure> Ptr;

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

  protected:
    virtual double get_measure(const TElement& element) const = 0;
  };

  typedef Measure<Object> PointwiseMeasure;
  typedef Measure<ObjectPair> PairwiseMeasure;
  typedef Measure<ObjectList> ListwiseMeasure;
};

namespace ltr {  // template realizations

  template< class TElement>
  double Measure<TElement>::operator()(const TElement& element) const{
      if (element.size() == 0){
        throw logic_error(alias() + " gained empty list");
      }
      double measure;
      try {
        measure = get_measure(element);
      }
      catch(logic_error err) {
        throw err;
      }
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

    // #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < static_cast<int>(set.size()); ++i) {
      query_proceed += 1;
      double val;
      try {
        val = this->operator()(*set[i]);
      }
      catch(logic_error err) {
        LTR_LOG.warning() << "In Measure::average : "
          << err.what() << ".\n";
        val = 0.0;
        // #pragma omp critical
        {
          query_proceed -= 1;
        }
      }
      // #pragma omp critical
      {
        sum += val;
      }
    }

    sum /= static_cast<double>(query_proceed);
    return sum;
  }

  // waiting for element weights in dataset
/*  template< class TElement>
  double Measure<TElement>::weightedAverage(const DataSet<TElement>& set) const {
    if(set.size() == 0) {
      throw logic_error(alias() + " gained empty set");
    }

    double sum = 0.0;
    double weights = 0.0;

    // #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < int(set.size()); ++i) {
      double w = set.weight(i);
      double val; 
      try {
        val = w * this->operator()(*set[i]);
      }
      catch (logic_error err) {
        LTR_LOG.warning() << "In Measure::weightedAverage : " 
          << err.what() << ".\n";
        val = 0.0;
        w = 0.0;
      }
      // #pragma omp critical (WAverage)
      {
        weights += w;
        sum += val;
      }
    }
    return sum / weights;
  }*/
};

#endif  // LTR_MEASURES_MEASURE_H_
