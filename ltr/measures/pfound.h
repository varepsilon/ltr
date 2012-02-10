// Copyright 2011 Yandex

#ifndef LTR_MEASURES_PFOUND_H_
#define LTR_MEASURES_PFOUND_H_

#include <stdexcept>

#include "ltr/measures/measure.h"
#include "ltr/data/object_list.h"

using std::logic_error;
using ltr::ObjectList;

namespace ltr {
  class PFound: public MoreIsBetterMeasure<ObjectList> {
  public:
    PFound(const ParametersContainer& parameters = ParametersContainer())
      :MoreIsBetterMeasure<ObjectList>("PFound") {
        this->setDefaultParameters();
        this->parameters().copyParameters(parameters);
    }
    /**
    * P_BREAK Probability of unexpected user's break
    * MAX_LABEL The maximal possible relevance of (ideal) document
    * NUMBER_OF_OBJECTS_TO_CONSIDER Number of top documents to consider
    * if 0 than consider all docs
    */
    void setDefaultParameters() {
      this->parameters().setDouble("P_BREAK", 0.15);
      this->parameters().setDouble("MAX_LABEL", 5.0);
      this->parameters().setInt("NUMBER_OF_OBJECTS_TO_CONSIDER", 5);
    }
    void checkParameters() const {
      if (this->parameters().getDouble("P_BREAK") > 1.0) {
        throw logic_error(alias() + " P_BREAK > 1");
      }
      if (this->parameters().getDouble("P_BREAK") < 0.0) {
        throw logic_error(alias() + " P_BREAK < 0");
      }
      if (this->parameters().getInt("NUMBER_OF_OBJECTS_TO_CONSIDER") < 0) {
        throw logic_error(alias() + " NUMBER_OF_OBJECTS_TO_CONSIDER < 0");
      }
      if (this->parameters().getDouble("MAX_LABEL") < 0.0) {
        throw logic_error(alias() + " MAX_LABEL < 0");
      }
    }

  private:
    double get_measure(const ObjectList& objects) const;
  };
};

#endif  // LTR_MEASURES_PFOUND_H_
