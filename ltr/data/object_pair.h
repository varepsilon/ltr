// Copyright 2011 Yandex

#ifndef LTR_DATA_OBJECT_PAIR_H_
#define LTR_DATA_OBJECT_PAIR_H_

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp> //NOLINT

#include "ltr/data/object.h"
#include "ltr/interfaces/printable.h"

namespace ltr {

/** \class The class stores a pair of two objects. The container for two
 *  objects, can be stored in DataSet. That would represent a pairwise data set.
 */
  class ObjectPair : public Printable {
  public:
  /** \typedef Shared pointer to an object pair.
   */
  typedef boost::shared_ptr<ObjectPair> Ptr;

  /** Default constructor creates a pair of two objects with no features.
   *
   */
  ObjectPair();
  /** Constructor creates a new pair of two copies of the two given objects.
   */
  ObjectPair(const Object& first, const Object& second);
  /** Creates a ObjectPair from copies of objects[0] and objects[1] from the
   * given vector.
   */
  explicit ObjectPair(const std::vector<Object>& objects);
  /** Returns a constant link to the first object in the pair.
   */
  const Object& first() const;
  /** Returns a link to the first object in the pair.
   */
  Object& first();
  /** Returns a constant link to the second object in the pair.
   */
  const Object& second() const;
  /** Returns a link to the second object in the pair.
   */
  Object& second();
  /** If i is less than 1 then the constant link to the first object in the pair
   * is returned otherwise the constant link to the second. This method is
   * needed to follow object container interface.
   */
  const Object& operator[](size_t i) const;
  /** If i is less than 1 then the link to the first object in the pair
   * is returned otherwise the link to the second. This method is
   * needed to follow object container interface.
   */
  Object& operator[](size_t i);
  /** If i is less than 1 then the constant link to the first object in the pair
   * is returned otherwise the constant link to the second. This method is
   * needed to follow object container interface.
   */
  const Object& at(size_t i) const;
  /** If i is less than 1 then the link to the first object in the pair
   * is returned otherwise the link to the second. This method is
   * needed to follow object container interface.
   */
  Object& at(size_t i);
  /*
   * Returns always 2. This method is needed to follow object container
   * interface.
   */
  size_t size() const;
  /**
   * Makes a deep copy of the pair, the new created copy consists of two copies
   * of the objects in the pair.
   */
  ObjectPair deepCopy() const;

  /** 
   * Function for serialization pair into string.
   */
  string toString() const;

  private:
  /** The first object in the pair.
   */
  Object first_;
  /** The second object in the pair.
   */
  Object second_;
};
/** Operator, checks whether two objects are equal.
 */
bool operator==(const ObjectPair& lhs, const ObjectPair& rhs);
/** Operator, checks whether two objects are not equal.
 */
bool operator!=(const ObjectPair& lhs, const ObjectPair& rhs);
}
#endif  // LTR_DATA_OBJECT_PAIR_H_
