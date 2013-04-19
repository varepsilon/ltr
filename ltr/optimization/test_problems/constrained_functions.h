// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_TEST_PROBLEMS_CONSTRAINED_FUNCTIONS_H_
#define LTR_OPTIMIZATION_TEST_PROBLEMS_CONSTRAINED_FUNCTIONS_H_

#include <string>
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/optimization/functions/twice_differentiable_function.h"

namespace optimization {
/**
 * \brief Class for G1 function in \f$\mathbb{R}^13\f$.
 *
 * \f$f(x) = 5 * \sum_{i = 1} ^4 (x_i - x_i^2) - sum_{i = 5}^13 x_i\f$.
 *
 * Get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page506.htm
 */
class G1Function : public TwiceDifferentiableFunction {
 public:
  explicit G1Function()
    : TwiceDifferentiableFunction(13) { }
  ~G1Function() { }
  typedef ltr::utility::shared_ptr<G1Function> Ptr;

  string getDefaultAlias() const { return "G1Function"; }
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
 * \brief Class for G3 function in \f$\mathbb{R}^n\f$.
 *
 * \f$f(x) = (\sqrt{n})^n \prod_{i = 1}^n x_i\f$.
 *
 * Get more info on http://www-optima.amp.i.kyoto-u.ac.jp/member/student/hedar/Hedar_files/TestGO_files/Page2613.htm.
 */
class G3Function : public TwiceDifferentiableFunction {
 public:
  explicit G3Function(int dimension)
    : TwiceDifferentiableFunction(dimension) {}
  ~G3Function() {}
  typedef ltr::utility::shared_ptr<G3Function> Ptr;

  string getDefaultAlias() const { return "G3Function"; }
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
}
#endif  // LTR_OPTIMIZATION_TEST_PROBLEMS_CONSTRAINED_FUNCTIONS_H_
