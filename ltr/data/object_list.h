// Copyright 2012 Yandex

#ifndef LTR_DATA_OBJECT_LIST_H_
#define LTR_DATA_OBJECT_LIST_H_

#include <stdexcept>
#include <string>
#include <vector>

#include "ltr/data/object.h"

#include "ltr/interfaces/printable.h"

#include "ltr/utility/boost/shared_ptr.h"

using std::vector;

namespace ltr {
/** \class ObjectList The class stores a number of objects. The container for number of
 *  objects, can be stored in DataSet. That would represent a listwise data set.
 */
  class ObjectList : public Printable {
  public:
  /** \typedef Shared pointer to an object list.
   */
  typedef ltr::utility::shared_ptr< ObjectList > Ptr;
  /** Default constructor creates an empty list of objects.
   */
  ObjectList();
  /** The constructor creates an list of objects from the copies of objects in
   *  the given vector.
   */
  explicit ObjectList(const vector<Object>& objects);
  /** Returns a constant link to the ith object in the object list.
   */
  const Object& operator[](int i) const;
  /** Returns a link to the ith object in the object list.
   */
  Object& operator[](int i);
  /** Returns a constant link to the ith object in the object list.
   */
  const Object& at(int i) const;
  /** Returns a link to the ith object in the object list.
   */
  Object& at(int i);
  /** Adds a deep copy of the given object to the object list.
   */
  ObjectList& operator<<(const Object& object);
  /** Adds a deep copy of the given object to the object list.
   */
  void add(const Object& object);
  /** Returns the number of objects in the object list.
   */
  int size() const;
  /** Removes all object from the object list.
   */
  void clear();
  /** Creates a new object list cantaining deep copies of the objects in the
   * list.
   */
  ObjectList deepCopy() const;
  /** Operator, checks whether two objects are equal.
   */
  friend bool operator==(const ObjectList& lhs, const ObjectList& rhs);
  /**
   * Function for serialization list into string.
   */
  string toString() const;

  private:
  /** Shared pointer to the vector of Object s.
   */
  ltr::utility::shared_ptr<vector<Object> > objects_;
};
/** Operator, checks whether two object_lists are equal.
 */
bool operator==(const Object& lhs, const Object& rhs);
/** Operator, checks whether two object_lists are not equal.
 */
bool operator!=(const Object& lhs, const Object& rhs);
};
#endif  // LTR_DATA_OBJECT_LIST_H_
