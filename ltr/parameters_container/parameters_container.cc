// Copyright 2011 Yandex

#include <sstream>

#include "ltr/parameters_container/parameters_container.h"

using std::stringstream;

namespace ltr {

ParametersContainer::ParametersContainer() {
}

ParametersContainer::ParametersContainer(const ParametersContainer& other) {
  *this = other;
}

ParametersContainer& ParametersContainer::operator=(
    const ParametersContainer& other) {
  name_value_hash_ = other.name_value_hash_;
  return *this;
}

void ParametersContainer::Clear() {
  name_value_hash_.clear();
}

bool ParametersContainer::Contains(const string& name) const {
  return name_value_hash_.find(to_lower(name)) != name_value_hash_.end();
}

void ParametersContainer::Copy(const ParametersContainer& parameters) {
  for (StringAnyHash::const_iterator iterator =
       parameters.name_value_hash_.begin();
       iterator != parameters.name_value_hash_.end();
       ++iterator) {
    const string& name = iterator->first;
    const Any& value = iterator->second;

    if (!Contains(name))  {
      string error_string = "Wrong parameter name " + name + "\n";
      error_string += "Did you mean one of these:";
      for (StringAnyHash::const_iterator iterator = name_value_hash_.begin();
           iterator != name_value_hash_.end();
           ++iterator) {
        error_string += " " + iterator->first;
    }
      error_string += "?";
      throw std::logic_error(error_string);
    } else if (value.type() != name_value_hash_[name].type() &&
               !(value.type().before(name_value_hash_[name].type()) ||
                name_value_hash_[name].type().before(value.type()))) {
      throw std::logic_error("Wrong parameter " + name + " type\n" +
                             "value.type(): " + value.type().name() +
                             "\n name_value_hash_[name].type():" +
                             name_value_hash_[name].type().name());
    } else {
      name_value_hash_[name] = value;
    }
  }
}

string ParametersContainer::toString() const {
  stringstream out;
  if (name_value_hash_.size() == 0) {
    out << "ParametersContainer(empty)";
    return out.str();
  }
  out <<
    "ParametersContainer (number of parameters=" << name_value_hash_.size();
  StringAnyHash::size_type parameter_index = 1;
  for (StringAnyHash::const_iterator iterator = name_value_hash_.begin();
       iterator != name_value_hash_.end();
       ++iterator) {
    const string& name = iterator->first;
    const Any& value = iterator->second;
    out << "\n ";
    out << parameter_index++ << ") name=" << name
        << ", type=" << value.type().name();
  }
  out << ')';
  return out.str();
}
}
