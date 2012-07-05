// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_GP_LEARNER_H_
#define LTR_LEARNERS_GP_LEARNER_GP_LEARNER_H_

#include <boost/lexical_cast.hpp>

#include <string>
#include <functional>

#include "contrib/puppy/Puppy.hpp"

#include "ltr/learners/learner.h"
#include "ltr/learners/gp_learner/gp_primitives.h"
#include "ltr/learners/gp_learner/gp_functions.h"
#include "ltr/scorers/gp_scorer.h"
#include "ltr/measures/measure.h"

#include "ltr/parameters_container/parameters_container.h"

namespace ltr {
namespace gp {
/**
 \brief GPLearner
 Implements genetic programming approach applied to learning to rank.
 \tparam TElement object container of those the dataset consists (it can be
 Object, ObjectPair, ObjectList).
 */
template <typename TElement>
class GPLearner : public BaseLearner<TElement, GPScorer> {
 public:
  /** Constructor creates a GPLearner.
   * \param measure shared pointer to the measure that would be maximized on
   * a dataset within learning.
   * \param parameters the ParametersContainer parameters from which would
   * overwrite the default parameters.
   */
  explicit GPLearner(typename Measure<TElement>::Ptr measure,
      const ParametersContainer& parameters)
  : feature_count_(0),
  best_tree_index_(0) {
    measure_ = measure;
    this->setParameters(parameters);
  }
  /** Constructor creates a GPLearner. But leaves p_measure uninitialized.
   * \param parameters the ParametersContainer parameters from which would
   * overwrite the default parameters.
   */
  explicit GPLearner(const ParametersContainer& parameters)
  : feature_count_(0),
  best_tree_index_(0) {
    this->setParameters(parameters);
  }

  explicit GPLearner(typename Measure<TElement>::Ptr measure,
    int pop_size = 10,
    int nbr_gen = 3,
    int max_depth = 35,
    int min_init_depth = 2,
    int max_init_depth = 5,
    int mut_max_regen_depth = 5,
    double init_grow_proba = 0.5,
    double crossover_distrib_proba = 0.5,
    double mut_swap_distrib_proba = 0.5,
    int seed = 1,
    bool use_add = true,
    bool use_sub = true,
    bool use_mul = true,
    bool use_div = true,
    bool use_if = true,
    bool use_efem = true,
    int nbr_part = 2,
    double crossover_proba = 0.9,
    double mut_std_proba = 0.05,
    double mut_swap_proba = 0.05)
  : feature_count_(0),
  best_tree_index_(0) {
    measure_ = measure;
    pop_size_ = pop_size;
    nbr_gen_ = nbr_gen;
    nbr_part_ = nbr_part;
    max_depth_ = max_depth;
    min_init_depth_ = min_init_depth;
    max_init_depth_ = max_init_depth;
    mut_max_regen_depth_ = mut_max_regen_depth;
    init_grow_proba_ = init_grow_proba;
    crossover_proba_ = crossover_proba;
    crossover_distrib_proba_ = crossover_distrib_proba;
    mut_std_proba_ = mut_std_proba;
    mut_swap_proba_ = mut_swap_proba;
    mut_swap_distrib_proba_ = mut_swap_distrib_proba;
    seed_ = seed;
    use_add_ = use_add;
    use_sub_ = use_sub;
    use_mul_ = use_mul;
    use_div_ = use_div;
    use_if_ = use_if;
    use_efem_ = use_efem;
  }

