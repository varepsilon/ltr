// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>
#include <string>
#include <set>
#include <stdexcept>

#include "ltr/parameters_container/parameters_container_old.h"

using std::string;
using std::logic_error;

namespace ltr {
  class toStringVisitor : public boost::static_visitor<string> {
  public:
    template<class type>
    string operator()(const type& a) const {
      return boost::lexical_cast<string>(a);
    }
  };
  template<>
  string toStringVisitor::operator()<List>(const List& a) const {
    string res = "[";
    for (int i = 0; i < a.size(); i++) {
      if (i != 0)
        res.append(",");
      res.append(boost::lexical_cast<string>(a[i]));
    }
    res.append("]");
    return res;
  }

  void ParametersContainerOld::setDouble(const string& name,
                                     double value,
                                     const string& group) {
    set<double>(name, value, group);
  };

  void ParametersContainerOld::setInt(const string& name,
                                   int value,
                                   const string& group) {
    set<int>(name, value, group);
  };

  void ParametersContainerOld::setBool(const string& name,
                                    bool value,
                                    const string& group) {
    set<bool>(name, value, group);
  };

  void ParametersContainerOld::setList(const string& name,
                                    const List& value,
                                    const string& group) {
    set<List>(name, value, group);
  };

  double ParametersContainerOld::getDouble(const string& name,
                                        const string& group) const {
    return get<double>(name, group);
  };

  int ParametersContainerOld::getInt(const string& name,
                                  const string& group) const {
    return get<int>(name, group);
  };

  List ParametersContainerOld::getList(const string& name,
                                    const string& group) const {
    return get<List>(name, group);
  }

  bool ParametersContainerOld::getBool(const string& name,
                                    const string& group) const {
    return get<bool>(name, group);
  };

  string ParametersContainerOld::toString() const {
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

  void ParametersContainerOld::copy(const ParametersContainerOld& parameters) {
    for (TMap::const_iterator it = parameters.params.begin();
                        it != parameters.params.end(); ++it) {
      if (params.find(it->first) == params.end()) {
        throw logic_error("Wrong parameter group name: " + it->first);
      }
      const TGroup& group_ = it->second;
      TGroup& my_group_ = params[it->first];
      for (TGroup::const_iterator g_it = group_.begin();
                                   g_it != group_.end(); g_it++) {
        if (my_group_.find(g_it->first) == my_group_.end()) {
          string err = "Wrong parameter name " + g_it->first;
          err.append(". You may mean one of these:");
          for (TGroup::const_iterator inner_it = my_group_.begin();
              inner_it != my_group_.end(); ++inner_it) {
            err.append(" " + inner_it->first);
          }
          err.append(".");
          throw logic_error(err);
        } else if (my_group_[g_it->first].type() != g_it->second.type()) {
          throw logic_error("Wrong parameter " + g_it->first + " type");
        } else {
          my_group_[g_it->first] = g_it->second;
        }
      }
    }
  }

  ParametersContainerOld ParametersContainerOld::getGroup
      (const string& group) const {
    ParametersContainerOld result;
    if (params.find(group) == params.end())
      return result;
    const TGroup& group_ = params.find(group)->second;
    for (TGroup::const_iterator it = group_.begin(); it != group_.end(); it++)
      result.params[""][it->first] = it->second;
    return result;
  }

  void ParametersContainerOld::setGroup(const ParametersContainerOld& container,
                                     const string& group) {
    for (TMap::const_iterator it = container.params.begin();
         it != container.params.end(); it++) {
      const TGroup& group_ = it->second;
      for (TGroup::const_iterator g_it = group_.begin();
           g_it != group_.end(); g_it++) {
        params[group][g_it->first] = g_it->second;
      }
    }
  }

  void ParametersContainerOld::clear() {
    params.clear();
    params[""];
  }
};
