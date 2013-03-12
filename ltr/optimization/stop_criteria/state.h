// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_STATE_H_
#define LTR_OPTIMIZATION_STOP_CRITERIA_STATE_H_

#include "contrib/include_Eigen.h"

#include "ltr/utility/macros.h"

namespace optimization {

typedef Eigen::VectorXd Point;
typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

class State {
 public:
  explicit State(Point point = Point(),
                 int iteration = 0,
                 double value = 0.0,
                 Vector gradient = Vector(),
                 Matrix hessian = Matrix())
                 : point_(point),
                   iteration_(iteration),
                   gradient_(gradient),
                   value_(value),
                   hessian_(hessian) { }
  ~State() { }

  GET_SET(Point, point)
  GET_SET(int, iteration)
  GET_SET(double, value)
  GET_SET(Vector, gradient)
  GET_SET(Matrix, hessian)

 private:
  Point point_;
  int iteration_;
  double value_;
  Vector gradient_;
  Matrix hessian_;
};
}

#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_STATE_H_