  explicit GPLearner(
    int pop_size = 10,
    int nbr_gen = 3,
    int max_depth = 35,
    int min_init_depth = 2,
    int max_init_depth = 5,
    int mut_max_regen_depth = 5,
    double init_grow_proba = 0.5,
    double crossover_distrib_proba = 0.5,
    double mut_swap_distrib_proba = 0.5,
    int seed = 1,
    bool use_add = true,
    bool use_sub = true,
    bool use_mul = true,
    bool use_div = true,
    bool use_if = true,
    bool use_efem = true,
    int nbr_part = 2,
    double crossover_proba = 0.9,
    double mut_std_proba = 0.05,
    double mut_swap_proba = 0.05)
    : feature_count_(0),
      best_tree_index_(0) {
    pop_size_ = pop_size;
    nbr_gen_ = nbr_gen;
    nbr_part_ = nbr_part;
    max_depth_ = max_depth;
    min_init_depth_ = min_init_depth;
    max_init_depth_ = max_init_depth;
    mut_max_regen_depth_ = mut_max_regen_depth;
    init_grow_proba_ = init_grow_proba;
    crossover_proba_ = crossover_proba;
    crossover_distrib_proba_ = crossover_distrib_proba;
    mut_std_proba_ = mut_std_proba;
    mut_swap_proba_ = mut_swap_proba;
    mut_swap_distrib_proba_ = mut_swap_distrib_proba;
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
    nbr_part_ = 2;
    max_depth_ = 35;
    min_init_depth_ = 2;
    max_init_depth_ = 5;
    mut_max_regen_depth_ = 5;
    init_grow_proba_ = 0.5;
    crossover_proba_ = 0.9;
    crossover_distrib_proba_ = 0.5;
    mut_std_proba_ = 0.05;
    mut_swap_proba_ = 0.05;
    mut_swap_distrib_proba_ = 0.5;
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
    CHECK(nbr_part_ > 1);
    CHECK(max_depth_ > 0);
    CHECK(min_init_depth_ > 0);
    CHECK(mut_max_regen_depth_ > 0);
    CHECK(max_init_depth_ > min_init_depth_ - 1);
    CHECK(0.0 <= init_grow_proba_ && init_grow_proba_ <= 1.0);
    CHECK(0.0 <= crossover_proba_ && crossover_proba_ <= 1.0);
    CHECK(0.0 <= crossover_distrib_proba_ && crossover_distrib_proba_ <= 1.0);
    CHECK(0.0 <= mut_std_proba_ && mut_std_proba_ <= 1.0);
    CHECK(0.0 <= mut_swap_proba_ && mut_swap_proba_ <= 1.0);
    CHECK(0.0 <= mut_swap_distrib_proba_ && mut_swap_distrib_proba_ <= 1.0);
  }
  /** The function recreates the context and reinitializes the population.
   */
  void reset() {
    this->initContext();
    this->initPopulation();
  }

  string toString() const {
    std::stringstream str;
    std::fixed(str);
    str.precision(2);
    str << "Genetic programming learner with parameters: POP_SIZE = ";
    str << this->parameters().Get<int>("POP_SIZE") << ", NBR_GEN = ";
    str << this->parameters().Get<int>("NBR_GEN") << ", NBR_PART = ";
    str << this->parameters().Get<int>("NBR_PART") << ", MAX_DEPTH = ";
    str << this->parameters().Get<int>("MAX_DEPTH") << ", MIN_INIT_DEPTH = ";
    str << this->parameters().Get<int>("MIN_INIT_DEPTH")
      << ", MAX_INIT_DEPTH = ";
    str << this->parameters().Get<int>("MAX_INIT_DEPTH")
      << ", INIT_GROW_PROBA = ";
    str << this->parameters().Get<double>("INIT_GROW_PROBA")
      << ", CROSSOVER_PROBA = ";
    str << this->parameters().Get<double>("CROSSOVER_PROBA")
      << ", CROSSOVER_DISTRIB_PROBA = ";
    str << this->parameters().Get<double>("CROSSOVER_DISTRIB_PROBA")
      << ", MUT_STD_PROBA = ";
    str << this->parameters().Get<double>("MUT_STD_PROBA")
      << ", MUT_MAX_REGEN_DEPTH = ";
    str << this->parameters().Get<int>("MUT_MAX_REGEN_DEPTH")
      << ", MUT_SWAP_PROBA = ";
    str << this->parameters().Get<double>("MUT_SWAP_PROBA")
      << ", MUT_SWAP_DISTRIB_PROBA = ";
    str << this->parameters().Get<double>("MUT_SWAP_DISTRIB_PROBA")
      << ", SEED = ";
    str << this->parameters().Get<int>("SEED") << ", USE_ADD = ";
    str << this->parameters().Get<bool>("USE_ADD") << ", USE_SUB = ";
    str << this->parameters().Get<bool>("USE_SUB") << ", USE_MUL = ";
    str << this->parameters().Get<bool>("USE_MUL") << ", USE_DIV = ";
    str << this->parameters().Get<bool>("USE_DIV") << ", USE_IF = ";
    str << this->parameters().Get<bool>("USE_IF") << ", USE_EFEM = ";
    str << this->parameters().Get<bool>("USE_EFEM");
    return str.str();
  }

