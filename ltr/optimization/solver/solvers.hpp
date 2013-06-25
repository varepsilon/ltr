//  copyright 2013 yandex

#ifndef LTR_OPTIMIZATION_SOLVER_SOLVERS_HPP_
#define LTR_OPTIMIZATION_SOLVER_SOLVERS_HPP_

#include <string>
#include "ltr/optimization/solver/solver.h"
#include "ltr/utility/boost/shared_ptr.h"

namespace optimization {
/**
 * \brief Class for solve problem via gradient descent with fixed step method.
 * it uses gradient magnitude stop criterion
 *
 * \see PopulationUpdater
 */
template<class TFunction>
class GradientDescentSolver : public Solver<TFunction> {
 public:
  typedef ltr::utility::shared_ptr<GradientDescentSolver> Ptr;  // NOLINT
  /**
   * Ctor for creating solver with
   * -gradient descent updater with fixed_step
   * -point gradient magnitude stop criterion with min_gradient_magnitude
   *     and stop_criteria_quorum (by default, 1.0)
   *
   * \param fixed_step step for gradient descent method
   * \param min_gradient_magnitude for gradient magnitude stop criterion
   * \param stop_criteria_quorum minimal part of true answers from per point stop criterion
   */
  GradientDescentSolver(double fixed_step,
                        double min_gradient_magnitude,
                        double stop_criteria_quorum = 1.0)
     : Solver<TFunction>(new NaiveIniter,
                         new PerPointUpdater<TFunction>(
                             new FixedStepGradientUpdater<TFunction>(
                                 fixed_step)),
                         new PerPointStopCriterion<TFunction>(
                             new GradientMagnitudeStopCriterion<TFunction>(
                                 min_gradient_magnitude),
                             stop_criteria_quorum)) { }
  /**
   * Ctor for creating solver with
   *   -gradient descent updater with fixed_step
   *   -stop_criterion
   *
   * \param fixed_step step for gradient descent method
   * \param stop_criterion population stop criterion
   */
  GradientDescentSolver(double fixed_step,
                        typename StopCriterion<TFunction>::Ptr stop_criterion)
      : Solver<TFunction>(new NaiveIniter,
                          new PerPointUpdater<TFunction>(
                              new FixedStepGradientUpdater<TFunction>(
                                  fixed_step)),
                          stop_criterion) { }
  /**
   * Ctor for creating solver with
   *   -gradient descent updater with fixed_step
   *   -one_point_stop_criterion
   *
   * \param fixed_step step for gradient descent method
   * \param stop_criterion_sample one point stop criterion
   * \param stop_criteria_quorum minimal part of true answers from per point stop criterion
   * sample for per point stop criterion
   */
  GradientDescentSolver(double fixed_step,
                        typename OnePointStopCriterion<TFunction>::Ptr
                            stop_criterion_sample,
                        double stop_criteria_quorum = 1.0)
     : Solver<TFunction>(new NaiveIniter,
                         new PerPointUpdater<TFunction>(
                             new FixedStepGradientUpdater<TFunction>(
                                 fixed_step)),
                         new PerPointStopCriterion<TFunction>(
                             stop_criterion_sample,
                             stop_criteria_quorum)) { }

  ~GradientDescentSolver() {}

  virtual string getDefaultAlias() { return "GradientDescentSolver"; }
};

template<class TFunction>
class SteepestGradientSolver : public Solver<TFunction> {
 public:
  typedef ltr::utility::shared_ptr<SteepestGradientSolver> Ptr;  // NOLINT
  /**
   * Ctor for creating solver with
   *   -steepest gradient descent updater max_step and brute_force_step
   *   -per point gradient magnitude stop criterion with min_gradient_magnitude
   *     and stop_criteria_quorum (by default, 1.0)
   *
   * \param max_step maximal step for steepest gradient descent method
   * \param brute_force_step brute force step for steepest gradient descent method
   * \param min_gradient_magnitude for gradient magnitude stop criterion
   * \param stop_criteria_quorum minimal part of true answers from per point stop criterion
   */
  SteepestGradientSolver(double max_step,
                         double brute_force_step,
                         double min_gradient_magnitude,
                         double stop_criteria_quorum = 1.0)
     : Solver<TFunction>(new NaiveIniter,
                         new PerPointUpdater<TFunction>(
                             new SteepestGradientUpdater<TFunction>(
                                 max_step,
                                 brute_force_step)),
                         new PerPointStopCriterion<TFunction>(
                             new GradientMagnitudeStopCriterion<TFunction>(
                                 min_gradient_magnitude),
                             stop_criteria_quorum)) { }
  /**
   * Ctor for creating solver with
   *   -steepest gradient descent updater max_step and brute_force_step
   *   -stop_criterion
   *
   * \param max_step maximal step for steepest gradient descent method
   * \param brute_force_step brute force step for steepest gradient descent method
   * \param stop_criterion population stop criterion
   */
  SteepestGradientSolver(double max_step,
                         double brute_force_step,
                         typename StopCriterion<TFunction>::Ptr stop_criterion)
      : Solver<TFunction>(new NaiveIniter,
                          new PerPointUpdater<TFunction>(
                              new SteepestGradientUpdater<TFunction>(
                                  max_step,
                                  brute_force_step)),
                          stop_criterion) { }
  /**
   * Ctor for creating solver with
   *   -steepest gradient descent updater max_step and brute_force_step
   *   -per point stop criterion with one_point_stop_criterion_sample and
   *      stop_criteria_quorum (by default, 1.0)
   *
   * \param max_step maximal step for steepest gradient descent method
   * \param brute_force_step brute force step for steepest gradient descent method
   * \param one_point_stop_criterion_sample one point stop criterion
   * sample for per point stop criterion
   * \param stop_criteria_quorum minimal part of true answers from per point stop criterion
   */
  SteepestGradientSolver(double max_step,
                         double brute_force_step,
                         typename OnePointStopCriterion<TFunction>::Ptr
                             one_point_stop_criterion_sample,
                         double stop_criteria_quorum = 1.0)
     : Solver<TFunction>(new NaiveIniter,
                         new PerPointUpdater<TFunction>(
                             new SteepestGradientUpdater<TFunction>(
                                 max_step,
                                 brute_force_step)),
                         new PerPointStopCriterion<TFunction>(
                             one_point_stop_criterion_sample,
                             stop_criteria_quorum)) { }

