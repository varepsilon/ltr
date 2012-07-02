// Copyright 2011 Yandex

#ifndef LTR_SCORERS_GP_SCORER_H_
#define LTR_SCORERS_GP_SCORER_H_

#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>
#include <sstream>

#include "contrib/puppy/Puppy.hpp"

#include "ltr/learners/gp_learner/gp_functions.h"
#include "ltr/learners/gp_learner/gp_primitives.h"
#include "ltr/interfaces/serializable.h"

using std::vector;
using std::string;
using std::stringstream;

namespace ltr {
namespace gp {

typedef vector<Puppy::Tree> Population;
/** \class The class prepresent Scorer for GPLearner. It contains the last
 * population of genetic programming's evolution process.
 */
class GPScorer : public Scorer {
  public:
  /** \typedef boost shared pointer to GPScorer;
   */
  typedef boost::shared_ptr< GPScorer > Ptr;
  /** Constructor.
   * \param population the current population of  genetic programming's
   * evolution process, vector of Puppy::tree.
   * \param context the context upon which the Puppy::trees in the population
   * are constructed.
   * \param featureCountInContext the number of features for those the context
   * was created.
   * \param inPopulationBestTreeIdx the index of best Puppy::tree(formula,
   * individ) in the population.
   * \param featureConverters the vector of featureConverters to be applied
   * to the dataset before scoring.
   */
  GPScorer(const Population& population,
      const Puppy::Context& context,
      size_t featureCountInContext,
      size_t inPopulationBestTreeIdx,
      const FeatureConverterArray& featureConverters =
          FeatureConverterArray())
  : Scorer("GPScorer", featureConverters),
  population_(population),
  context_(context),
  featureCountInContext_(featureCountInContext),
  inPopulationBestTreeIdx_(inPopulationBestTreeIdx) {}

  string toString() const {
    return "Genetic programming scorer";
  }

  private:
  /** The implementation of scoring function. It scores using the best
   *  Puppy::tree in the population.
   */
  double scoreImpl(const Object& obj) const {
    assert(featureCountInContext_ == obj.feature_count());
    setContextToObject(&context_, obj);
    double resultScore;
    population_[inPopulationBestTreeIdx_].interpret(&resultScore, context_);
    return resultScore;
  }
  /** the function generates code for the scorer as cpp code function
   * \param class_name the name for the class that would be created.
   */
  string generateCppCodeImpl(const string& function_name) const {
    string code;
    code.append("#include <vector>\n");
    // generate primitive-functions code.
    vector<Puppy::PrimitiveHandle>::const_iterator functionItr =
        context_.mFunctionSet.begin();
    for (; functionItr != context_.mFunctionSet.end(); ++functionItr) {
      const Serializable* pSerializable = puppyPrimitiveHandleToPSerializable(
          *functionItr);
      string primiriveFunctionName =
          pSerializable->getDefaultSerializableObjectName();
      primiriveFunctionName += (*functionItr)->getName();
      code.append(pSerializable->generateCppCode(primiriveFunctionName));
    }
    // generate the function from tree.
    stringstream sstreamForCalls;
    writeTreeAsStringOfCppCalls(population_[inPopulationBestTreeIdx_],
        &sstreamForCalls, 0);
    // generate scoring function
    code.append("double ");
    code.append(function_name);
    code.append("(const std::vector< double >& feature) {\n");
    code.append("  return ");
    code.append(sstreamForCalls.str());
    code.append(";\n");
    code.append("}\n");

    return code;
  }

  /** the current population of  genetic programming's
   * evolution process, vector of Puppy::tree.
   */
  mutable Population population_;
  /** the context upon which the Puppy::trees in the population
   * are constructed.
   */
  mutable Puppy::Context context_;
  /** the number of features for those the context
    * was created.
    */
  size_t inPopulationBestTreeIdx_;
  /** the index of best Puppy::tree(formula,
   * individ) in the population.
   */
  size_t featureCountInContext_;
  /** GPlearner can access the class private data to set up the scorer as the
   * starting point of GP evolution process.
   */
  template <typename TElement>
  friend class GPLearner;
};
}
}
#endif  // LTR_SCORERS_GP_SCORER_H_
