// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_SOLVER_SOLVER_BENCHMARK_H_
#define LTR_OPTIMIZATION_SOLVER_SOLVER_BENCHMARK_H_

#include <string>
#include <vector>
#include <stdexcept>


#include "ltr/utility/macros.h"
#include "ltr/optimization/functions/function.h"
#include "ltr/optimization/sets/set.h"
#include "ltr/optimization/population_initers/population_initer.h"
#include "ltr/optimization/population_updaters/population_updater.h"
#include "ltr/optimization/population_info/population.h"
#include "ltr/optimization/stop_criteria/stop_criterion.h"
#include "ltr/optimization/solver/solution.h"
#include "ltr/optimization/solver/solver.h"
#include "ltr/utility/multitable.h"

using std::string;
using std::vector;
using std::logic_error;
using ltr::utility::MultiTable;

namespace optimization {

/**
 * \brief A class to benchmark optimization Solver
 * with different test problems.
 */
template<typename TFunction>
class SolverBenchmark {
  public:
    explicit SolverBenchmark(int population_size);
    typedef MultiTable<string, 2> Table;

    /**
     * \brief Remove all solvers and problems from the stand.
     */
    void clear();
    void addProblem(typename TFunction::Ptr function, Set::Ptr set);
    void addSolver(const Solver<TFunction> &solver,
                   const string &alias);

    /**
     * \brief Run a benchmark using all solvers and problems specified
     * at the moment.
     * \return A 2-dimensional table where columns and rows represent 
     * problems and solvers respectively.
     */
    Table run() const;

  private:
    vector<typename TFunction::Ptr> functions;
    vector<Set::Ptr> sets;
    vector<Solver<TFunction> > solvers;
    vector<string> solver_aliases;

    int population_size_;
};

template<typename TFunction>
SolverBenchmark<TFunction>::SolverBenchmark(int population_size)
  : population_size_(population_size) {
}
template<typename TFunction>
void SolverBenchmark<TFunction>::clear() {
  functions.clear();
  sets.clear();
  solvers.clear();
  solver_aliases.clear();
}
template<typename TFunction>
void SolverBenchmark<TFunction>::addProblem(typename TFunction::Ptr function,
                                           Set::Ptr set) {
  functions.push_back(function);
  sets.push_back(set);
}
template<typename TFunction>
void SolverBenchmark<TFunction>::addSolver(const Solver<TFunction> &solver,
                                           const string &alias) {
  solvers.push_back(solver);
  solver_aliases.push_back(alias);
}
template<typename TFunction>
typename SolverBenchmark<TFunction>::Table
SolverBenchmark<TFunction>::run() const {
  CHECK_MSG(!functions.empty(), "No test problems specified for a benchmark");
  CHECK_MSG(!solvers.empty(), "No solvers specified for a benchmark");

  LexicalCastConfig::getInstance().restoreDefaults();

  int solvers_num = solvers.size();
  int problems_num = functions.size();

  vector<int> size(2);
  size[0] = solvers_num;
  size[1] = problems_num;

  Table table(size);

  table.setAxisLabel(0, "Solvers");
  table.setAxisLabel(1, "Problems");

  for (int solver_index = 0; solver_index < solvers_num; ++solver_index) {
    table.setTickLabel(0, solver_index, solver_aliases[solver_index]);
  }
  for (int problem_index = 0; problem_index < problems_num; ++problem_index) {
    table.setTickLabel(1, problem_index,
                       string("#") + lexical_cast<string>(problem_index + 1));
  }

  vector<int> multi_index(2);

  for (int solver_index = 0; solver_index < solvers_num; ++solver_index) {
    multi_index[0] = solver_index;
    for (int problem_index = 0; problem_index < problems_num; ++problem_index) {
      multi_index[1] = problem_index;
      Solution solution = solvers[solver_index].solve(functions[problem_index],
                                                      sets[problem_index],
                                                      population_size_);
      table[multi_index] = lexical_cast<string, double>(solution.min_value);
    }
  }

  return table;
}
}

#endif  // LTR_OPTIMIZATION_SOLVER_SOLVER_BENCHMARK_H_