  /** The function sets up context and population from the given GPScorer.
   * \param scorer GPScorer whose population and context would be set up.
   */
  void setInitialScorer(const GPScorer& scorer) {
    population_ = scorer.population_;
    context_ = scorer.context_;
    feature_count_ = scorer.feature_count_;
    best_tree_index_ = scorer.best_tree_index_;
  }

  GET_SET(int, pop_size);
  GET_SET(int, nbr_gen);
  GET_SET(int, nbr_part);
  GET_SET(int, max_depth);
  GET_SET(int, min_init_depth);
  GET_SET(int, max_init_depth);
  GET_SET(int, mut_max_regen_depth);
  GET_SET(double, init_grow_proba);
  GET_SET(double, crossover_proba);
  GET_SET(double, crossover_distrib_proba);
  GET_SET(double, mut_std_proba);
  GET_SET(double, mut_swap_proba);
  GET_SET(double, mut_swap_distrib_proba);
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
    nbr_part_ = parameters.Get<int>("NBR_PART");
    max_depth_ = parameters.Get<int>("MAX_DEPTH");
    min_init_depth_ = parameters.Get<int>("MIN_INIT_DEPTH");
    max_init_depth_ = parameters.Get<int>("MAX_INIT_DEPTH");
    mut_max_regen_depth_ = parameters.Get<int>("MUT_MAX_REGEN_DEPTH");
    init_grow_proba_ = parameters.Get<double>("INIT_GROW_PROBA");
    crossover_proba_ = parameters.Get<double>("CROSSOVER_PROBA");
    crossover_distrib_proba_ =
      parameters.Get<double>("CROSSOVER_DISTRIB_PROBA");
    mut_std_proba_ = parameters.Get<double>("MUT_STD_PROBA");
    mut_swap_proba_ = parameters.Get<double>("MUT_SWAP_PROBA");
    mut_swap_distrib_proba_ = parameters.Get<double>("MUT_SWAP_DISTRIB_PROBA");
    seed_ = parameters.Get<int>("SEED");
    use_add_ = parameters.Get<bool>("USE_ADD");
    use_sub_ = parameters.Get<bool>("USE_SUB");
    use_mul_ = parameters.Get<bool>("USE_MUL");
    use_div_ = parameters.Get<bool>("USE_DIV");
    use_if_ = parameters.Get<bool>("USE_IF");
    use_efem_ = parameters.Get<bool>("USE_EFEM");
  }

  virtual string getDefaultAlias() const {return "GPLeaner";}
  /** Method clears and adds primitives to the context.
   */
  void initContext() {
    Puppy::Context newContext;
    context_= newContext;

    const ParametersContainer &params = this->parameters();

    context_.mRandom.seed(seed_);
    if (use_add_) context_.insert(new Add);
    if (use_sub_) context_.insert(new Subtract);
    if (use_mul_) context_.insert(new Multiply);
    if (use_div_) context_.insert(new Divide);
    if (use_if_)  context_.insert(new IfThenFunc);
    if (use_efem_) context_.insert(new Ephemeral);

    for (int feature_index = 0;
        feature_index < feature_count_;
        ++feature_index) {
      std::string feature_name = "feature[";
      feature_name += boost::lexical_cast<string>(feature_index);
      feature_name += "]";
      context_.insert(new Puppy::TokenT<double>(feature_name));
    }
  }

  /** Method creates initial population.
   */
  void initPopulation() {
    population_.clear();
    ParametersContainer params = this->parameters();

    population_.resize(pop_size_);
    Puppy::initializePopulation(population_, context_, init_grow_proba_,
                                min_init_depth_, max_init_depth_);
  }

