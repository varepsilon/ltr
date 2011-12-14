// Copyright 2011 Yandex

#ifndef LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_
#define LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_

#include <boost/variant.hpp>

#include <map>
#include <string>
#include <vector>
#include <stdexcept>

namespace ltr {
  class ParametersContainer {
  public:
    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    void setBool(const std::string& name, bool value);

    int getInt(const std::string& name) const;
    float getFloat(const std::string& name) const;
    bool getBool(const std::string& name) const;

    std::string getString() const;

    void copyParameters(const ParametersContainer& parameters);

  protected:
    template<class T> T get(const std::string &name) const;

    typedef std::map<std::string, boost::variant<int, float, bool> > TMap;
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

  class Parameterized {
  public:
    ParametersContainer& parameters();
    const ParametersContainer& parameters() const;
    virtual void setDefaultParameters();
      /*
       * checks, if current parameters are valid.
       * If invalid parameters found - description is written in result. 
       * If no invalids - result will be empty
       */
    virtual std::string checkParameters() const;
    virtual ~Parameterized() {}
  protected:
    ParametersContainer parameters_;
  };
};

#endif  // LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_
