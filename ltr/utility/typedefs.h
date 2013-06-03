// Copyright 2013 Yandex

#ifndef LTR_UTILITY_TYPEDEFS_H_
#define LTR_UTILITY_TYPEDEFS_H_

#include <map>
#include <utility>
#include <functional>

#include "contrib/include_Eigen.h"

using std::map;
using Eigen::aligned_allocator;
using Eigen::VectorXd;
using Eigen::MatrixXd;

namespace ltr {
namespace utility {
/**
 * \brief maps double to VectorXd.
 */
typedef map<double, VectorXd, std::less<double>,
            aligned_allocator<std::pair<double, VectorXd> > > doubleVectorXdMap;
/**
 * \brief maps double to MatrixXd.
 */
typedef map<double, MatrixXd, std::less<double>,
            aligned_allocator<std::pair<double, MatrixXd> > > doubleMatrixXdMap;
}
}


#endif  // LTR_UTILITY_TYPEDEFS_H_
