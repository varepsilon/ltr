// Copyright 2013 Yandex

#include "ltr/utility/macros.h"
#include "ltr/optimization/test_problems/unconstrained_functions.h"

namespace optimization {
const double PI = 3.141592653589797;
double EasomFunction::squareExp(double variable) const {
  return exp(-pow(variable - PI, 2));
}
double EasomFunction::partValue(double variable) const {
  return squareExp(variable) * cos(variable);
}
double EasomFunction::partGradient(double variable) const {
  return squareExp(variable) *
    (-2.0 * (variable - PI) * cos(variable) - sin(variable));
}
double EasomFunction::partHessian(double variable) const {
  return -2.0 * (variable - PI) * partGradient(variable) +
    squareExp(variable) * (-3.0 * cos(variable) +
    sin(variable) * 2.0 * (variable - PI));
}
double EasomFunction::computeValueImpl(const Point& point) const {
  return -partValue(point(0)) * partValue(point(1));
}
void EasomFunction::computeGradientImpl(const Point& point,
                                    Vector* gradient) const {
  (*gradient)(0) = -partGradient(point(0)) * partValue(point(1));
  (*gradient)(1) = -partGradient(point(1)) * partValue(point(0));
}
void EasomFunction::computeHessianImpl(const Point& point,
                                       Matrix* hessian) const {
  (*hessian)(0, 0) = - partHessian(point(0)) * partValue(point(1));
  (*hessian)(1, 1) = - partHessian(point(1)) * partValue(point(0));
  (*hessian)(0, 1) = - partGradient(point(0)) * partGradient(point(1));
  (*hessian)(1, 0) = (*hessian)(0, 1);
}

double GriewankFunction::computeValueImpl(const Point& point) const {
  /**
   * f(x) = sum/4000 + production
   */
  double sum = 0.0;
  double production = 1.0;

  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    sum += pow(point(coordinate), 2);
    production *= cos(point(coordinate) / pow(coordinate + 1, 0.5));
  }

  return sum / 4000.0 - production + 1.0;
}
void GriewankFunction::computeGradientImpl(const Point& point,
                                       Vector* gradient) const {
  double production = 1.0;

  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    (*gradient)(coordinate) = point(coordinate) / 2000.0;
    production = 1.0;

    for (int i = 0; i < dimension(); ++i) {
      if (i != coordinate) {
        production *= cos(point(i) / pow(i + 1, 0.5));
      }
    }

    production *= sin(point(coordinate) / pow(coordinate + 1, 0.5)) /
      pow(coordinate + 1, 0.5);
    (*gradient)(coordinate) += production;
  }
}
void GriewankFunction::computeHessianImpl(const Point& point,
                                      Matrix* hessian) const {
  for (int first_coordinate = 0;
       first_coordinate < dimension();
       ++first_coordinate) {
    for (int second_coordinate = 0;
         second_coordinate < dimension();
         ++second_coordinate) {
      (*hessian)(first_coordinate, second_coordinate) =
        computePartHessian(first_coordinate, second_coordinate, point);
    }
  }
}
double GriewankFunction::computePartHessian(int first_coordinate,
                                            int second_coordinate,
                                            const Point& point) const {
  double part_hessian = 0.0;
  double production = 1.0;
  if (first_coordinate != second_coordinate) {
    for (int i = 0; i < dimension(); ++i) {
      if ((i != first_coordinate) && (i != second_coordinate)) {
        production *= cos(point(i) / pow(i + 1, 0.5));
      }
    }
    part_hessian = production * sin(point(first_coordinate) /
      pow(first_coordinate + 1, 0.5)) * sin(point(second_coordinate) /
      pow(second_coordinate + 1, 0.5)) / (pow(first_coordinate + 1, 0.5) *
      pow(second_coordinate + 1, 0.5));
  } else {  //  for diag elements of hessian
    part_hessian = 1.0 / 2000.0;
    for (int i = 0; i < dimension(); ++i)  {
      production *= cos(point(i) / pow(i + 1, 0.5));
    }
    part_hessian += production / (first_coordinate + 1);
  }
  return part_hessian;
}

