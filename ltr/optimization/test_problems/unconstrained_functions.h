// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_TEST_PROBLEMS_UNCONSTRAINED_FUNCTIONS_H_
#define LTR_OPTIMIZATION_TEST_PROBLEMS_UNCONSTRAINED_FUNCTIONS_H_

#include <string>
#include "ltr/optimization/functions/twice_differentiable_function.h"
#include "ltr/utility/macros.h"

namespace optimization {

/**
 * \brief Class for Easom function in \f$\mathbb{R}^2\f$.
 *
 * \f$f(x, y) = - e^{-(x - \pi)^2} * e^{-(y - \pi)^2} * \cos(x) * \cos(y)\f$ <br>
 * \f$f(x, y) = - g(x) * g(y), \text{where } g(x) = e^{-(x - \pi)^2} * cos(x)\f$ <br>
 * Minimum at \f$ (x, y) = (\pi, \pi), \quad f(x, y) = -1\f$ <br>
 * Get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page1361.htm
 */
class EasomFunction : public TwiceDifferentiableFunction {
  ALLOW_SHARED_PTR_ONLY_CREATION(EasomFunction)
 public:
  explicit EasomFunction()
    : TwiceDifferentiableFunction(2) { }

  string getDefaultAlias() const { return "EasomFunction"; }
 private:
  /**
   * Implementation of computeValue method.
   */
  double computeValueImpl(const Point& point) const;
  /**
   * Implementation of computeGradient method.
   */
  void computeGradientImpl(const Point& point, Vector* gradient) const;
  /**
   * Implementation of computeHessian method.
   */
  void computeHessianImpl(const Point& point, Matrix* hessian) const;
  /**
   * Find \f$e^{-(x - \pi)^2}\f$.
   */
  double squareExp(double variable) const;
  /**
   * Find \f$g(x)\f$.
   */
  double partValue(double variable) const;
  /**
   * Find \f$\frac{dg(x)}{dx}\f$.
   */
  double partGradient(double variable) const;
  /**
   * Find \f$\frac{d^2g(x)}{dx^2}\f$.
   */
  double partHessian(double variable) const;
};

/**
 * \brief Class for Griewank function in \f$\mathbb{R}^n\f$.
 *
 * \f$f(x) = \sum_{i = 1}^n x_i^2/4000 - \prod_{i = 1} ^ n \cos(x_i / \sqrt(i))\f$ <br>
 * Minimum at \f$ x = (0, \dots, 0), \quad f(x) = 0 \f$ <br>
 * Get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page1905.htm
 */
class GriewankFunction : public TwiceDifferentiableFunction {
  ALLOW_SHARED_PTR_ONLY_CREATION(GriewankFunction)
 public:
  explicit GriewankFunction(int dimension)
    : TwiceDifferentiableFunction(dimension) { }

  string getDefaultAlias() const { return "GriewankFunction"; }
 private:
  /**
   * Implementation of computeValue method.
   */
  double computeValueImpl(const Point& point) const;
  /**
   * Implementation of computeGradient method.
   */
  void computeGradientImpl(const Point& point, Vector* gradient) const;
  /**
   * Implementation of computeHessian method.
   */
  void computeHessianImpl(const Point& point, Matrix* hessian) const;
  /**
   * Compute second part deriviate of function.
   */
  double computePartHessian(int first_coordinate,
                            int second_coordinate,
                            const Point& point) const;
};

/**
 * \brief Class for Rosenbrock function in \f$\mathbb{R}^n\f$.
 *
 * \f$f(x) = \sum_{i = 1}^{n - 1} (100 * (x_i^2 - x_{i + 1}) + (x_i - 1)^2)\f$ <br>
 * Minimum at \f$ x = (1, \dots, 1), \quad f(x) = 0 \f$ <br>
 * Get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page2537.htm
 */
class RosenbrockFunction : public TwiceDifferentiableFunction {
  ALLOW_SHARED_PTR_ONLY_CREATION(RosenbrockFunction)
 public:
  explicit RosenbrockFunction(int dimension);

  string getDefaultAlias() const { return "RosenbrockFunction"; }
 private:
  /**
   * Implementation of computeValue method.
   */
  double computeValueImpl(const Point& point) const;
  /**
   * Implementation of computeGradient method.
   */
  void computeGradientImpl(const Point& point, Vector* gradient) const;
  /**
   * Implementation of computeHessian method.
   */
  void computeHessianImpl(const Point& point, Matrix* hessian) const;
};

/**
 * \brief Class for Sum Squares function in \f$\mathbb{R}^n\f$.
 *
 * \f$f(x) = \sum_{i = 1}^{n} i * x_i^2 \f$ <br>
 * Minimum at \f$ x = (0, \dots, 0), \quad f(x) = 0 \f$. <br>
 * Get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page674.htm.
 */
class SumSquaresFunction : public TwiceDifferentiableFunction {
  ALLOW_SHARED_PTR_ONLY_CREATION(SumSquaresFunction)
 public:
  explicit SumSquaresFunction(int dimension)
    : TwiceDifferentiableFunction(dimension) { }

  string getDefaultAlias() const { return "SumSquaresFunction"; }
 private:
  /**
   * Implementation of computeValue method.
   */
  double computeValueImpl(const Point& point) const;
  /**
   * Implementation of computeGradient method.
   */
  void computeGradientImpl(const Point& point, Vector* gradient) const;
  /**
   * Implementation of computeHessian method.
   */
  void computeHessianImpl(const Point& point, Matrix* hessian) const;
};

/** \brief Class for Trid function in \f$\mathbb{R}^n\f$.
 *
 * \f$f(x) = \sum_{i = 1}^{n} (x_i - 1)^2 - \sum_{i = 2}^n x_i*x_{i-1}\f$ <br>
 * Get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page2904.htm
 */
class TridFunction : public TwiceDifferentiableFunction {
  ALLOW_SHARED_PTR_ONLY_CREATION(TridFunction)
 public:
  explicit TridFunction(int dimension);

  string getDefaultAlias() const { return "TridFunction"; }
 private:
  /**
   * Implementation of computeValue method.
   */
  double computeValueImpl(const Point& point) const;
  /**
   * Implementation of computeGradient method.
   */
  void computeGradientImpl(const Point& point, Vector* gradient) const;
  /**
   * Implementation of computeHessian method.
   */
  void computeHessianImpl(const Point& point, Matrix* hessian) const;
};

/**
 * \brief Class for Rastrigin function in \f$\mathbb{R}^n\f$.
 *
 * \f$f(x) = 10 n + \sum_{i=1}^n \left(x_i^2 - 10\cos(2 \pi x_i)\right)\f$ <br>
 * Minimum at \f$ x = (0, \dots, 0), \quad f(x) = 0 \f$. <br>
 * Get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page2607.htm
 */
class RastriginFunction : public TwiceDifferentiableFunction {
  ALLOW_SHARED_PTR_ONLY_CREATION(RastriginFunction);
 public:
  explicit RastriginFunction(int dimension)
    : TwiceDifferentiableFunction(dimension) {}
  string getDefaultAlias() const { return "RastriginFunction"; }
 private:
  /**
    * Implementation of computeValue method.
    */
  double computeValueImpl(const Point& point) const;
  /**
    * Implementation of computeGradient method.
    */
  void computeGradientImpl(const Point& point, Vector* gradient) const;
  /**
    * Implementation of computeHessian method.
    */
  void computeHessianImpl(const Point& point, Matrix* hessian) const;
};
}

#endif  // LTR_OPTIMIZATION_TEST_PROBLEMS_UNCONSTRAINED_FUNCTIONS_H_
