// Copyright 2011 Yandex

#include "ltr/parameters_container/parameters_container.h"

#include <iostream>

namespace ltr {
  void ParametersContainer::parse(const std::string& parameters) {
    std::string line;
    std::stringstream str;
    str << parameters;
    while (str >> line) {
		if (params.find(line) == params.end()) {
          throw logic_error("can't parse: unknown parameter " + line);
		}
      switch (params[line].which()) {
          case 0: {
                  int tm;
                  str >> tm;
                  params[line] = tm;
               } break;
          case 1: {
                  float tm;
                  str >> tm;
                  params[line] = tm;
               } break;
          case 2: {
                  bool tm;
                  str >> tm;
                  params[line] = tm;
               } break;
      }
    }
  };

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

  void ParametersContainer::operator >> (std::string& parameters) {
    std::stringstream p_stream;
    for (TMap::iterator it = params.begin(); it != params.end(); it++)
        p_stream << it->first << " " << it->second << " ";
    parameters = p_stream.str();
  }

  void ParametersContainer::copyParameters(
                                       const ParametersContainer& parameters) {
    for (TMap::const_iterator it = parameters.params.begin();
                        it != parameters.params.end(); ++it) {
        params[it->first] = it->second;
    }
  }
};
