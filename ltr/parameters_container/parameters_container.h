// Copyright 2011 Yandex

#ifndef LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_
#define LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_

#include "boost/variant.hpp"

#include <map>
#include <string>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace ltr {
  class ParametersContainer {
  public:
    /*
     * Parse line to set new parameters' values.
     * @param line - line for parsing
     */
    void parse(const std::string& parameters);

    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    void setBool(const std::string& name, bool value);

    int getInt(const std::string& name) const;
    float getFloat(const std::string& name) const;
    bool getBool(const std::string& name) const;

    void operator>> (std::string& parameters);

    /*
     * copies parameters from another container
     * @param parameters - container to copy parameters from
     */
    void copyParameters(const ParametersContainer& parameters);

  protected:
    template<class T> T get(const std::string &name) const;
    
    typedef std::map<string, boost::variant<int, float, bool> > TMap;
    TMap params;
  };


  // Template realization
  template <class T>
  T ParametersContainer::get(const std::string &name) const {
    if(params.find(name) == params.end()) {
        throw logic_error(name + " no such parameter");
	}
    try {
		return boost::get<T>(params.find(name)->second);
    } catch(...) {
        throw logic_error("parameter " + name + " has another type");
    }
  }
};

#endif  // LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_
