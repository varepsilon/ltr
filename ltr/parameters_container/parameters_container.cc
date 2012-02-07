// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>
#include <string>
#include <stdexcept>

#include "ltr/parameters_container/parameters_container.h"

using std::string;
using std::logic_error;

namespace ltr {
  class printVisitor : public boost::static_visitor<string> {
  public:
    template<class type>
    string operator()(type a) const {
      return boost::lexical_cast<string>(a);
    }
  };
  template<>
  string printVisitor::operator()(List a) const {
    string res = "[";
    for (int i = 0; i < a.size(); i++) {
      if (i != 0)
        res.append(",");
      res.append(boost::lexical_cast<string>(a[i]));
    }
    res.append("]");
    return res;
  }

  void ParametersContainer::setDouble(const string& name,
                                     double value) {
    params[name] = value;
  };

  void ParametersContainer::setInt(const string& name, int value) {
    params[name] = value;
  };

  void ParametersContainer::setBool(const string& name, bool value) {
    params[name] = value;
  };

  void ParametersContainer::setList(const string& name, const List& value) {
    params[name] = value;
  };

  double ParametersContainer::getDouble(const string& name) const {
    return get<double>(name);
  };

  int ParametersContainer::getInt(const string& name) const {
    return get<int>(name);
  };

  List ParametersContainer::getList(const string& name) const {
    return get<List>(name);
  }

  bool ParametersContainer::getBool(const string& name) const {
    return get<bool>(name);
  };

  string ParametersContainer::getString() const {
    string output;
    for (TMap::const_iterator it = params.begin(); it != params.end(); it++) {
        output.append(it->first + " "
          + boost::apply_visitor(printVisitor(), it->second) + " ");
    }
    return output;
  }

  void ParametersContainer::copyParameters(
                                       const ParametersContainer& parameters) {
    for (TMap::const_iterator it = parameters.params.begin();
                        it != parameters.params.end(); ++it) {
        if (params.find(it->first) != params.end()) {
          params[it->first] = it->second;
        } else {
          string err = "Wrong parameter name " + it->first;
          err.append(". You may mean one of these:");
          for (TMap::const_iterator inner_it = params.begin();
              inner_it != params.end(); ++inner_it) {
            err.append(" " + inner_it->first);
          }
          err.append(".");
          throw logic_error(err);
        }
    }
  }

  void ParametersContainer::clear() {
    params.clear();
  }
};
