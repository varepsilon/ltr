// Copyright 2011 Yandex

#ifndef LTR_UTILITY_SERIALIZABLE_H_
#define LTR_UTILITY_SERIALIZABLE_H_

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <string>

using std::string;

namespace ltr {
/** Counter of unique serializable objects.
 */
extern size_t serializable_objects_counter_;

/** \class The interface class for objects, that can be serialized. Tat means
 * that a cpp code for those can be created.
 */
class Serializable {
  public:
  /** \typedef boost shared pointer to the Functor
   */
  typedef boost::shared_ptr<Serializable> Ptr;
  /** Default constructor. Counts all serializable objects and assigns them
   * unique id.
   */
  Serializable() {
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
  string generateCppCode() {
    this->generateCppCode(this->getDefaultSerializableObjectName());
  }
  /** Returns unique id of the serializable object.
   */
  size_t getId() {return id_;}
  /** The function returns a string with default name for the object.
   */
  string getDefaultSerializableObjectName() {
    return "serializable_object_" + boost::lexical_cast<string>(id_);
  }

  virtual ~Serializable() {}

  private:
  /** The unique id of the serializable object.
   */
  size_t id_;
};
}
#endif  // LTR_UTILITY_SERIALIZABLE_H_
