// Copyright 2012 Yandex

#ifndef LTR_UTILITY_EIGEN_CONVERTERS_H_
#define LTR_UTILITY_EIGEN_CONVERTERS_H_

#include <vector>
#include <utility>
#include <map>
#include <functional>

#include "contrib/include_Eigen.h"

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"
#include "ltr/data/data_set.h"
#include "ltr/data/utility/data_set_statistics.h"

using std::vector;
using std::map;

using Eigen::VectorXd;
using Eigen::MatrixXd;
using Eigen::aligned_allocator;

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

/**
 * \brief Converts std::vector to Eigen::VectorXd.
 * \param std_vector input std::vector.
 */
VectorXd StdVectorToEigenVector(const vector<double>& std_vector);

/**
 * \brief Converts Eigen::VectorXd to ltr::Object.
 * \param eigen_vector input Eigen vector.
 * \param std_vector output std::vector.
 */
void EigenVectorToStdVector(const VectorXd& eigen_vector,
                            vector<double>* std_vector);

/**
 * \brief Converts ltr::DataSet to Eigen::MatrixXd.
 * \param source source dataset.
 */
template <class TElement>
MatrixXd DataSetToEigenMatrix(const DataSet<TElement>& source) {
  int rows_count = getDataSetObjectCount(source);
  int cols_count = source.feature_info().feature_count();
  MatrixXd result(rows_count, cols_count);

  int row_index = 0;
  for (int element_index = 0; element_index < source.size(); ++element_index) {
    PerObjectAccessor<const TElement> object_accessor(&source[element_index]);
    for (int object_index = 0;
         object_index < object_accessor.object_count();
         ++object_index) {
      result.row(row_index) =
        StdVectorToEigenVector(object_accessor.object(object_index).features());
      ++row_index;
    }
  }
  return result;
}

/**
 * \brief Initializes every element of Eigen::MatrixXd equal to zero.
 */
void InitEigenMatrix(MatrixXd* eigen_matrix);

/**
 * \brief Initializes every element of Eigen::VectorXd equal to zero.
 */
void InitEigenVector(VectorXd* eigen_vector);
};
};

#endif  // LTR_UTILITY_EIGEN_CONVERTERS_H_
