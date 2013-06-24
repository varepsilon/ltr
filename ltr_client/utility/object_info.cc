// Copyright 2012 Yandex
#include <sstream>
#include <iostream>

#include "ltr_client/utility/object_info.h"
#include "ltr/utility/boost/string_utils.h"
#include "ltr/utility/macros.h"

using std::stringstream;

using ltr::utility::split;

typedef string ObjectDependency;

void ObjectInfo::fill_dependency_list(const ObjectInfos& object_infos) {
  dependency_infos.clear();

  typedef ParametersContainer::NameValue<const ObjectDependency> TNameValue;
  typedef list<TNameValue> TDependencies;

  const TDependencies& my_dependencies =
      parameters.getValuesByType<const ObjectDependency>();

  for (TDependencies::const_iterator my_dependency_it = my_dependencies.begin();
      my_dependency_it != my_dependencies.end();
      ++my_dependency_it) {
    const TNameValue& dependency = *my_dependency_it;
    vector<string> dependency_parts;

    split(dependency.value, "\t", &dependency_parts);
    split(&dependency_parts, " ");
    split(&dependency_parts, ",");

    for (int i = 0; i < dependency_parts.size(); ++i) {
      CHECK_MSG(object_infos.contains(dependency_parts[i]),
                "ObjectInfo::fillDependency List: "
                "Could not resolve dependency " + dependency_parts[i]);
      dependency_infos.push_back(
        object_infos.safeFind(dependency_parts[i])->second);
    }
  }
}

ObjectInfo::ObjectInfo(const string& tag_name_,
                       const string& object_name_,
                       const string& object_type_,
                       const string& approach_,
                       ParametersContainer parameters_)
  : tag_name(tag_name_)
  , name(object_name_)
  , type(object_type_)
  , approach(approach_)
  , parameters(parameters_) {}

ObjectInfo::ObjectInfo(const ObjectInfo& other) {
  *this = other;
}

ObjectInfo& ObjectInfo::operator= (const ObjectInfo& other) {
  if (this == &other) {
    return *this;
  }
  tag_name = other.tag_name;
  name = other.name;
  type = other.type;
  approach = other.approach;
  parameters = other.parameters;
  dependency_infos = other.dependency_infos;
  return *this;
}

string ObjectInfo::toString() const {
  stringstream out(stringstream::out);
  out << "ObjectInfo(name=" << name
      << ", type=" << type
      << ", approach=" << approach
      << ", parameters=" << parameters.toString()
      << ", my dependencies=(";
  for (ObjectInfosList::const_iterator it = dependency_infos.begin();
       it != dependency_infos.end();
       ++it) {
    out << (*it)->name << ", ";
  }
  out << "))";
  return out.str();
}
