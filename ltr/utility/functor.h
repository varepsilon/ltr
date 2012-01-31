// Copyright 2012 Yandex

#ifndef LTR_UTILITY_FUNCTOR_H_
#define LTR_UTILITY_FUNCTOR_H_

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <string>

#include "ltr/data/object.h"

using std::string;
using boost::shared_ptr;
using boost::lexical_cast;

namespace ltr {

extern size_t n_serializable_functors_;

/**
 * Base class for all serializable functors
 * TValue - type of the result of the function
 */
template<class TValue>
class SerializableFunctor {
  private:
    int id;

  public:
    typedef shared_ptr<SerializableFunctor> Ptr;
    SerializableFunctor() {
      id = n_serializable_functors_++;
    }

    /**
     * Function returns unique Id of the functor
     */
    int getId() {return id;}

    /**
     * Function returns default function name for this functor
     */
    string getDefaultFunctionName() {
      return "function" + lexical_cast<string>(id);
    }

    /**
     * Function returns serialized code of this functor
     * @param function_name - name of the function to generate
     */
    virtual string generateCppCode(const string& function_name) const = 0;

    /**
     * Function returns serialized code of this functor
     * with default function name
     */
    string generateCppCode() {
      return generateCppCode(getDefaultFunctionName());
    }

    /**
     * Function returns the value of this functor for the given object
     */
    TValue operator()(const Object& obj) const {
      return value(obj);
    }
    /**
     * Function returns the value of this functor for the given object
     */
    virtual TValue value(const Object& obj) const = 0;
    virtual ~SerializableFunctor() {}
};
}
#endif  // LTR_UTILITY_FUNCTOR_H_
