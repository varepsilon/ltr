// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_FUNCTION_H_

#include <vector>
#include <string>
#include "Eigen/Core"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/interfaces/aliaser.h"

namespace optimization {

typedef Eigen::VectorXd Point;
typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

/**
 * \class\brief abstract class for defining functions in \f$\mathbb{R}^n\f$
 */
class Function : public ltr::Aliaser {
 public:
  typedef ltr::utility::shared_ptr<Function> Ptr;

  explicit Function(int dimension);
  virtual ~Function() { }
  /** 
   * get dimension of the function domain
   */
  int dimension() const;

  /**
   * compute value of this function in a given point
   */
  virtual double computeValue(const Point& point) const = 0;
  /**
   * compute value of this function in a given point
   */
  double operator() (const Point& point) const {
    return computeValue(point);
  }
  virtual string getDefaultAlias() const {
    return "Function";
  }
 private:
  int dimension_;
};
}


#endif  // LTR_OPTIMIZATION_FUNCTIONS_FUNCTION_H_
