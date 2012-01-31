// Copyright 2011 Yandex

#ifndef LTR_SCORERS_GP_SCORER_H_
#define LTR_SCORERS_GP_SCORER_H_

#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>

#include "contrib/puppy/Puppy.hpp"

#include "ltr/learners/utility/gp_functions.h"

using std::vector;
using std::string;

namespace ltr {
namespace gp {

typedef vector<Puppy::Tree> Population;

class GPScorer : public Scorer {
  public:
  typedef boost::shared_ptr< GPScorer > Ptr;

  GPScorer(const Population& population,
      const Puppy::Context& context,
      size_t featureCountInContext,
      size_t inPopulationBestTreeIdx,
      const FeatureConverterArray& featureConverters =
      FeatureConverterArray())
  :Scorer("GPScorer", featureConverters),
  population_(population),
  context_(context),
  featureCountInContext_(featureCountInContext),
  inPopulationBestTreeIdx_(inPopulationBestTreeIdx) {}

  string brief() const {
    return "GPScorer";
  }

  private:
  double scoreImpl(const Object& obj) const {
    assert(featureCountInContext_ == obj.featureCount());
    setContextToObject(&context_, obj);
    double resultScore;
    population_[inPopulationBestTreeIdx_].interpret(&resultScore, context_);
    return resultScore;
  }

  string generateCppCode(const string& function_name) const {
    return "Not implemented.";
  }

  mutable Population population_;
  mutable Puppy::Context context_;
  size_t inPopulationBestTreeIdx_;
  size_t featureCountInContext_;

  template <typename TElement>
  friend class GPLearner;
};
}
}

#endif  // LTR_SCORERS_GP_SCORER_H_
