// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_GP_LEARNER_DETERMINANT_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_GP_LEARNER_DETERMINANT_STRATEGY_H_

#include "ltr/learners/gp_learner/gp_learner.h"

#include <algorithm>
#include <string>
#include <functional>

using std::string;

namespace ltr {
namespace gp {

template <typename TElement>
class GPLearnerWithDeterminantStrategy : public GPLearner<TElement> {
 public:
  /** Constructor creates a GPLearnerWithDeterminantStrategy.
   * \param measure shared pointer to the measure that would be maximized on
   * a dataset within learning.
   * \param parameters the ParametersContainer parameters from which would
   * overwrite the default parameters.
   */
  GPLearnerWithDeterminantStrategy(typename Measure<TElement>::Ptr measure,
      const ParametersContainer& parameters = ParametersContainer())
  : GPLearner<TElement>(measure, parameters) {
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
    this->addNewParam("POP_SIZE", 10);
    this->addNewParam("NBR_GEN", 3);
    this->addNewParam("MAX_DEPTH", 35);
    this->addNewParam("MIN_INIT_DEPTH", 2);
    this->addNewParam("MAX_INIT_DEPTH", 5);
    this->addNewParam("INIT_GROW_PROBA", 0.5);
    this->addNewParam("TOP_FOR_NEXT_GENERATION_PART", 0.3);
    this->addNewParam("CROSSOVER_VS_MUTATION_PART", 0.7);
    this->addNewParam("CROSSOVER_DISTRIB_PROBA", 0.3);
    this->addNewParam("STANDART_MUTATION_VS_SWAP_MUTATION_PROBA", 0.5);
    this->addNewParam("MUT_MAX_REGEN_DEPTH", 5);
    this->addNewParam("MUT_SWAP_DISTRIB_PROBA", 0.5);
    this->addNewParam("SEED", 1);
    this->addNewParam("USE_ADD", true);
    this->addNewParam("USE_SUB", true);
    this->addNewParam("USE_MUL", true);
    this->addNewParam("USE_DIV", true);
    this->addNewParam("USE_IF", true);
    this->addNewParam("USE_EFEM", true);
  }
  /** The method checks the correctness of the parameters in the parameters
   * container. If one of them is not correct it throws
   * std::logical_error(PARAMETER_NAME).
   */
  virtual void checkParameters() const {
    std::binder2nd<std::greater<int> > GreaterThen0 =
        std::bind2nd(std::greater<int>(), 0);

    Parameterized::checkParameter<int>("POP_SIZE",            GreaterThen0);
    Parameterized::checkParameter<int>("NBR_GEN",             GreaterThen0);
    Parameterized::checkParameter<int>("MAX_DEPTH",           GreaterThen0);
    Parameterized::checkParameter<int>("MUT_MAX_REGEN_DEPTH", GreaterThen0);
    Parameterized::checkParameter<int>("MIN_INIT_DEPTH",      GreaterThen0);

    Parameterized::checkParameter<int>("MAX_INIT_DEPTH",
      std::bind2nd(std::greater<int>(), this->parameters().
                          template Get<int>("MIN_INIT_DEPTH") - 1));

    const Belongs<double> G0L1(0, 1);
    Parameterized::checkParameter<double>("INIT_GROW_PROBA",         G0L1);
    Parameterized::checkParameter<double>("CROSSOVER_DISTRIB_PROBA", G0L1);
    Parameterized::checkParameter<double>("MUT_SWAP_DISTRIB_PROBA",  G0L1);
    Parameterized::checkParameter<double>("TOP_FOR_NEXT_GENERATION_PART",
                                         G0L1);
    Parameterized::checkParameter<double>("CROSSOVER_VS_MUTATION_PART",
                                         G0L1);
    Parameterized::checkParameter<double>(
       "STANDART_MUTATION_VS_SWAP_MUTATION_PROBA",
                                          G0L1);
  }

 private:
    template <class T>
    struct Belongs: public std::unary_function<T, bool> {
      Belongs(const T &min, const T &max): min_(min), max_(max) { }
      bool operator()(const T& x) const {
        return x >= min_ && x <= max_;
      }
    private:
      const T &min_;
      const T &max_;
    };

  virtual string getDefaultAlias() const {
    return "GPLearnerWithDeterminantStrategy";
  }

  /** \brief This function implements the changes made in the population at each
   *  algorithm's iteration.
   */
  virtual void evaluationStepImpl() {
    assert(this->population_.size() > 0);

    std::cout << "Population looks like:\n";
    for (size_t tree_index = 0;
        tree_index < this->population_.size(); ++tree_index) {
      std::cout << this->population_.at(tree_index).mFitness
          << "(" << this->population_.at(tree_index).mValid <<") ";
    }
    std::cout << std::endl;

    std::cout << "Sorting population.\n";
    std::sort(this->population_.begin(), this->population_.end());

    const ParametersContainer &params = this->parameters();
    size_t top_bound_index =
        (1 - params.template Get<double>("TOP_FOR_NEXT_GENERATION_PART")) *
        this->population_.size();
    if (top_bound_index == this->population_.size()) {
      top_bound_index -= 1;
    }

    size_t currrent_top_tree_index = top_bound_index;
    for (size_t tree_index = 0; tree_index < top_bound_index; ++tree_index) {
      this->population_.at(tree_index)
        = this->population_.at(currrent_top_tree_index);
      ++currrent_top_tree_index;
      if (currrent_top_tree_index >= this->population_.size()) {
        currrent_top_tree_index = top_bound_index;
      }
    }

    size_t crossover_bound_index =
        params.template Get<double>("CROSSOVER_VS_MUTATION_PART")
        * top_bound_index;
    if (crossover_bound_index % 2 != 1) {
      crossover_bound_index -= 1;
    }

    for (size_t tree_index = 0;
        tree_index < crossover_bound_index; tree_index += 2) {
      Puppy::mateTrees(this->population_[tree_index],
                       this->population_[tree_index + 1],
                       this->context_,
                       params.template Get<double>("CROSSOVER_DISTRIB_PROBA"),
                       params.template Get<int>("MAX_DEPTH"));
    }

    for (size_t tree_index = crossover_bound_index;
        tree_index < top_bound_index; ++tree_index) {
      if (this->context_.mRandom.rollUniform() <=
          params.template Get<double>(
              "STANDART_MUTATION_VS_SWAP_MUTATION_PROBA")) {
        Puppy::mutateStandard(this->population_[tree_index],
            this->context_,
            params.template Get<int>("MUT_MAX_REGEN_DEPTH"),
            params.template Get<int>("MAX_DEPTH"));
      } else {
        Puppy::mutateSwap(this->population_[tree_index],
            this->context_,
            params.template Get<double>("MUT_SWAP_DISTRIB_PROBA"));
      }
    }
  }
};
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_GP_LEARNER_DETERMINANT_STRATEGY_H_
