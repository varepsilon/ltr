// Copyright 2012 Yandex

#ifndef LTR_UTILITY_FUNCTOR_H_
#define LTR_UTILITY_FUNCTOR_H_

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <string>

#include "ltr/data/object.h"

using std::string;
using boost::lexical_cast;

namespace ltr {

extern size_t n_serializable_functors_;

template<class TValue>
class SerializableFunctor {
  private:
    int id;

  public:
    SerializableFunctor() {
      id = n_serializable_functors_++;
    }

    int getId() {return id;}
    string getDefaultFunctionName() {
      return "function" + lexical_cast<string>(id);
    }

    virtual string generateCppCode(const string& function_name) const = 0;
    string generateCppCode() {
      return generateCppCode("function" + lexical_cast<string>(id));
    }

    TValue operator()(const Object& obj) const {
      return value(obj);
    }
    virtual TValue value(const Object& obj) const = 0;
    virtual ~SerializableFunctor() {}
};
}
#endif  // LTR_UTILITY_FUNCTOR_H_
