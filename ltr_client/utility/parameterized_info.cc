// Copyright 2012 Yandex
#include <stdexcept>
#include <assert.h>

#include "ltr_client/utility/parameterized_info.h"
#include "ltr/utility/boost/string_utils.h"

using std::logic_error;

using ltr::utility::split;

typedef string ObjectDependency;

void ObjectInfo::fill_dependency_list(
    const ObjectInfos& object_infos) {
  dependency_infos_.clear();

  typedef ltr::ParametersContainer::NameValue<const ObjectDependency>
      TNameValue;
  typedef list<TNameValue> TDependencies;
  const TDependencies& my_dependencies =
      parameters_.getValuesByType<const ObjectDependency>();

  for (TDependencies::const_iterator my_dependency_it = my_dependencies.begin();
      my_dependency_it != my_dependencies.end();
      ++my_dependency_it) {
    const TNameValue& dependency = *my_dependency_it;
    vector<string> dependency_parts;
    split(dependency.value, "\t", &dependency_parts);
    split(&dependency_parts, " ");
    split(&dependency_parts, ",");
    for (int i = 0; i < dependency_parts.size(); ++i) {
      const ObjectInfo* found = NULL;

      if (dependency_parts[i].empty()) continue;  // TODO: remove
      // this after getting rid of boost::split

      assert(!dependency_parts[i].empty());

      ObjectInfos::const_iterator it = object_infos.find(
            dependency_parts[i]);
      assert(it != object_infos.end());
      found = &it->second;

      if (!found) {
        throw logic_error("TXmlObjectInfo::fillDependency List: "
                          "Could not resolve dependency " +
                          dependency_parts[i]);
      }
      dependency_infos_.push_back(found);
    }
  }
}

ObjectInfo::ObjectInfo()
  : tag_name_()
  , object_name_()
  , object_type_()
  , approach_()
  , parameters_()
  , dependency_infos_() {}

ObjectInfo::ObjectInfo(const string& tag_name,
                 const string& object_name,
                 const string& object_type,
                 const string& approach,
                 ltr::ParametersContainer parameters)
  : tag_name_(tag_name)
  , object_name_(object_name)
  , object_type_(object_type)
  , approach_(approach)
  , parameters_(parameters) {}

ObjectInfo::ObjectInfo(const ObjectInfo& other) {
  *this = other;
}

ObjectInfo& ObjectInfo::operator= (const ObjectInfo& other) {
  if (this == &other) {
    return *this;
  }
  object_name_ = other.get_name();
  object_type_ = other.get_type();
  approach_ = other.get_approach();
  parameters_ = other.get_parameters();
  dependency_infos_ = other.dependency_infos();
  return *this;
}


ObjectInfo::~ObjectInfo() {
}

const string& ObjectInfo::get_tag_name() const {
  return tag_name_;
}
const string& ObjectInfo::get_name() const {
  return object_name_;
}
const string& ObjectInfo::get_type() const {
  return object_type_;
}
const string& ObjectInfo::get_approach() const {
  return approach_;
}
const ltr::ParametersContainer& ObjectInfo::get_parameters() const {
  return parameters_;
}
const ObjectInfosList& ObjectInfo::dependency_infos() const {
  return dependency_infos_;
}

string ToString(const ObjectInfo& info) {
  stringstream out(stringstream::out);
  out << "TXmlTokenInfo(name=" << info.get_name()
      << ", type=" << info.get_type()
      << ", approach=" << info.get_approach()
      << ", parameters=" << info.get_parameters().toString()
         << ", my dependencies=(";
  for (ObjectInfosList::const_iterator it =
         info.dependency_infos().begin();
       it != info.dependency_infos().end();
       ++it) {
    out << (*it)->get_name() << ", ";
  }
  out << "))";
  return out.str();
}