RosenbrockFunction::RosenbrockFunction(int dimension)
    : TwiceDifferentiableFunction(dimension) {
  CHECK(dimension > 1);
}
double RosenbrockFunction::computeValueImpl(const Point& point) const {
  double value = 0.0;
  for (int coordinate = 1; coordinate < dimension(); ++coordinate) {
    value += (100.0 * pow(pow(point(coordinate - 1), 2) -
      point(coordinate), 2) + pow(point(coordinate - 1) - 1, 2));
  }

  return value;
}
void RosenbrockFunction::computeGradientImpl(const Point& point,
                                         Vector* gradient) const {
  (*gradient)(0) = 400.0 * (pow(point(0), 2) - point(1)) * point(0) +
    2.0 * (point(0) - 1.0);
  (*gradient)(dimension() - 1) = -200.0 * (pow(point(dimension() - 2), 2) -
    point(dimension() - 1));

  for (int coordinate = 1; coordinate < dimension() - 1; ++coordinate) {
    (*gradient)(coordinate) =
      -200.0 * (pow(point(coordinate - 1), 2) - point(coordinate)) +
      400.0 * (pow(point(coordinate), 2) - point(coordinate + 1)) *
      point(coordinate) + 2.0 * (point(coordinate) - 1.0);
  }
}
void RosenbrockFunction::computeHessianImpl(const Point& point,
                                         Matrix* hessian) const {
  (*hessian)(0, 0) = 1200.0 * pow(point(0), 2) - 400.0 * point(1) + 2.0;
  (*hessian)(0, 1) = -400.0 * point(0);
  (*hessian)(1, 0) = (*hessian)(0, 1);

  (*hessian)(dimension() - 1, dimension() - 1) = 200.0;

  for (int coordinate = 1; coordinate < dimension() - 1; ++coordinate) {
    /**
     * set diag elements
     */
    (*hessian)(coordinate, coordinate) = 200.0 + 1200.0 *
      pow(point(coordinate), 2) - 400.0 * point(coordinate + 1) + 2.0;
    /**
     * set above diag elements
     */
    (*hessian)(coordinate, coordinate + 1) = -400.0 * point(coordinate);
     /**
     * set under diag elements
     */
    (*hessian)(coordinate + 1, coordinate) = (*hessian)(coordinate,
                                                        coordinate + 1);
  }
}

double SumSquaresFunction::computeValueImpl(const Point& point) const {
  double value = 0.0;

  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    value += (coordinate + 1.0) * pow(point(coordinate), 2);
  }

  return value;
}
void SumSquaresFunction::computeGradientImpl(const Point& point,
                                         Vector* gradient) const {
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    (*gradient)(coordinate) = 2.0 * (coordinate + 1.0) * point(coordinate);
  }
}
void SumSquaresFunction::computeHessianImpl(const Point& point,
                                        Matrix* hessian) const {
  /**
   * compute diag elements of hessian
   */
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    (*hessian)(coordinate, coordinate) = 2.0 * (coordinate + 1.0);
  }
}

TridFunction::TridFunction(int dimension)
    : TwiceDifferentiableFunction(dimension) {
  CHECK(dimension > 1);
}
double TridFunction::computeValueImpl(const Point& point) const {
  /**
   * f(x) = sum_squares - prod
   */
  double sum_squares = 0.0;
  double sum_prod = 0.0;

  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    sum_squares += pow(point(coordinate) - 1.0, 2);
  }

  for (int coordinate = 1; coordinate < dimension(); ++coordinate) {
    sum_prod += point(coordinate) * point(coordinate - 1);
  }

  return sum_squares - sum_prod;
}
void TridFunction::computeGradientImpl(const Point& point,
                                   Vector* gradient) const {
  (*gradient)(0) = 2.0 * point(0) - 2.0 - point(1);
  (*gradient)(dimension() - 1) = 2.0 * point(dimension() - 1) -
    2.0 - point(dimension() - 2);

  for (int coordinate = 1; coordinate + 1 < dimension(); ++coordinate) {
    (*gradient)(coordinate) = 2.0 * point(coordinate) - 2.0 -
      point(coordinate - 1) - point(coordinate + 1);
  }
}
void TridFunction::computeHessianImpl(const Point& point,
                                  Matrix* hessian) const {
  /**
   * compute diag elements of hessian
   */
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    (*hessian)(coordinate, coordinate) = 2.0;
  }

  /**
   * compute another elements
   */
  for (int coordinate = 0; coordinate + 1 < dimension(); ++coordinate) {
    (*hessian)(coordinate, coordinate + 1) = -1.0;
    (*hessian)(coordinate + 1, coordinate) = -1.0;
  }
}

double RastriginFunction::computeValueImpl(const Point& point) const {
  double res = 10.0 * dimension();
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    res += pow(point(coordinate), 2) - 10.0 * cos(2.0 * PI * point(coordinate));
  }
  return res;
}

void RastriginFunction::computeGradientImpl(const Point& point,
                                            Vector* gradient) const {
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    (*gradient)(coordinate) = 2.0 * point(coordinate) +
                              20.0 * PI * sin(2.0 * PI * point(coordinate));
  }
}

void RastriginFunction::computeHessianImpl(const Point& point,
                                           Matrix* hessian) const {
  for (int first_coordinate = 0;
       first_coordinate < dimension();
       ++first_coordinate) {
    for (int second_coordinate = 0;
         second_coordinate < dimension();
         ++second_coordinate) {
      if (first_coordinate == second_coordinate) {
        (*hessian)(first_coordinate, second_coordinate) = 2.0 +
            40.0 * pow(PI, 2) * cos(2.0 * PI * point(first_coordinate));
      } else {
        (*hessian)(first_coordinate, second_coordinate) = 0.0;
      }
    }
  }
}
}
