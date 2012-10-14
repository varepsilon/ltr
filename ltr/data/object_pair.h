// Copyright 2012 Yandex

#ifndef LTR_DATA_OBJECT_PAIR_H_
#define LTR_DATA_OBJECT_PAIR_H_

#include <stdexcept>
#include <string>
#include <vector>

#include "ltr/data/object.h"

#include "ltr/interfaces/printable.h"

#include "ltr/utility/shared_ptr.h"

namespace ltr {
/** \brief The class stores a pair of two objects. The container for two
 *  objects, can be stored in DataSet. That would represent a pairwise data set.
 */
  class ObjectPair : public Printable {
  public:
  /** \typedef Shared pointer to an object pair.
   */
  typedef ltr::utility::shared_ptr<ObjectPair> Ptr;
  /** Default constructor creates a pair of two objects with no features.
   *
   */
  ObjectPair();
  /** Constructor creates a new pair of two copies of the two given objects.
   */
  ObjectPair(const Object& first, const Object& second);
  /**
   * Makes a deep copy of the pair, the new created copy consists of two copies
   * of the objects in the pair.
   */
  ObjectPair deepCopy() const;
  /** 
   * Function for serialization pair into string.
   */
  string toString() const;
  /** The first object in the pair.
   */
  Object first;
  /** The second object in the pair.
   */
  Object second;
};
/** Operator, checks whether two objects are equal.
 */
bool operator==(const ObjectPair& lhs, const ObjectPair& rhs);
/** Operator, checks whether two objects are not equal.
 */
bool operator!=(const ObjectPair& lhs, const ObjectPair& rhs);
};
#endif  // LTR_DATA_OBJECT_PAIR_H_
