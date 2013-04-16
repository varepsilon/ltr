// Copyright 2012 Yandex

#ifndef LTR_DATA_OBJECT_H_
#define LTR_DATA_OBJECT_H_

#include <map>
#include <ostream> //NOLINT
#include <stdexcept>
#include <string>
#include <vector>

#include "ltr/interfaces/printable.h"

#include "ltr/utility/boost/shared_ptr.h"

#include "ltr/data/object_representation.h"

using std::map;
using std::string;
using std::vector;

using ltr::utility::shared_ptr;

namespace ltr {
/**
 * Type for feature vector.
 */
typedef vector<double> Features;
/**
 * Type for pointer to feature vector.
 */
typedef ltr::utility::shared_ptr<Features> FeaturesPtr;
/**
 * \brief Base class for storing information in a Dataset. An object consist of
 * feature vector and meta information.
 */
class Object : public Printable {
 public:
  /**
   * Shared pointer to an object.
   */
  typedef shared_ptr<Object> Ptr;
  /**
   * Default constructor, creates an object with empty feature vector.
   */
  Object();
  /**
   * Default constructor, creates an deep copy of an object.
   */
  Object(const Object& object);
  /** Construct object from InnerRepresentation
   */
  Object(InnerRepresentation::Ptr presentation,
         ElementBounds bounds);
  Object(InnerRepresentation::Ptr presentation,
         int object_index);
  /**
   * Returns constant link to the feature vector of an object.
   */
  const string& getMetaInfo(const string& name) const;
  /**
   * Sets a meta information with given name.
   */
  void setMetaInfo(const string& name, const string& value);
  /**
   * Append a feature to the feature vector of the object.
   */
  Object& operator<<(double feature);

  // \TODO(sameg) Don't like the next 3 operators

  /**
   * Returns constant link to feature at index i.
   */
  const double& operator[](int feature_index) const;
  /**
   * Returns link to feature at index i.
   */
  double& operator[](int feature_index);
  /**
   * Returns constant link to feature at index i.
   */
  const double& at(int feature_index) const;
  /**
   * Returns link to feature at index i.
   */
  double& at(int feature_index);
  /**
   * Returns link to Eigen vector of features
   */
  EigenColumn eigen_features();
  /**
   * Returns constant link to Eigen row
   */
  const EigenColumn eigen_features() const;
  void set_eigen_features(const VectorXd& row);
  /**
   * Heavy operator=. Copies data from one object to another.
   * To copy only references, use lightCopy
   */
  Object& operator=(const Object& other);
  /**
   * Clear object features and FeaturesInfo
  */
  void clear();
  /**
   * Returns the number of features in the object.
   */
  int feature_count() const;
  /**
   * Resizes feature vector
   */
  void set_feature_count(int feature_count);
  /**
   * Returns actual (that means, the label was read from input file or any
   * other way is known) scoring label of the object.
   */
  double actual_label() const;
  /**
   * Returns predicted (that means, the label set by a scoring algorithm)
   * label of the object.
   */
  double predicted_label() const;
  /**
   * Sets actual (that means, the label was read from input file or any
   * other way is known) scoring label of the object.
   */
  void set_actual_label(double label);
  /**
   * Sets predicted (that means, the label set by a scoring algorithm)
   * label of the object.
   */
  void set_predicted_label(double label) const;
  /**
   * Creates a deep copy of the object, this copies the object resources and
   * creates a new object with these copied resources. The new object is
   * returned.
   */
  Object deepCopy() const;
  /**
   * Makes reference copy of given object - if one object changes,
   * other changes too.
   */
  void lightCopyFrom(const Object& obj);
  /**
   * Friend operator, checks whether two objects are equal.
   */
  friend bool operator==(const Object& lhs, const Object& rhs);
  /**
   * Function for serialization of object.
   * Returns string of feature values in brackets. For example: [1, 5.6, 2.3]
   */
  virtual string toString() const;

 private:
  ObjectRepresentation::Ptr presentation_;

  template<typename TElement>
  friend class DataSet;
};
/**
 * Operator, checks whether two objects are equal.
 */
bool operator==(const Object& lhs, const Object& rhs);
/**
 * Operator, checks whether two objects are not equal.
 */
bool operator!=(const Object& lhs, const Object& rhs);
};
#endif  // LTR_DATA_OBJECT_H_
