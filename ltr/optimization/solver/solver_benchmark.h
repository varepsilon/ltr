// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_SOLVER_SOLVER_BENCHMARK_H_
#define LTR_OPTIMIZATION_SOLVER_SOLVER_BENCHMARK_H_

#include <iostream>  // NOLINT
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
using std::stringstream;
using ltr::utility::MultiTable;
using optimization::Matrix;

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
    typedef Eigen::Matrix<Solution, -1, -1> MatrixSolution;

    /**
     * \brief Remove all solvers and problems from the stand.
     */
    void clear();
    void addProblem(typename TFunction::Ptr function, Set::Ptr set);
    void addSolver(const Solver<TFunction> &solver);

    /**
     * \brief Run a benchmark using all solvers and problems specified
     * at the moment.
     * \return A 2-dimensional table where columns and rows represent 
     * problems and solvers respectively.
     */
    Table run() const;
    /**
     * Runs a benchmark several times using all solvers and problems specified
     * at the monemt.
     *
     * \return A 2-dimensional table of (mean+-standard deviation) where
     * colums and rows represent problems and solvers respectively.
     */
    Table multiRun(int run_count) const;

  private:
    /**
     * Sets axis's and tick's label using solvers and problems.
     */
    void initTable(Table* table) const;
    /**
     * Gets mean for every cell of the matrix
     */
    Matrix mean(const vector<Matrix>& matrices) const;
    /**
     * Gets stantard deviation for every cell of the matrix.
     */
    Matrix standardDeviation(const vector<Matrix>& matrices) const;

    vector<typename TFunction::Ptr> functions;
    vector<Set::Ptr> sets;
    vector<Solver<TFunction> > solvers;

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
}

template<typename TFunction>
void SolverBenchmark<TFunction>::addProblem(typename TFunction::Ptr function,
                                           Set::Ptr set) {
  functions.push_back(function);
  sets.push_back(set);
}

template<typename TFunction>
void SolverBenchmark<TFunction>::addSolver(const Solver<TFunction> &solver) {
  solvers.push_back(solver);
}

template<typename TFunction>
typename SolverBenchmark<TFunction>::Table
SolverBenchmark<TFunction>::run() const {
  Table table;
  initTable(&table);

  vector<int> multi_index(2);

  for (int solver_index = 0; solver_index < solvers.size(); ++solver_index) {
    multi_index[0] = solver_index;
    for (int problem_index = 0;
         problem_index < functions.size();
         ++problem_index) {
      multi_index[1] = problem_index;
      Solution solution = solvers[solver_index].solve(functions[problem_index],
                                                      sets[problem_index],
                                                      population_size_);
      table[multi_index] = lexical_cast<string, double>(solution.min_value);
    }
  }

  return table;
}

template <typename TFunction>
typename SolverBenchmark<TFunction>::Table
SolverBenchmark<TFunction>::multiRun(int run_count) const {
  Table table;
  initTable(&table);

  vector<int> multi_index(2);

  vector<Matrix> solutions;
  for (int run_index = 0; run_index < run_count; ++run_index) {
    Matrix solution(solvers.size(), functions.size());
    for (int solver_index = 0; solver_index < solvers.size(); ++solver_index) {
      for (int problem_index = 0;
           problem_index < functions.size();
           ++problem_index) {
        solution(solver_index, problem_index) = solvers[solver_index].solve(
            functions[problem_index],
            sets[problem_index],
            population_size_).min_value;
      }
    }
    solutions.push_back(solution);
  }
  Matrix mean_matrix = mean(solutions);
  Matrix standard_deviation_matrix = standardDeviation(solutions);
  for (int solver_index = 0; solver_index < solvers.size(); ++solver_index) {
    multi_index[0] = solver_index;
    for (int problem_index = 0;
         problem_index < functions.size();
         ++problem_index) {
      multi_index[1] = problem_index;
      string cell = lexical_cast<string>(
          mean_matrix(solver_index, problem_index));
      cell += "+-";
      cell += lexical_cast<string>(
          standard_deviation_matrix(solver_index, problem_index));
      table[multi_index] = cell;
    }
  }
  return table;
}

template <class TFunction>
void SolverBenchmark<TFunction>::initTable(Table* table) const {
  CHECK_MSG(!functions.empty(), "No test problems specified for a benchmark");
  CHECK_MSG(!solvers.empty(), "No solvers specified for a benchmark");

  int solvers_count = solvers.size();
  int problems_count = functions.size();

  vector<int> size(2);
  size[0] = solvers_count;
  size[1] = problems_count;

  table->resize(size);

  table->setAxisLabel(0, "Solvers");
  table->setAxisLabel(1, "Problems");

  for (int solver_index = 0; solver_index < solvers_count; ++solver_index) {
    table->setTickLabel(0, solver_index, solvers[solver_index].alias());
  }
  for (int problem_index = 0; problem_index < problems_count; ++problem_index) {
    table->setTickLabel(1, problem_index,
                        string("#") + lexical_cast<string>(problem_index + 1));
  }
}

template <class TFunction>
Matrix SolverBenchmark<TFunction>::mean(
    const vector<Matrix>& matrices) const {
  Matrix mean_matrix = matrices[0];
  for (int matrix_index = 1; matrix_index < matrices.size(); ++matrix_index) {
    mean_matrix += matrices[matrix_index];
  }
  mean_matrix /= (double)matrices.size();
  return mean_matrix;
}

template <class TFunction>
Matrix SolverBenchmark<TFunction>::standardDeviation(
    const vector<Matrix>& matrices) const {
  CHECK_MSG(matrices.size() >= 2,
            "Not enough elements to get standard deviation");
  Matrix mean_matrix = mean(matrices);
  Matrix variance_matrix = Matrix::Zero(solvers.size(), functions.size());
  for (int matrix_index = 0; matrix_index < matrices.size(); ++matrix_index) {
    Matrix subtraction = mean_matrix - matrices[matrix_index];
    variance_matrix += subtraction.cwiseProduct(subtraction);
  }
  variance_matrix /= (double)matrices.size() - 1.0;
  return variance_matrix.cwiseSqrt();
}
}

#endif  // LTR_OPTIMIZATION_SOLVER_SOLVER_BENCHMARK_H_
