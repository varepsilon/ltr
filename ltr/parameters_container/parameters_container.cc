// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>
#include <string>

#include "ltr/parameters_container/parameters_container.h"

using std::string;

namespace ltr {
  void ParametersContainer::setFloat(const string& name,
                                     float value) {
    params[name] = value;
  };

  void ParametersContainer::setInt(const string& name, int value) {
    params[name] = value;
  };

  void ParametersContainer::setBool(const string& name, bool value) {
    params[name] = value;
  };

  float ParametersContainer::getFloat(const string& name) const {
    return get<float>(name);
  };

  int ParametersContainer::getInt(const string& name) const {
    return get<int>(name);
  };

  bool ParametersContainer::getBool(const string& name) const {
    return get<bool>(name);
  };

  string ParametersContainer::getString() const {
    string output;
    for (TMap::const_iterator it = params.begin(); it != params.end(); it++) {
        output.append(it->first + " "
            + boost::lexical_cast<string>(it->second) + " ");
    }
    return output;
  }

  void ParametersContainer::copyParameters(
                                       const ParametersContainer& parameters) {
    for (TMap::const_iterator it = parameters.params.begin();
                        it != parameters.params.end(); ++it) {
        params[it->first] = it->second;
    }
  }
};
