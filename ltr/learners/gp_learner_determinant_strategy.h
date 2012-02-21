// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_DETERMINANT_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_DETERMINANT_STRATEGY_H_

#include "ltr/learners/gp_learner.h"

#include <algorithm>

namespace ltr {
namespace gp {

template <typename TElement>
class GPLearnerWithDeterminantStrategy : public GPLearner<TElement> {
  public:
  /** Constructor creates a GPLearnerWithDeterminantStrategy.
   * \param p_Measure shared pointer to the measure that would be maximized on
   * a dataset within learning.
   * \param parameters the ParametersContainer parameters from which would
   * overwrite the default parameters.
   */
  GPLearnerWithDeterminantStrategy(typename Measure<TElement>::Ptr p_Measure,
      const ParametersContainer& parameters = ParametersContainer())
  : GPLearner<TElement>(p_Measure, parameters) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
  }

  /** Constructor creates a GPLearnerWithDeterminantStrategy. But leaves
   * p_measure uninitialized.
   * \param parameters the ParametersContainer parameters from which would
   * overwrite the default parameters.
   */
  GPLearnerWithDeterminantStrategy(
      const ParametersContainer& parameters = ParametersContainer())
  : GPLearner<TElement>(parameters) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
  }

  /** The function sets up default parameters for genetic learning process.
   */
  virtual void setDefaultParameters() {
    this->clearParameters();
    this->addIntParameter("POP_SIZE", 10);
    this->addIntParameter("NBR_GEN", 3);
    this->addIntParameter("MAX_DEPTH", 35);
    this->addIntParameter("MIN_INIT_DEPTH", 2);
    this->addIntParameter("MAX_INIT_DEPTH", 5);
    this->addDoubleParameter("INIT_GROW_PROBA", 0.5);
    this->addDoubleParameter("TOP_FOR_NEXT_GENERATION_PART", 0.3);
    this->addDoubleParameter("CROSSOVER_VS_MUTATION_PART", 0.7);
    this->addDoubleParameter("CROSSOVER_DISTRIB_PROBA", 0.3);
    this->addDoubleParameter("STANDART_MUTATION_VS_SWAP_MUTATION_PROBA",
        0.5);
    this->addIntParameter("MUT_MAX_REGEN_DEPTH", 5);
    this->addDoubleParameter("MUT_SWAP_DISTRIB_PROBA", 0.5);
    this->addIntParameter("SEED", 1);
    this->addBoolParameter("USE_ADD", true);
    this->addBoolParameter("USE_SUB", true);
    this->addBoolParameter("USE_MUL", true);
    this->addBoolParameter("USE_DIV", true);
    this->addBoolParameter("USE_IF", true);
    this->addBoolParameter("USE_EFEM", true);
  }
  /** The method checks the correctness of the parameters in the parameters
   * container. If one of them is not correct it throws
   * std::logical_error(PARAMETER_NAME).
   */
  virtual void checkParameters() const {
    CHECK_INT_PARAMETER("POP_SIZE", X > 0);
    CHECK_INT_PARAMETER("NBR_GEN", X > 0);
    CHECK_INT_PARAMETER("MAX_DEPTH", X > 0);
    CHECK_INT_PARAMETER("MUT_MAX_REGEN_DEPTH", X > 0);
    CHECK_INT_PARAMETER("MIN_INIT_DEPTH", X > 0);

    CHECK_INT_PARAMETER("MAX_INIT_DEPTH", \
      X > this->getIntParameter("MIN_INIT_DEPTH") - 1);

    CHECK_DOUBLE_PARAMETER("INIT_GROW_PROBA", \
      X >= 0 && X <= 1);
    CHECK_DOUBLE_PARAMETER("TOP_FOR_NEXT_GENERATION_PART", \
      X >= 0 && X <= 1);
    CHECK_DOUBLE_PARAMETER("CROSSOVER_VS_MUTATION_PART", \
      X >= 0 && X <= 1);
    CHECK_DOUBLE_PARAMETER("CROSSOVER_DISTRIB_PROBA", \
      X >= 0 && X <= 1);
    CHECK_DOUBLE_PARAMETER("STANDART_MUTATION_VS_SWAP_MUTATION_PROBA", \
      X >= 0 && X <= 1);
    CHECK_DOUBLE_PARAMETER("MUT_SWAP_DISTRIB_PROBA", \
      X >= 0 && X <= 1);
  }

  private:
  /** \brief This function implements the changes made in the population at each
   *  algorithm's iteration.
   */
  virtual void evaluationStrategyStepImpl() {
    assert(this->population_.size() > 0);

    std::cout << "Population looks like:\n";
    for (size_t treeIdx = 0; treeIdx < this->population_.size(); ++treeIdx) {
      std::cout << this->population_.at(treeIdx).mFitness
          << "(" << this->population_.at(treeIdx).mValid <<") ";
    }
    std::cout << std::endl;

    std::cout << "Sorting population.\n";
    std::sort(this->population_.begin(), this->population_.end());

    size_t topBoundIdx =
        (1 - this->getDoubleParameter("TOP_FOR_NEXT_GENERATION_PART")) *
        this->population_.size();
    if (topBoundIdx == this->population_.size()) {
      topBoundIdx -= 1;
    }

    size_t curTreeFromTopIdx = topBoundIdx;
    for (size_t treeIdx = 0; treeIdx < topBoundIdx; ++treeIdx) {
      this->population_.at(treeIdx) = this->population_.at(curTreeFromTopIdx);
      ++curTreeFromTopIdx;
      if (curTreeFromTopIdx >= this->population_.size()) {
        curTreeFromTopIdx = topBoundIdx;
      }
    }

    size_t crossoverBoundIdx =
        this->getDoubleParameter("CROSSOVER_VS_MUTATION_PART") *
        topBoundIdx;
    if (crossoverBoundIdx % 2 != 1) {
      crossoverBoundIdx -= 1;
    }

    for (size_t treeIdx = 0; treeIdx < crossoverBoundIdx; treeIdx += 2) {
      Puppy::mateTrees(this->population_[treeIdx],
          this->population_[treeIdx + 1],
          this->context_,
          this->getDoubleParameter("CROSSOVER_DISTRIB_PROBA"),
          this->getIntParameter("MAX_DEPTH"));
    }

    for (size_t treeIdx = crossoverBoundIdx; treeIdx < topBoundIdx; ++treeIdx) {
      if (this->context_.mRandom.rollUniform() <=
          this->getDoubleParameter(
              "STANDART_MUTATION_VS_SWAP_MUTATION_PROBA")) {
        Puppy::mutateStandard(this->population_[treeIdx],
            this->context_,
            this->getIntParameter("MUT_MAX_REGEN_DEPTH"),
            this->getIntParameter("MAX_DEPTH"));
      } else {
        Puppy::mutateSwap(this->population_[treeIdx],
            this->context_,
            this->getDoubleParameter("MUT_SWAP_DISTRIB_PROBA"));
      }
    }
  }
};
template class GPLearnerWithDeterminantStrategy<Object>;
template class GPLearnerWithDeterminantStrategy<ObjectPair>;
template class GPLearnerWithDeterminantStrategy<ObjectList>;
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_DETERMINANT_STRATEGY_H_
