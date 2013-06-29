// Copyright 2012 Yandex

#ifndef LTR_SCORERS_GP_SCORER_H_
#define LTR_SCORERS_GP_SCORER_H_

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

#include "contrib/puppy/Puppy.hpp"

#include "ltr/data/data_set.h"

#include "ltr/learners/gp_learner/gp_primitives.h"

#include "ltr/interfaces/serializable.h"
#include "ltr/interfaces/parameterized.h"

#include "ltr/utility/boost/shared_ptr.h"

#include "ltr/scorers/scorer.h"

using std::vector;
using std::string;
using std::stringstream;

namespace ltr {
namespace gp {
typedef vector<Puppy::Tree> Population;
/**
 * \brief The class prepresent Scorer for GPLearner. It contains the last
 * population of genetic programming's evolution process.
 */
class GPScorer : public Scorer {
  ALLOW_SHARED_PTR_ONLY_CREATION(GPScorer)
 public:
  GPScorer() {}
  /**
   * \param best_tree is the best Puppy::tree(formula, individ) in the population.
   * \param context is the context upon which the Puppy::trees in the population
   * are constructed.
   * \param feature_count is the number of features for those the context
   * was created.
   * \param feature_converters is the vector of feature_converters to be applied
   * to the dataset before scoring.
   */
  GPScorer(const Puppy::Tree& best_tree,
           const Puppy::Context& context,
           int feature_count,
           const FeatureConverterArray& feature_converters =
           FeatureConverterArray())
  : Scorer(feature_converters),
  best_tree_(best_tree),
  context_(context),
  feature_count_(feature_count) {}

  /**
   * The function sets predicted labels in the dataset to the values calculated
   * using the Puppy::Tree(formula, individ).
   * \param data dataset to mark up.
   */
  template <typename TElement>
  void markDataSet(const DataSet<TElement>& data) const;

 private:
  /**
   * The implementation of scoring function. It scores using the best
   * Puppy::tree in the population.
   */
  double scoreImpl(const Object& object) const;
  /**
   * The function generates code for the scorer as cpp code function
   * \param function_name is the name of generating function.
   */
  string generateCppCodeImpl(const string& function_name) const;

  virtual string getDefaultAlias() const {return "GPScorer";}
  /**
   * The function sets up the values for primitives that represent feature
   * values to the concrete values from the given object.
   * \param object - object, whose feature values would be set up.
   */
  void setContextToObject(const Object& object) const;
  /**
   * The function writes down the given Puppy::Tree(formala, individ) as line
   * of cpp function calls. The function is used within serialization to Cpp
   * code.
   */
  void writeTreeAsStringOfCppCalls(std::ostream& output, int node_index) const; // NOLINT
  /**
   * The function casts the Puppy::PrimitiveHandle to the const Serializable*
   * using dynamic_cast. If the given object does't implement Serializable
   * interface a std::logic_error is thrown. The function is used within
   * serialization to Cpp code.
   */
  const Serializable* puppyPrimitiveHandleToPSerializable(
    const Puppy::PrimitiveHandle puppy_primitive) const;
  /**
   * The best Puppy::tree(formula, individ) in the population.
   */
  mutable Puppy::Tree best_tree_;
  /**
   * The context upon which the Puppy::trees in the population
   * are constructed.
   */
  mutable Puppy::Context context_;
  /**
   * The number of features for those the context was created.
   */
  int feature_count_;

  string toStringImpl() const {return "Genetic programming scorer";}

  /**
   * GPlearner can access the class private data to set up the scorer as the
   * starting point of GP evolution process.
   */
  template <typename TElement>
  friend class GPLearner;
};
};
};
#endif  // LTR_SCORERS_GP_SCORER_H_
