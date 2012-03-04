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
    typedef map<string,
                map<string,
                    boost::variant<int, double, bool, List> > > TMap;

    typedef map<string,
                boost::variant<int, double, bool, List> > TGroup;

    ParametersContainer() {
      params[""] = TGroup();
    }
    explicit ParametersContainer(TMap parameters) : params(parameters) {}

    void setDouble(const string& name, double value, const string& group="");
    void setInt(const string& name, int value, const string& group="");
    void setBool(const string& name, bool value, const string& group="");
    void setList(const string& name, const List& value, const string& group="");

    template<class T>
    bool has(const string& name, const string& group="") const;

    template<class T> T get(const string& name, const string& group="") const;
    double getDouble(const string& name, const string& group="") const;
    int getInt(const string& name, const string& group="") const;
    bool getBool(const string& name, const string& group="") const;
    List getList(const string& name, const string& group="") const;

    string getString() const;

    void copyParameters(const ParametersContainer& parameters);
    ParametersContainer getParametersGroup(const string& group) const;

    void clear();

  protected:
    TMap params;
  };

  // Template realization
  template <class T>
  bool ParametersContainer::has(const string& name,
                                const string& group) const {
    if (params.find(group) == params.end())
      return 0;
    const TGroup& gr = params.find(group)->second;

    if (gr.find(name) == gr.end())
      return 0;
    try {
      boost::get<T>(gr.find(name)->second);
      return 1;
    } catch(...) {
      return 0;
    }
  }

  template <class T>
  T ParametersContainer::get(const string& name,
                             const string& group) const {
    if (params.find(group) == params.end()) {
      throw std::logic_error("no such group: " + group);
    }
    const TGroup& gr = params.find(group)->second;
    if (gr.find(name) == gr.end()) {
      throw std::logic_error("no such parameter: " + name);
    }
    try {
      return boost::get<T>(gr.find(name)->second);
    } catch(...) {
      throw std::logic_error("parameter " + name + " has another type");
    }
  }
}
#endif  // LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_
