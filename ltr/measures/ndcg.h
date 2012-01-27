// Copyright 2011 Yandex

#ifndef LTR_MEASURES_NDCG_H_
#define LTR_MEASURES_NDCG_H_

#include <vector>

#include "ltr/data/object_list.h"
#include "ltr/measures/measure.h"
#include "ltr/measures/dcg.h"

namespace ltr {

/** \fn This function is used for temp array sorting in NDCG calculation.
 * \param firstObjectsLabels a pair of two doubles: predicted and actual labels
 * of an object.
 * \param secondObjectsLabels a pair of two doubles: predicted and actual labels
 * of an object.
 */
bool sortActualDecreasingFunc(
    const PredictedAndActualLabels& firstObjectsLabels,
    const PredictedAndActualLabels& secondObjectsLabels);

/**
 * \class NDCG
 * Implements function-object to calculate NDCG on ObjectLists.
 */
class NDCG : public Measure<ObjectList> {
  public:
  /**
   * Constructor
   * \param parameters Standart LTR parameter container with int parameter
   * NUMBER_OF_OBJECTS_TOCONSIDER (by default 0 is used, that means all objects
   * in an ObjectList are considered).
   */
  NDCG(const ParametersContainer& parameters = ParametersContainer())
  :Measure<ObjectList>("NDCG") {
    this->setDefaultParameters();
    this->parameters().copyParameters(parameters);
  }

  /**
   * Constructor
   * \param numberOfObjectsToConsider The number of top objects used for NDCG
   * calculation. 0 means, that all objects are used.
   */
  explicit NDCG(int numberOfObjectsToConsider)
  :Measure<ObjectList>("NDCG") {
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
  /** The function calculates NDCG measure.
   * \param objects ObjectList to calculate measure on
   */
  double get_measure(const ObjectList& objects) const;

  /** The result of this function is added to the NDCG for each position.
   * \param relevance the relevance the object in position
   * \param position the position number 0..size() - 1
   */
  static double iterartion_dcg_formula(double relevance, size_t position);

  /** Buffer, it is used for sorting objects in ObjectList to find top
   * considered_top_objects_number_ to calculate NDCG on those.
   */
  mutable vector<PredictedAndActualLabels > predictedAndActualLabels;
};
}
#endif  // LTR_MEASURES_NDCG_H_
