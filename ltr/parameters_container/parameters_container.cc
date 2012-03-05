// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>
#include <string>
#include <stdexcept>

#include "ltr/parameters_container/parameters_container.h"

using std::string;
using std::logic_error;

template<>
string boost::lexical_cast<string, ltr::List>(const ltr::List& a) {
  string res = "[";
  for (int i = 0; i < a.size(); i++) {
    if (i != 0)
      res.append(",");
    res.append(boost::lexical_cast<string>(a[i]));
  }
  res.append("]");
  return res;
}

namespace ltr {
  class toStringVisitor : public boost::static_visitor<string> {
  public:
    template<class type>
    string operator()(type a) const {
      return boost::lexical_cast<string>(a);
    }
  };

  void ParametersContainer::setDouble(const string& name,
                                     double value,
                                     const string& group) {
    params[group][name] = value;
  };

  void ParametersContainer::setInt(const string& name,
                                   int value,
                                   const string& group) {
    params[group][name] = value;
  };

  void ParametersContainer::setBool(const string& name,
                                    bool value,
                                    const string& group) {
    params[group][name] = value;
  };

  void ParametersContainer::setList(const string& name,
                                    const List& value,
                                    const string& group) {
    params[group][name] = value;
  };

  double ParametersContainer::getDouble(const string& name,
                                        const string& group) const {
    return get<double>(name, group);
  };

  int ParametersContainer::getInt(const string& name,
                                  const string& group) const {
    return get<int>(name, group);
  };

  List ParametersContainer::getList(const string& name,
                                    const string& group) const {
    return get<List>(name, group);
  }

  bool ParametersContainer::getBool(const string& name,
                                    const string& group) const {
    return get<bool>(name, group);
  };

  string ParametersContainer::toString() const {
    string output;
    for (TMap::const_iterator it = params.begin(); it != params.end(); it++) {
      const TGroup& group_ = it->second;
      for (TGroup::const_iterator g_it = group_.begin();
                                                  g_it != group_.end(); g_it++)
        if (it->first != "")
          output.append(it->first + "." + g_it->first + " = "
            + boost::apply_visitor(toStringVisitor(), g_it->second) + " ");
        else
          output.append(g_it->first + " = "
            + boost::apply_visitor(toStringVisitor(), g_it->second) + " ");
    }
    return output;
  }

  void ParametersContainer::copy(const ParametersContainer& parameters) {
    for (TMap::const_iterator it = parameters.params.begin();
                        it != parameters.params.end(); ++it) {
      if (params.find(it->first) == params.end()) {
        throw logic_error("Wrong parameter group name: " + it->first);
      }
      const TGroup& group_ = it->second;
      TGroup& my_group_ = params[it->first];
      for (TGroup::const_iterator g_it = group_.begin();
                                   g_it != group_.end(); g_it++) {
        if (my_group_.find(g_it->first) != my_group_.end()) {
          my_group_[g_it->first] = g_it->second;
        } else {
          string err = "Wrong parameter name " + g_it->first;
          err.append(". You may mean one of these:");
          for (TGroup::const_iterator inner_it = my_group_.begin();
              inner_it != my_group_.end(); ++inner_it) {
            err.append(" " + inner_it->first);
          }
          err.append(".");
          throw logic_error(err);
        }
      }
    }
  }

  ParametersContainer ParametersContainer::getGroup
      (const string& group) const {
    ParametersContainer result;
    if (params.find(group) == params.end())
      return result;
    const TGroup& group_ = params.find(group)->second;
    for (TGroup::const_iterator it = group_.begin(); it != group_.end(); it++)
      result.params[""][it->first] = it->second;
    return result;
  }

  void ParametersContainer::clear() {
    params.clear();
    params[""];
  }
};
