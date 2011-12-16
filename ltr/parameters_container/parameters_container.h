// Copyright 2011 Yandex

#ifndef LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_
#define LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_

#include <boost/variant.hpp>

#include <map>
#include <string>
#include <vector>
#include <stdexcept>

using std::string;
using std::map;

namespace ltr {
  class ParametersContainer {
  public:
    void setFloat(const string& name, float value);
    void setInt(const string& name, int value);
    void setBool(const string& name, bool value);

    int getInt(const string& name) const;
    float getFloat(const string& name) const;
    bool getBool(const string& name) const;

    string getString() const;

    void copyParameters(const ParametersContainer& parameters);

  protected:
    template<class T> T get(const string &name) const;

    typedef map<string, boost::variant<int, float, bool> > TMap;
    TMap params;
  };


  // Template realization
  template <class T>
  T ParametersContainer::get(const std::string &name) const {
    if (params.find(name) == params.end()) {
      throw std::logic_error(name + " no such parameter");
    }
    try {
      return boost::get<T>(params.find(name)->second);
    } catch(...) {
      throw std::logic_error("parameter " + name + " has another type");
    }
  }
};

#endif  // LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_
