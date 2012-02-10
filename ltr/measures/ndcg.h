// Copyright 2011 Yandex

#ifndef LTR_MEASURES_NDCG_H_
#define LTR_MEASURES_NDCG_H_

#include <stdexcept>

#include "ltr/data/object_list.h"
#include "ltr/measures/measure.h"
#include "ltr/measures/dcg.h"

using std::logic_error;
using ltr::ObjectList;

namespace ltr {
  /**
   * \class NDCG
   * Implements function-object to calculate NDCG on ObjectLists.
   */
  class NDCG : public MoreIsBetterMeasure<ObjectList> {
    public:
    /**
     * Constructor
     * \param parameters Standart LTR parameter container with int parameter
     * NUMBER_OF_OBJECTS_TOCONSIDER (by default 0 is used, that means all objects
     * in an ObjectList are considered).
     */
    NDCG(const ParametersContainer& parameters = ParametersContainer())
    :MoreIsBetterMeasure<ObjectList>("NDCG") {
      this->setDefaultParameters();
      this->parameters().copyParameters(parameters);
    }

    /** The function clears parameters container and sets default value 0 for
     *  NUMBER_OF_OBJECTS_TOCONSIDER;
     */
    void setDefaultParameters() {
      this->parameters().clear();
      this->parameters().setInt("NUMBER_OF_OBJECTS_TO_CONSIDER", 0);
    }
    void checkParameters() const {
      if (this->parameters().getInt("NUMBER_OF_OBJECTS_TO_CONSIDER") < 0) {
        throw logic_error(alias() + " NUMBER_OF_OBJECTS_TO_CONSIDER < 0");
      }
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
    static double NDCGFormula(double relevance, size_t position);
  };
}
#endif  // LTR_MEASURES_NDCG_H_
