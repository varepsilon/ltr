// Copyright 2011 Yandex

#ifndef LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_
#define LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_

#include <boost/variant.hpp>

#include <map>
#include <string>
#include <vector>
#include <stdexcept>

using std::string;
using std::vector;
using std::map;

namespace ltr {
  typedef vector<int> List;
  /**
   * ParametersContainer holds parameters of types int, double and bool
   * with their names
   */
  class ParametersContainer {
  public:
    typedef map<string, boost::variant<int, double, bool, List> > TMap;

    ParametersContainer() {}
    explicit ParametersContainer(TMap parameters) : params(parameters) {}

    void setDouble(const string& name, double value);
    void setInt(const string& name, int value);
    void setBool(const string& name, bool value);
    void setList(const string& name, const List& value);

    double getDouble(const string& name) const;
    int getInt(const string& name) const;
    bool getBool(const string& name) const;
    List getList(const string& name) const;

    string getString() const;

    void copyParameters(const ParametersContainer& parameters);

    void clear();

  protected:
    template<class T> T get(const string& name) const;

    TMap params;
  };


  // Template realization
  template <class T>
  T ParametersContainer::get(const string& name) const {
    if (params.find(name) == params.end()) {
      throw std::logic_error(name + " no such parameter");
    }
    try {
      return boost::get<T>(params.find(name)->second);
    } catch(...) {
      throw std::logic_error("parameter " + name + " has another type");
    }
  }
}
#endif  // LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_
