// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_SERIALIZABLE_H_
#define LTR_INTERFACES_SERIALIZABLE_H_

#include "ltr/utility/shared_ptr.h"
#include <boost/lexical_cast.hpp>

#include <string>
#include <iostream>

using std::string;

namespace ltr {
/** Counter of unique serializable objects.
 */
extern size_t serializable_objects_counter_;

/** \class The interface class for objects, that can be serialized. That means
 * that a cpp code for those can be created.
 */
class Serializable {
  public:
  /** \typedef boost shared pointer to the Functor
   */
  typedef ltr::utility::shared_ptr<Serializable> Ptr;
  /** Default constructor. Counts all serializable objects and assigns them
   * unique id.
   */
  Serializable() {
    id_ = serializable_objects_counter_++;
  }
  /** Copy constructor. Counts all serializable objects and assigns them
   * unique id.
   */
  Serializable(const Serializable&) {
    id_ = serializable_objects_counter_++;
  }
  /** The function creates cpp code for serializable object using given name.
   * \param serrializable_object_name name that should be used for naming the
   * object (function or class) within the generated cpp code.
   */
  virtual string generateCppCode(const string& serrializable_object_name)
  const = 0;
  /** The function creates cpp code for serializable object using default name.
   */
  string generateCppCode() const {
    return this->generateCppCode(this->getDefaultSerializableObjectName());
  }
  /** Returns unique id of the serializable object.
   */
  size_t getId() const {return id_;}
  /** The function returns a string with default name for the object.
   */
  string getDefaultSerializableObjectName() const {
    return "serializable_object_" + boost::lexical_cast<string>(this->getId());
  }

  virtual ~Serializable() {}

  private:
  /** The unique id of the serializable object.
   */
  size_t id_;
};
}
#endif  // LTR_INTERFACES_SERIALIZABLE_H_
