// Copyright 2011 Yandex

#ifndef LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_OLD_H_
#define LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_OLD_H_

#include <boost/variant.hpp>

#include <map>
#include <string>
#include <vector>
#include <stdexcept>

// I don't need it. It is for Hook!!!
#include <algorithm>

using std::string;
using std::vector;
using std::map;

namespace ltr {
  typedef vector<int> List;
  /**
   * ParametersContainer holds parameters of types int, double and bool
   * with their names
   */
  class ParametersContainerOld {
  public:
    typedef map<string,
                boost::variant<int, double, bool, List> > TGroup;

    typedef map<string, TGroup> TMap;

    ParametersContainerOld() {
      params[""] = TGroup();
    }
    explicit ParametersContainerOld(TMap parameters) : params(parameters) {}

    template<class T>
    void set(const string& name, const T& value, const string& group="") {
      params[group][name] = value;
    }

    void setDouble(const string& name, double value, const string& group="");
    void setInt(const string& name, int value, const string& group="");
    void setBool(const string& name, bool value, const string& group="");
    void setList(const string& name, const List& value, const string& group="");

    template<class T>
    bool has(const string& name, const string& group="") const;
    bool hasGrouop(const string& group) const {
      return params.find(group) != params.end();
    }

    template<class T> T get(const string& name, const string& group="") const;
    double getDouble(const string& name, const string& group="") const;
    int getInt(const string& name, const string& group="") const;
    bool getBool(const string& name, const string& group="") const;
    List getList(const string& name, const string& group="") const;

    string toString() const;

    void copy(const ParametersContainerOld& parameters);
    /**
     * Returns parametersContainer of all parameters, which has the given group.
     * In the resulting container all theese parameters will have the default group.
     */
    ParametersContainerOld getGroup(const string& group) const;

    /**
     * Adds all the parameters from the given parametersContainer
     * into the given group.
     * @param container - container to copy from.
     * @param group - group to copy parameters in.
     */
    void setGroup(const ParametersContainerOld& container, const string& group);
    /**
     * Removes group of parameters.
     */
    void removeGroup(const string& group) {
      params.erase(group);
    }

    void clear();

  protected:
    TMap params;
  };

  // Template realization
  template <class T>
  bool ParametersContainerOld::has(const string& name,
                                const string& group) const {
    if (params.find(group) == params.end())
      return 0;
    const TGroup& group_ = params.find(group)->second;

    if (group_.find(name) == group_.end())
      return 0;
    try {
      boost::get<T>(group_.find(name)->second);
      return 1;
    } catch(...) {
      return 0;
    }
  }

  template <class T>
  T ParametersContainerOld::get(const string& name,
                             const string& group) const {
    if (params.find(group) == params.end()) {
      throw std::logic_error("no such group: " + group);
    }
    const TGroup& group_ = params.find(group)->second;
    if (group_.find(name) == group_.end()) {
      throw std::logic_error("no such parameter: " + name);
    }
    try {
      return boost::get<T>(group_.find(name)->second);
    } catch(...) {
      throw std::logic_error("parameter " + name + " has another type");
    }
  }
}
#endif  // LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_OLD_H_
