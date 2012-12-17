// Copyright 2012 Yandex
#include <stdexcept>

#include "ltr_client/utility/parameterized_info.h"

using std::logic_error;

typedef string ParameterizedDependency;

void ParameterizedInfo::fill_dependency_list(
    const ParameterizedInfos& token_specs) {
  dependency_specs_.clear();

  typedef ltr::ParametersContainer::NameValue<const ParameterizedDependency>
      TNameValue;
  typedef list<TNameValue> TDependencies;
  const TDependencies& my_dependencies =
      parameters_.getValuesByType<const ParameterizedDependency>();

  for (TDependencies::const_iterator my_dependency_it = my_dependencies.begin();
      my_dependency_it != my_dependencies.end();
      ++my_dependency_it) {
    const TNameValue& dependency = *my_dependency_it;
    const ParameterizedInfo* found = NULL;

    ParameterizedInfos::const_iterator it = token_specs.find(
          dependency.value);
    found = &it->second;

    if (!found) {
      throw logic_error("TXmlTokenSpec::fillDependency List: "
                        "Could not resolve dependency " +
                        dependency.value);
    }
    dependency_specs_.push_back(found);
  }
}

ParameterizedInfo::ParameterizedInfo()
  : tag_name_()
  , object_name_()
  , object_type_()
  , approach_()
  , parameters_()
  , dependency_specs_() {}

ParameterizedInfo::ParameterizedInfo(const string& tag_name,
                 const string& object_name,
                 const string& object_type,
                 const string& approach,
                 ltr::ParametersContainer parameters)
  : tag_name_(tag_name)
  , object_name_(object_name)
  , object_type_(object_type)
  , approach_(approach)
  , parameters_(parameters) {}

ParameterizedInfo::ParameterizedInfo(const ParameterizedInfo& other) {
  *this = other;
}

ParameterizedInfo& ParameterizedInfo::operator= (const ParameterizedInfo& other) {
  if (this == &other) {
    return *this;
  }
  object_name_ = other.get_name();
  object_type_ = other.get_type();
  approach_ = other.get_approach();
  parameters_ = other.get_parameters();
  dependency_specs_ = other.dependency_specs();
  return *this;
}


ParameterizedInfo::~ParameterizedInfo() {
}

const string& ParameterizedInfo::get_tag_name() const {
  return tag_name_;
}
const string& ParameterizedInfo::get_name() const {
  return object_name_;
}
const string& ParameterizedInfo::get_type() const {
  return object_type_;
}
const string& ParameterizedInfo::get_approach() const {
  return approach_;
}
const ltr::ParametersContainer& ParameterizedInfo::get_parameters() const {
  return parameters_;
}
const ParameterizedInfosList& ParameterizedInfo::dependency_specs() const {
  return dependency_specs_;
}

string ToString(const ParameterizedInfo& info) {
  stringstream out(stringstream::out);
  out << "TXmlTokenSpec(name=" << info.get_name()
      << ", type=" << info.get_type()
      << ", approach=" << info.get_approach()
      << ", parameters=" << info.get_parameters().toString()
         << ", my dependencies=(";
  for (ParameterizedInfosList::const_iterator it =
         info.dependency_specs().begin();
       it != info.dependency_specs().end();
       ++it) {
    out << (*it)->get_name() << ", ";
  }
  out << "))";
  return out.str();
}
