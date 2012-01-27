// Copyright 2011 Yandex

#ifndef LTR_MEASURES_DCG_H_
#define LTR_MEASURES_DCG_H_

#include <utility>
#include <vector>

#include "ltr/data/object_list.h"
#include "ltr/measures/measure.h"
#include "ltr/parameters_container/parameters_container.h"

using std::pair;

namespace ltr {

/** \typedef
 * Type for predicted and actual labels of an object.
 */
typedef pair<double, double> PredictedAndActualLabels;

/** \fn This function is used for temp array sorting in DCG calculation.
 * \param firstObjectsLabels a pair of two doubles: predicted and actual labels
 * of an object.
 * \param secondObjectsLabels a pair of two doubles: predicted and actual labels
 * of an object.
 */
bool sortPredicatedDecreasingActualIncreasingFunc(
    const PredictedAndActualLabels& firstObjectsLabels,
    const PredictedAndActualLabels& secondObjectsLabels);

/**
 * \class DCG
 * Implements function-object to calculate DCG on ObjectLists.
 */
class DCG : public Measure<ObjectList> {
  public:
  /**
   * Constructor
   * \param parameters Standart LTR parameter container with int parameter
   * NUMBER_OF_OBJECTS_TO_CONSIDER (by default 0 is used, that means all objects
   * in an ObjectList are considered).
   */
  DCG(const ParametersContainer& parameters = ParametersContainer())
  :Measure<ObjectList>("DCG") {
    this->setDefaultParameters();
    this->parameters().copyParameters(parameters);
  }

  /**
   * Constructor
   * \param numberOfObjectsToConsider The number of top objects used for DCG
   * calculation. 0 means, that all objects are used.
   */
  explicit DCG(int numberOfObjectsToConsider)
  :Measure<ObjectList>("DCG") {
    assert(numberOfObjectsToConsider >= 0);
    this->parameters().clear();
    this->parameters().setInt("NUMBER_OF_OBJECTS_TO_CONSIDER",
        numberOfObjectsToConsider);
  }

  /** The function clears parameters container and sets default value 0 for
   *  NUMBER_OF_OBJECTS_TOCONSIDER;
   */
  void setDefaultParameters() {
    this->parameters().clear();
    this->parameters().setInt("NUMBER_OF_OBJECTS_TO_CONSIDER", 0);
  }

  /** The function returns true if expected_better is actually better
   * expected_worse.
   */
  bool better(double expected_better, double expected_worse) const {
    return expected_better > expected_worse;
  }

  private:
  /** The function calculates DCG measure.
   * \param objects ObjectList to calculate measure on
   */
  double get_measure(const ObjectList& objects) const;

  /** The result of this function is added to the DCG for each position.
   * \param relevance the relevance the object in position
   * \param position the position number 0..size() - 1
   */
  static double iterartion_dcg_formula(double relevance, size_t position);

  /** Buffer, it is used for sorting objects in ObjectList to find top
   * considered_top_objects_number_ to calculate DCG on those.
   */
  mutable vector<PredictedAndActualLabels > predictedAndActualLabels;
};
}
#endif  // LTR_MEASURES_DCG_H_
