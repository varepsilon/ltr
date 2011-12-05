// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>

#include "ltr/parameters_container/parameters_container.h"

namespace ltr {
  void ParametersContainer::setFloat(const std::string& name,
                                     float value) {
    params[name] = value;
  };

  void ParametersContainer::setInt(const std::string& name, int value) {
    params[name] = value;
  };

  void ParametersContainer::setBool(const std::string& name, bool value) {
    params[name] = value;
  };

  float ParametersContainer::getFloat(const std::string& name) const {
    return get<float>(name);
  };

  int ParametersContainer::getInt(const std::string& name) const {
    return get<int>(name);
  };

  bool ParametersContainer::getBool(const std::string& name) const {
    return get<bool>(name);
  };

  std::string ParametersContainer::getString() const {
    std::string output;
    for (TMap::const_iterator it = params.begin(); it != params.end(); it++) {
        output.append(it->first + " "
            + boost::lexical_cast<std::string>(it->second) + " ");
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



  ParametersContainer& Parameterized::parameters() {
    return parameters_;
  }

  const ParametersContainer& Parameterized::parameters() const {
    return parameters_;
  }

  void Parameterized::setDefaultParameters() {
  }

  std::string Parameterized::checkParameters() const {
    return "";
  }
};