  ~SteepestGradientSolver() {}

  virtual string getDefaultAlias() { return "SteepestGradientSolver"; }
};

template<class TFunction>
class ConjugateGradientSolver : public Solver<TFunction> {
 public:
  typedef ltr::utility::shared_ptr<ConjugateGradientSolver> Ptr;  // NOLINT
  /**
   * Ctor for creating solver with
   *   -conjugate gradient descent updater max_step and brute_force_step
   *   -per point gradient magnitude stop criterion with min_gradient_magnitude
   *     and stop_criteria_quorum (by default, 1.0)
   *
   * \param max_step maximal step for conjugate gradient method
   * \param brute_force_step brute force step for conjugate gradient method
   * \param min_gradient_magnitude for gradient magnitude stop criterion
   * \param stop_criteria_quorum minimal part of true answers from per point stop criterion
   */
  ConjugateGradientSolver(double max_step,
                          double brute_force_step,
                          double min_gradient_magnitude,
                          double stop_criteria_quorum = 1.0)
     : Solver<TFunction>(new NaiveIniter,
                         new PerPointUpdater<TFunction>(
                             new ConjugateGradientUpdater<TFunction>(
                                 max_step,
                                 brute_force_step)),
                         new PerPointStopCriterion<TFunction>(
                             new GradientMagnitudeStopCriterion<TFunction>(
                                 min_gradient_magnitude),
                             stop_criteria_quorum)) { }
  /**
   * Ctor for creating solver with
   *   -conjugate gradient descent updater max_step and brute_force_step
   *   -stop_criterion
   *
   * \param max_step maximal step for conjugate gradient method
   * \param brute_force_step brute force step for conjugate gradient method
   * \param stop_criterion population stop criterion
   */
  ConjugateGradientSolver(double max_step,
                          double brute_force_step,
                          typename StopCriterion<TFunction>::Ptr stop_criterion)
      : Solver<TFunction>(new NaiveIniter,
                          new PerPointUpdater<TFunction>(
                              new ConjugateGradientUpdater<TFunction>(
                                  max_step,
                                  brute_force_step)),
                          stop_criterion) { }
  /**
   * Ctor for creating solver with
   *   -conjugate gradient descent updater max_step and brute_force_step
   *   -per point stop criterion with one_point_stop_criterion_sample and
   *      stop_criteria_quorum (by default, 1.0)
   *
   * \param max_step maximal step for conjugate gradient method
   * \param brute_force_step brute force step for conjugate gradient method
   * \param one_point_stop_criterion_sample one point stop criterion
   * sample for per point stop criterion
   * \param stop_criteria_quorum minimal part of true answers from per point stop criterion
   */
  ConjugateGradientSolver(double max_step,
                          double brute_force_step,
                          typename OnePointStopCriterion<TFunction>::Ptr
                              one_point_stop_criterion_sample,
                         double stop_criteria_quorum = 1.0)
     : Solver<TFunction>(new NaiveIniter,
                         new PerPointUpdater<TFunction>(
                             new ConjugateGradientUpdater<TFunction>(
                                 max_step,
                                 brute_force_step)),
                         new PerPointStopCriterion<TFunction>(
                             one_point_stop_criterion_sample,
                             stop_criteria_quorum)) { }

  ~ConjugateGradientSolver() {}

  virtual string getDefaultAlias() { return "ConjugateGradientSolver"; }
};
}

#endif  // LTR_OPTIMIZATION_SOLVER_SOLVERS_HPP_
