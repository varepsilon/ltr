#include <stdexcept>

#include "ltr_client/utility/parameterized_info.h"

using std::logic_error;

typedef string ParameterizedDependency;

void ParametrizedInfo::fill_dependency_list(
    const TXmlTokenSpecs& token_specs) {
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
    const ParametrizedInfo* found = NULL;

    TXmlTokenSpecs::const_iterator it = token_specs.find(
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

ParametrizedInfo::ParametrizedInfo()
  : tag_name_()
  , name_()
  , type_()
  , approach_()
  , parameters_()
  , dependency_specs_() {}

ParametrizedInfo::ParametrizedInfo(const string& tag_name,
                 const string& name,
                 const string& type,
                 const string& approach,
                 ltr::ParametersContainer parameters)
  : tag_name_(tag_name)
  , name_(name)
  , type_(type)
  , approach_(approach)
  , parameters_(parameters) {}

ParametrizedInfo::ParametrizedInfo(const ParametrizedInfo& other) {
  *this = other;
}

ParametrizedInfo& ParametrizedInfo::operator= (const ParametrizedInfo& other) {
  if (this == &other) {
    return *this;
  }
  name_ = other.get_name();
  type_ = other.get_type();
  approach_ = other.get_approach();
  parameters_ = other.get_parameters();
  dependency_specs_ = other.dependency_specs();
  return *this;
}


ParametrizedInfo::~ParametrizedInfo() {
}

const string& ParametrizedInfo::get_tag_name() const {
  return tag_name_;
}
const string& ParametrizedInfo::get_name() const {
  return name_;
}
const string& ParametrizedInfo::get_type() const {
  return type_;
}
const string& ParametrizedInfo::get_approach() const {
  return approach_;
}
const ltr::ParametersContainer& ParametrizedInfo::get_parameters() const {
  return parameters_;
}
const TXmlTokenSpecList& ParametrizedInfo::dependency_specs() const {
  return dependency_specs_;
}

string ToString(const ParametrizedInfo& info) {
  stringstream out(stringstream::out);
  out << "TXmlTokenSpec(name=" << info.get_name()
      << ", type=" << info.get_type()
      << ", approach=" << info.get_approach()
      << ", parameters=" << info.get_parameters().toString()
         << ", my dependencies=(";
  for (TXmlTokenSpecList::const_iterator it = info.dependency_specs().begin();
      it != info.dependency_specs().end();
      ++it) {
    out << (*it)->get_name() << ", ";
  }
  out << "))";
  return out.str();
}