  /** \brief This function implements the changes made in the population at each
   *  algorithm's iteration.
   */
  virtual void evaluationStepImpl() {
    std::cout << "Tournament.\n";
    ParametersContainer params = this->parameters();

    Puppy::applySelectionTournament(population_, context_, nbr_part_);

    std::cout << "Crossover.\n";
    Puppy::applyCrossover(population_, context_, crossover_proba_,
                          crossover_distrib_proba_, max_depth_);

    std::cout << "Mutation standart.\n";
    Puppy::applyMutationStandard(population_, context_, mut_std_proba_,
                                 mut_max_regen_depth_, max_depth_);

    std::cout << "Mutation swap.\n";
    Puppy::applyMutationSwap(population_, context_, mut_swap_proba_,
                             mut_swap_distrib_proba_);
  }

  /** The implementation of genetic programming optimization approach.
   * \param data DataSet on which the measure would be maximized within the
   * learning procedure.
   */
  void learnImpl(const DataSet<TElement>& data, GPScorer* scorer) {
    if (data.feature_count() != feature_count_) {
      feature_count_ = data.feature_count();
      reset();
    }

    std::cout << "Evaluating data for the first time.\n";
    this->evaluatePopulation(data);

    std::cout << "The population looks like: \n";
    for (size_t tree_index = 0; tree_index < population_.size(); ++tree_index) {
      using ::operator <<;
      std::cout << population_[tree_index] << std::endl;
    }

    std::cout << "Evolution begins.\n";
    for (int generationIdx = 0;
       generationIdx < nbr_gen_;
       ++generationIdx) {
      std::cout << "Generation "<< generationIdx << ".\n";

      std::cout << "Calling strategy\n";
      this->evaluationStepImpl();

      std::cout << "Evaluation.\n";
      this->evaluatePopulation(data);

      best_tree_index_ = 0;
      for (size_t tree_index = 1;
          tree_index < population_.size(); ++tree_index) {
        if (population_[best_tree_index_].mFitness <
            population_[tree_index].mFitness) {
          best_tree_index_ = tree_index;
        }
      }

      std::cout
      << "The best one is number " << best_tree_index_ << ".\n";
      using ::operator <<;
      std::cout << population_[best_tree_index_] << std::endl;
      std::cout << "with fitness " <<
          population_[best_tree_index_].mFitness << "\n";
    }
    // \TODO ? rewrite with setters and getters
    *scorer = GPScorer(population_, context_,
                       feature_count_, best_tree_index_);
  }

  /** Method evaluates the population, it sets individ tree fitness to the
   *  average on the data set metric value.
   *  @param data data set for calculation of the average metric value
   */
  void evaluatePopulation(const DataSet<TElement>& data) {
    for (size_t tree_index = 0; tree_index < population_.size(); ++tree_index) {
      if (population_[tree_index].mValid) {
        continue;
      }
      markDataSetWithTree<TElement>(data, &context_, &population_[tree_index]);

      // This line yields a topic for research. Why so?
      //
      double measureVal = this->measure_->weightedAverage(data);

      population_[tree_index].mFitness = static_cast<float>(measureVal);
      population_[tree_index].mValid = true;
    }
  }

  size_t feature_count_;
  /** The index of the best Puppy::tree (formula, individ) in current
   * population.
   */
  size_t best_tree_index_;

  typename Measure<TElement>::Ptr measure_;

  int pop_size_;
  int nbr_gen_;
  int nbr_part_;
  int max_depth_;
  int min_init_depth_;
  int max_init_depth_;
  int mut_max_regen_depth_;
  double init_grow_proba_;
  double crossover_proba_;
  double crossover_distrib_proba_;
  double mut_std_proba_;
  double mut_swap_proba_;
  double mut_swap_distrib_proba_;
  int seed_;
  bool use_add_;
  bool use_sub_;
  bool use_mul_;
  bool use_div_;
  bool use_if_;
  bool use_efem_;

 protected:
  /** The set of Puppy::trees (formulas that represent the optimization space),
   * that represent a population within the learning procedure.
   */
  Population population_;
  /** Context that contains all the routine needed to build Puppy::trees within
   * the population.
   */
  Puppy::Context context_;
  /** The number of features, to process dataset with the number of features
   * the context_ is constructed for.
   */
};
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_GP_LEARNER_H_
