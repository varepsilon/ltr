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
  /** Constructor creates a GPLearnerWithDeterminantStrategy. But leaves
   * p_measure uninitialized.
   * \param parameters the ParametersContainer parameters from which would
   * overwrite the default parameters.
   */
  explicit GPLearnerWithDeterminantStrategy(
      const ParametersContainer& parameters)
  : GPLearner<TElement>(parameters) {
    this->setParameters(parameters);
  }

  explicit GPLearnerWithDeterminantStrategy(
    typename Measure<TElement>::Ptr measure,
    int pop_size = 10,
    int nbr_gen = 3,
    int max_depth = 35,
    int min_init_depth = 2,
    int max_init_depth = 5,
    int mut_max_regen_depth = 5,
    double init_grow_proba = 0.5,
    double crossover_distrib_proba = 0.3,
    double standart_mutation_vs_swap_mutation_proba = 0.5,
    double mut_swap_distrib_proba = 0.5,
    double top_for_next_generation_part = 0.3,
    double crossover_vs_mutation_part = 0.7,
    int seed = 1,
    bool use_add = true,
    bool use_sub = true,
    bool use_mul = true,
    bool use_div = true,
    bool use_if = true,
    bool use_efem = true)
  : GPLearner<TElement>(measure,
  pop_size,
  nbr_gen,
  max_depth,
  min_init_depth,
  max_init_depth,
  mut_max_regen_depth,
  init_grow_proba,
  crossover_distrib_proba,
  mut_swap_distrib_proba,
  seed,
  use_add,
  use_sub,
  use_mul,
  use_div,
  use_if,
  use_efem) {
    pop_size_ = pop_size;
    nbr_gen_ = nbr_gen;
    max_depth_ = max_depth;
    min_init_depth_ = min_init_depth;
    max_init_depth_ = max_init_depth;
    mut_max_regen_depth_ = mut_max_regen_depth;
    init_grow_proba_ = init_grow_proba;
    crossover_distrib_proba_ = crossover_distrib_proba;
    standart_mutation_vs_swap_mutation_proba_ =
      standart_mutation_vs_swap_mutation_proba;
    mut_swap_distrib_proba_ = mut_swap_distrib_proba;
    top_for_next_generation_part_ = top_for_next_generation_part;
    crossover_vs_mutation_part_ = crossover_vs_mutation_part;
    seed_ = seed;
    use_add_ = use_add;
    use_sub_ = use_sub;
    use_mul_ = use_mul;
    use_div_ = use_div;
    use_if_ = use_if;
    use_efem_ = use_efem;
  }

  explicit GPLearnerWithDeterminantStrategy(
    int pop_size = 10,
    int nbr_gen = 3,
    int max_depth = 35,
    int min_init_depth = 2,
    int max_init_depth = 5,
    int mut_max_regen_depth = 5,
    double init_grow_proba = 0.5,
    double crossover_distrib_proba = 0.3,
    double standart_mutation_vs_swap_mutation_proba = 0.5,
    double mut_swap_distrib_proba = 0.5,
    double top_for_next_generation_part = 0.3,
    double crossover_vs_mutation_part = 0.7,
    int seed = 1,
    bool use_add = true,
    bool use_sub = true,
    bool use_mul = true,
    bool use_div = true,
    bool use_if = true,
    bool use_efem = true)
  : GPLearner<TElement>(
  pop_size,
  nbr_gen,
  max_depth,
  min_init_depth,
  max_init_depth,
  mut_max_regen_depth,
  init_grow_proba,
  crossover_distrib_proba,
  mut_swap_distrib_proba,
  seed,
  use_add,
  use_sub,
  use_mul,
  use_div,
  use_if,
  use_efem) {
    pop_size_ = pop_size;
    nbr_gen_ = nbr_gen;
    max_depth_ = max_depth;
    min_init_depth_ = min_init_depth;
    max_init_depth_ = max_init_depth;
    mut_max_regen_depth_ = mut_max_regen_depth;
    init_grow_proba_ = init_grow_proba;
    crossover_distrib_proba_ = crossover_distrib_proba;
    standart_mutation_vs_swap_mutation_proba_ =
      standart_mutation_vs_swap_mutation_proba;
    mut_swap_distrib_proba_ = mut_swap_distrib_proba;
    top_for_next_generation_part_ = top_for_next_generation_part;
    crossover_vs_mutation_part_ = crossover_vs_mutation_part;
    seed_ = seed;
    use_add_ = use_add;
    use_sub_ = use_sub;
    use_mul_ = use_mul;
    use_div_ = use_div;
    use_if_ = use_if;
    use_efem_ = use_efem;
  }

  /** The function sets up default parameters for genetic learning process.
   */
  virtual void setDefaultParameters() {
    pop_size_ = 10;
    nbr_gen_ = 3;
    max_depth_ = 35;
    min_init_depth_ = 2;
    max_init_depth_ = 5;
    mut_max_regen_depth_ = 5;
    init_grow_proba_ = 0.5;
    crossover_distrib_proba_ = 0.3;
    standart_mutation_vs_swap_mutation_proba_ = 0.5;
    mut_swap_distrib_proba_ = 0.5;
    top_for_next_generation_part_ = 0.3;
    crossover_vs_mutation_part_ = 0.7;
    seed_ = 1;
    use_add_ = true;
    use_sub_ = true;
    use_mul_ = true;
    use_div_ = true;
    use_if_ = true;
    use_efem_ = true;
  }
  /** The method checks the correctness of the parameters in the parameters
   * container. If one of them is not correct it throws
   * std::logical_error(PARAMETER_NAME).
   */
  virtual void checkParameters() const {
    CHECK(pop_size_ > 0);
    CHECK(nbr_gen_ > 0);
    CHECK(max_depth_ > 0);
    CHECK(min_init_depth_ > 0);
    CHECK(mut_max_regen_depth_ > 0);
    CHECK(max_init_depth_ > min_init_depth_ - 1);
    CHECK(0.0 <= init_grow_proba_ && init_grow_proba_ <= 1.0);
    CHECK(0.0 <= crossover_distrib_proba_ && crossover_distrib_proba_ <= 1.0);
    CHECK(0.0 <= mut_swap_distrib_proba_ && mut_swap_distrib_proba_ <= 1.0);
    CHECK(0.0 <= standart_mutation_vs_swap_mutation_proba_ &&
      standart_mutation_vs_swap_mutation_proba_ <= 1.0);
    CHECK(0.0 <= top_for_next_generation_part_ &&
      top_for_next_generation_part_ <= 1.0);
    CHECK(0.0 <= crossover_vs_mutation_part_ &&
      crossover_vs_mutation_part_ <= 1.0);
  }

  GET_SET(int, pop_size);
  GET_SET(int, nbr_gen);
  GET_SET(int, max_depth);
  GET_SET(int, min_init_depth);
  GET_SET(int, max_init_depth);
  GET_SET(int, mut_max_regen_depth);
  GET_SET(double, init_grow_proba);
  GET_SET(double, crossover_distrib_proba);
  GET_SET(double, standart_mutation_vs_swap_mutation_proba);
  GET_SET(double, mut_swap_distrib_proba);
  GET_SET(double, top_for_next_generation_part);
  GET_SET(double, crossover_vs_mutation_part);
  GET_SET(int, seed);
  GET_SET(bool, use_add);
  GET_SET(bool, use_sub);
  GET_SET(bool, use_mul);
  GET_SET(bool, use_div);
  GET_SET(bool, use_if);
  GET_SET(bool, use_efem);

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters) {
    pop_size_ = parameters.Get<int>("POP_SIZE");
    nbr_gen_ = parameters.Get<int>("NBR_GEN");
    max_depth_ = parameters.Get<int>("MAX_DEPTH");
    min_init_depth_ = parameters.Get<int>("MIN_INIT_DEPTH");
    max_init_depth_ = parameters.Get<int>("MAX_INIT_DEPTH");
    mut_max_regen_depth_ = parameters.Get<int>("MUT_MAX_REGEN_DEPTH");
    init_grow_proba_ = parameters.Get<double>("INIT_GROW_PROBA");
    crossover_distrib_proba_ =
      parameters.Get<double>("CROSSOVER_DISTRIB_PROBA");
    standart_mutation_vs_swap_mutation_proba_ =
      parameters.Get<double>("STANDART_MUTATION_VS_SWAP_MUTATION_PROBA");
    mut_swap_distrib_proba_ = parameters.Get<double>("MUT_SWAP_DISTRIB_PROBA");
    top_for_next_generation_part_ =
      parameters.Get<double>("TOP_FOR_NEXT_GENERATION_PART");
    crossover_vs_mutation_part_ =
      parameters.Get<double>("CROSSOVER_VS_MUTATION_PART");
    seed_ = parameters.Get<int>("SEED");
    use_add_ = parameters.Get<bool>("USE_ADD");
    use_sub_ = parameters.Get<bool>("USE_SUB");
    use_mul_ = parameters.Get<bool>("USE_MUL");
    use_div_ = parameters.Get<bool>("USE_DIV");
    use_if_ = parameters.Get<bool>("USE_IF");
    use_efem_ = parameters.Get<bool>("USE_EFEM");
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

    ParametersContainer params = this->parameters();
    size_t top_bound_index = (1 - top_for_next_generation_part_) *
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

    size_t crossover_bound_index = crossover_vs_mutation_part_
        * top_bound_index;
    if (crossover_bound_index % 2 != 1) {
      crossover_bound_index -= 1;
    }

    for (size_t tree_index = 0;
        tree_index < crossover_bound_index; tree_index += 2) {
      Puppy::mateTrees(this->population_[tree_index],
                       this->population_[tree_index + 1],
                       this->context_,
                       crossover_distrib_proba_,
                       max_depth_);
    }

    for (size_t tree_index = crossover_bound_index;
        tree_index < top_bound_index; ++tree_index) {
      if (this->context_.mRandom.rollUniform() <=
          standart_mutation_vs_swap_mutation_proba_) {
        Puppy::mutateStandard(this->population_[tree_index],
            this->context_,
            mut_max_regen_depth_,
            max_depth_);
      } else {
        Puppy::mutateSwap(this->population_[tree_index],
            this->context_,
            mut_swap_distrib_proba_);
      }
    }
  }

  int pop_size_;
  int nbr_gen_;
  int max_depth_;
  int min_init_depth_;
  int max_init_depth_;
  int mut_max_regen_depth_;
  double init_grow_proba_;
  double crossover_distrib_proba_;
  double standart_mutation_vs_swap_mutation_proba_;
  double mut_swap_distrib_proba_;
  double top_for_next_generation_part_;
  double crossover_vs_mutation_part_;
  int seed_;
  bool use_add_;
  bool use_sub_;
  bool use_mul_;
  bool use_div_;
  bool use_if_;
  bool use_efem_;
};
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_GP_LEARNER_DETERMINANT_STRATEGY_H_
