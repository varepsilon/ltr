// Copyright 2011 Yandex

#ifndef LTR_MEASURES_DCG_H_
#define LTR_MEASURES_DCG_H_

#include <stdexcept>
#include <algorithm>
#include <string>
#include <vector>

#include "ltr/data/object_list.h"
#include "ltr/measures/measure.h"
#include "ltr/measures/utils/measure_utility.h"

using std::sort;
using std::logic_error;
using std::string;
using std::vector;
using ltr::utility::PredictedAndActualLabels;
using ltr::utility::ExtractLabels;
using ltr::utility::PredictedDecreasingActualIncreasing;
using ltr::ObjectList;

namespace ltr {
  /**
   * DCGFormula is formula, which result is added to the DCG measure for each position.
   * Also it contains information if resulted DCG measure is MoreIsBetter of LessIsBetter
   * and what bounds has DCG result (e.g. it must be non-negative)
   */
  class DCGFormula : public Aliaser {
  public:
    DCGFormula() : Aliaser("common DCG formula") {}
    explicit DCGFormula(const string& alias) : Aliaser(alias) {}
    /**
     * The result of this function is added to the DCG for each position.
     * @param relevance - the relevance of the object in position
     * @param position - the position number 0..size() - 1
     */
    virtual double operator()(double relevance, size_t position) const;
    /**
     * Returns if expected better value of DCGmeasure is really better than expected worse one.
     * By default bigger DCG measure is better
     */
    virtual bool better(double expected_better, double expected_worse) const;
    /**
     * Checks DCG measure result. By default checks if result >= 0 (it should be true)
     */
    virtual void checkDCGResult(double result) const;
  };

  /**
   * Yandex DCG formula from http://imat2009.yandex.ru/datasets
   */
  class YandexDCGFormula : public DCGFormula {
  public:
    YandexDCGFormula() : DCGFormula("yandex DCG formula") {}
    double operator()(double relevance, size_t position) const;
  };


  /**
   * DCG listwise measure, which is parametrized by formula used in it
   */
  template<class TDCGFormula>
  class BaseDCG : public ListwiseMeasure {
    public:
    /**
     * @param parameters Standart LTR parameter container with int parameter
     * NUMBER_OF_OBJECTS_TO_CONSIDER (where 0 means consider all docs),
     * by default NUMBER_OF_OBJECTS_TO_CONSIDER = 0
     */
    BaseDCG(const ParametersContainer& parameters = ParametersContainer())
    :ListwiseMeasure("DCG with " + TDCGFormula().alias()) {
      this->setDefaultParameters();
      this->parameters().copyParameters(parameters);
      this->checkParameters();
    }

    /** 
     * Clears parameters container and sets default values:
     * NUMBER_OF_OBJECTS_TOCONSIDER = 0
     */
    void setDefaultParameters();
    /**
     * Checks if NUMBER_OF_OBJECTS_TOCONSIDER > 0 (should be true)
     */
    void checkParameters() const;

    bool better(double expected_better, double expected_worse) const;

    private:
    double get_measure(const ObjectList& objects) const;
  };

  typedef BaseDCG<DCGFormula> DCG;
  typedef BaseDCG<YandexDCGFormula> YandexDCG;

  // template realizations
  template<class TDCGFormula>
  bool BaseDCG<TDCGFormula>::better(double expected_better, double expected_worse) const {
    TDCGFormula formula;
    return formula.better(expected_better, expected_worse);
  }

  template<class TDCGFormula>
  double BaseDCG<TDCGFormula>::get_measure(const ObjectList& objects) const {
    vector<PredictedAndActualLabels> labels = ExtractLabels(objects);
    sort(labels.begin(), labels.end(), PredictedDecreasingActualIncreasing);

    size_t n = this->parameters().getInt("NUMBER_OF_OBJECTS_TO_CONSIDER");
    if ((n == 0) || (n > labels.size())) {
      n = labels.size();
    }

    double result = 0.0;
    TDCGFormula formula;
    for (int labels_index = 0; labels_index < n; ++labels_index) {
      result += formula(labels[labels_index].actual, labels_index);
    }

    formula.checkDCGResult(result);
    return result;
  }

  template<class TDCGFormula>
  void BaseDCG<TDCGFormula>::setDefaultParameters() {
    this->parameters().clear();
    this->parameters().setInt("NUMBER_OF_OBJECTS_TO_CONSIDER", 0);
  }
  template<class TDCGFormula>
  void BaseDCG<TDCGFormula>::checkParameters() const {
    if (this->parameters().getInt("NUMBER_OF_OBJECTS_TO_CONSIDER") < 0) {
      throw logic_error(alias() + " NUMBER_OF_OBJECTS_TO_CONSIDER < 0");
    }
  }
};
#endif  // LTR_MEASURES_DCG_H_
