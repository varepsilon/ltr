// Copyright 2012 Yandex

#include "ltr/learners/gp_learner/strategies/select_top_trees_strategy.h"

namespace ltr {
namespace gp {
void SelectTopTreesStrategy::
  handlePopulation(vector<Tree>& population, Context& context) { // NOLINT
    this->checkParameters();

    std::sort(population.rbegin(), population.rend());

    int top_bound_index = ceil(top_fraction_ * population.size());
    for (int tree_index = top_bound_index;
         tree_index < population.size(); ++tree_index) {
      population[tree_index] = population[tree_index % top_bound_index];
    }
}

void SelectTopTreesStrategy::setDefaultParameters() {
  top_fraction_ = 0.3;
}

void SelectTopTreesStrategy::checkParameters() const {
  CHECK(0 < top_fraction_ && top_fraction_ <= 1);
}

void SelectTopTreesStrategy::
  setParametersImpl(const ParametersContainer& parameters) {
    top_fraction_ = parameters.Get<double>("TOP_FRACTION");
}
}
}
