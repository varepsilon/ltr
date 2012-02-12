// Copyright 2011 Yandex

#ifndef LTR_MEASURES_DCG_H_
#define LTR_MEASURES_DCG_H_

#include <stdexcept>
#include <algorithm>
#include <string>

#include "ltr/data/object_list.h"
#include "ltr/measures/measure.h"
#include "ltr/measures/utils/measure_utility.h"

using std::sort;
using std::logic_error;
using std::string;
using ltr::utility::PredictedAndActualLabels;
using ltr::utility::ExtractLabels;
using ltr::utility::PredictedDecreasingActualIncreasing;
using ltr::ObjectList;

namespace ltr {
  class DCGFormula : public Aliaser {
  public:
    DCGFormula() : Aliaser("common DCG formula") {}
    DCGFormula(const string& alias) : Aliaser(alias) {}
    /** The result of this function is added to the DCG for each position.
     * \param relevance the relevance the object in position
     * \param position the position number 0..size() - 1
     */
    virtual double operator()(double relevance, size_t position) const;
  };

  class YandexDCGFormula : public DCGFormula {
  public:
    YandexDCGFormula() : DCGFormula("yandex DCG formula") {}
    double operator()(double relevance, size_t position) const;
  };


  /**
   * \class DCG
   * Implements function-object to calculate DCG on ObjectLists.
   */
  template<class TDCGFormula>
  class BaseDCG : public MoreIsBetterMeasure<ObjectList> {
    public:
    /**
     * Constructor
     * \param parameters Standart LTR parameter container with int parameter
     * NUMBER_OF_OBJECTS_TO_CONSIDER (by default 0 is used, that means all objects
     * in an ObjectList are considered).
     */
    BaseDCG(const ParametersContainer& parameters = ParametersContainer())
    :MoreIsBetterMeasure<ObjectList>("DCG with " + TDCGFormula().alias()) {
      this->setDefaultParameters();
      this->parameters().copyParameters(parameters);
    }

    /** The function clears parameters container and sets default value 0 for
     *  NUMBER_OF_OBJECTS_TOCONSIDER;
     */
    void setDefaultParameters();
    void checkParameters() const;

    private:
    /** The function calculates DCG measure.
     * \param objects ObjectList to calculate measure on
     */
    double get_measure(const ObjectList& objects) const;
  };

  typedef BaseDCG<DCGFormula> DCG;

  // template realizations
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

    if (result < 0.0) {
      throw logic_error(alias() + " calculated < 0");
    }
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
