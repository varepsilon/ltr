// Copyright 2011 Yandex


//#include<cstdio>
#include <boost/lexical_cast.hpp>

#include "ltr/parameters_container/parameters_container.h"


namespace ltr {
    // todo: rewrite using boost::spirit
  /*void ParametersContainer::parse(const std::string& parameters) {
    char *c_string, *current;
    c_string = new char[parameters.size() + 1];
    strcpy_s(c_string, parameters.size() + 1, parameters.c_str());

    current = strtok_s(c_string," \t\n", NULL);
    while (current != NULL) {
        std::string line(current);
        if (params.find(line) == params.end()) {
            throw std::logic_error("can't parse: unknown parameter " + line);
        }
        current = strtok_s(c_string," \t\n", NULL);
        switch (params[line].which()) {
          case 0: {
                  int tm;
                  sscanf (current,"%d",&tm);
                  params[line] = tm;
               } break;
          case 1: {
                  float tm;
                  sscanf (current,"%f",&tm);
                  params[line] = tm;
               } break;
          case 2: {
                  params[line] = strcmp(current,"true");
               } break;
      }
      current = strtok_s(c_string," \t\n", NULL);
    }
    delete[] c_string;
  };*/

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
};
