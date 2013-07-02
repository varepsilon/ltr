// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_SOLVER_SOLUTION_H_
#define LTR_OPTIMIZATION_SOLVER_SOLUTION_H_

#include "contrib/include_Eigen.h"

namespace optimization {
typedef Eigen::VectorXd Point;

/**
 * \brief This class contains information about a solution of
          the optimization task: a minumim value found, an argument
          where the minumum is reached, numer of iterations and time
          used to compute the answer.
*/
class Solution {
  public:
    Solution() { }
    Solution(double min_value, Point argmin, int num_iter, int time) :
      min_value(min_value), argmin(argmin), num_iter(num_iter), time(time)
        {  }
  public:
    /**
     * Minimum value of the target function
    */
    double min_value;
    /**
     * The argument of the target function where min_value is reached
    */
    Point argmin;
    /**
     * The number of iterations needed to compute min_value
    */
    int num_iter;
    /**
     * Time needed to compute min_value, measured in msec
    */
    int time;
};
}

#endif  // LTR_OPTIMIZATION_SOLVER_SOLUTION_H_

