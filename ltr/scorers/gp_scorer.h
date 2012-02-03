// Copyright 2011 Yandex

#ifndef LTR_SCORERS_GP_SCORER_H_
#define LTR_SCORERS_GP_SCORER_H_

#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>
#include <sstream>

#include "contrib/puppy/Puppy.hpp"

#include "ltr/learners/utility/gp_functions.h"
#include "ltr/learners/utility/gp_primitives.h"
#include "ltr/utility/serializable.h"

using std::vector;
using std::string;
using std::stringstream;

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

  using Scorer::generateCppCode;
  string generateCppCode(const string& function_name) const {
    string code;
    // generate primitive-functions code.
    vector<Puppy::PrimitiveHandle>::const_iterator functionItr =
        context_.mFunctionSet.begin();
    for (; functionItr != context_.mFunctionSet.end(); ++functionItr) {
      const Serializable* pSerializable = puppyPrimitiveHandleToPSerializable(
          *functionItr);
      string function_name = pSerializable->getDefaultSerializableObjectName();
      function_name += (*functionItr)->getName();
      code.append(pSerializable->generateCppCode(function_name));
    }
    // generate the function from tree.
    stringstream sstreamForCalls;
    writeTreeAsStringOfCppCalls(population_[inPopulationBestTreeIdx_],
        &sstreamForCalls, 0);
    // generate scoring function
    code.append("double ");
    code.append(function_name);
    code.append("(double * feature) {\n");
    code.append("  return ");
    code.append(sstreamForCalls.str());
    code.append(";\n");
    code.append("}\n");
    return code;
  }

  private:
  double scoreImpl(const Object& obj) const {
    assert(featureCountInContext_ == obj.featureCount());
    setContextToObject(&context_, obj);
    double resultScore;
    population_[inPopulationBestTreeIdx_].interpret(&resultScore, context_);
    return resultScore;
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
