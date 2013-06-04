// Copyright 2013 Yandex

#ifndef LTR_DATA_INNER_REPRESENTATION_H_
#define LTR_DATA_INNER_REPRESENTATION_H_

#include <map>
#include <vector>
#include <string>
#include "contrib/include_Eigen.h"
#include "ltr/utility/macros.h"
#include "ltr/utility/boost/shared_ptr.h"

namespace ltr {

using std::map;
using std::vector;
using std::string;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::RowVectorXd;
/**
 * Type for meta information. Just map from info field name to info field value.
 */
typedef map<string, string> MetaInfo;
typedef ltr::utility::shared_ptr<MetaInfo> MetaInfoPtr;
typedef Eigen::Block<Eigen::MatrixXd, -1, 1, true, true> EigenColumn;

/**
 * \brief Structure for storing matrix of features and accompanying vectors.
 */
class InnerRepresentation {
 public:
  /**
   * Shared pointer to presentation
   */
  typedef ltr::utility::shared_ptr<InnerRepresentation> Ptr;

  /**
   * Default constructor, creates matrix of _size_ vectors with
   * _features_ number of features
   */
  InnerRepresentation(int size, int features);

  /**
   * Returns feature vector for given index
   */
  EigenColumn get_features_column(int index);
  /**
   * Sets feature vector for given index
   */
  void set_features_column(int index, const VectorXd& column);
  /**
   * Const access to elements of feature matrix
   */
  const double& feature(int row, int column) const;
  /**
   * Access to elements of feature matrix
   */
  double& feature(int row, int column);

  GET(VectorXd, predicted_labels);
  void set_predicted_label(int index, double value);

  GET(VectorXd, actual_labels);
  void set_actual_label(int index, double value);

  /**
   * Meta information for given index
   */
  MetaInfoPtr meta_info(int index) const;
  /**
   * Meta information setter
   */
  void set_meta_info(int index, MetaInfoPtr ptr);

  /**
   * Get number of columns in the matrix columns represent vectors
   */
  int column_count() const { return features_.cols(); }
  /**
   * Get number of rows in the matrix rows represent features
   */
  int row_count() const { return features_.rows(); }

  /**
   * Set number of objects
   */
  void set_column_count(int cols);
  /**
   * Set number of features of each object
   */
  void set_row_count(int rows);
  /**
   * Resize matrix and accompanying vectors
   */
  void resize(int rows, int columns);

  /**
   * Add several new columns to the matrix and appropriate vectors
   */
  void add_several_objects(int num_objects);
 private:
  /**
   * Check whether index is in appropriate range
   */
  void checkIndex(int row, int column) const;
  /**
   * Check whether index is in appropriate range
   */
  void checkIndex(int index) const;

  MatrixXd features_;
  VectorXd predicted_labels_;
  VectorXd actual_labels_;
  vector<MetaInfoPtr> meta_info_;
};

/**
 * \brief Structure that keeps start and end lines of element
 * (object, pair, list) in the presentation.
 */
struct ElementBounds {
 public:
  explicit ElementBounds(int start = 0, int end = 0);

  int start_line_index;
  int end_line_index;
};
}
#endif  // LTR_DATA_INNER_REPRESENTATION_H_
