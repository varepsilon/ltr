// Copyright 2012 Yandex
#include "ltr/optimization/functions/test_functions/unconstrained/easom_function/easom_function.h"
#include "ltr/utility/macros.h"

namespace optimization {
const double PI = 3.141592653589797;
double EasomFunction::squareExp(double variable) const {
  double square_exp = exp(-pow(variable - PI, 2));

  return square_exp;
}
double EasomFunction::partValue(double variable) const {
  double part_value = squareExp(variable) * cos(variable);

  return part_value;
}
double EasomFunction::partGradient(double variable) const {
  double part_gradient = squareExp(variable) *
    (-2.0 * (variable - PI) * cos(variable) - sin(variable));

  return part_gradient;
}
double EasomFunction::partHessian(double variable) const {
  double part_hessian = -2.0 * (variable - PI) * partGradient(variable) +
    squareExp(variable) * (-3.0 * cos(variable) +
    sin(variable) * 2.0 * (variable - PI));

  return part_hessian;
}
double EasomFunction::value(const Point& point) const {
  CHECK(point.size() == dimension());

  double value = -partValue(point(0)) * partValue(point(1));

  return value;
}
Vector EasomFunction::gradient(const Point& point) const {
  CHECK(point.size() == dimension());

  Vector gradient(2);
  gradient(0) = -partGradient(point(0)) * partValue(point(1));
  gradient(1) = -partGradient(point(1)) * partValue(point(0));

  return gradient;
}
Matrix EasomFunction::hessian(const Point& point) const {
  CHECK(point.size() == dimension());

  Matrix hessian(2, 2);
  hessian(0, 0) = - partHessian(point(0)) * partValue(point(1));
  hessian(1, 1) = - partHessian(point(1)) * partValue(point(0));
  hessian(0, 1) = - partGradient(point(0)) * partGradient(point(1));
  hessian(1, 0) = hessian(0, 1);

  return hessian;
}
}
