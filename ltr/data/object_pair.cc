// Copyright 2011 Yandex

#include <sstream>

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/utility/numerical.h"

namespace ltr {

ObjectPair::ObjectPair() {}

ObjectPair::ObjectPair(const Object& first, const Object& second)
                      : first_(first),
                        second_(second) {}

const Object& ObjectPair::first() const {
  return first_;
}
Object& ObjectPair::first() {
  return first_;
}
const Object& ObjectPair::second() const {
  return second_;
}
Object& ObjectPair::second() {
  return second_;
}

const Object& ObjectPair::operator[](size_t i) const {
  return at(i);
}

Object& ObjectPair::operator[](size_t i) {
  return at(i);
}

const Object& ObjectPair::at(size_t i) const {
  if (i != 0 && i != 1) {
    throw std::logic_error("Error: pair indexation with index > 1");
  }
  return (i == 0) ? this->first_ : this->second_;
}

Object& ObjectPair::at(size_t i) {
  if (i != 0 && i != 1) {
    throw std::logic_error("Error: pair indexation with index > 1");
  }
  return (i == 0) ? this->first_ : this->second_;
}

size_t ObjectPair::size() const {
  return 2;
};

ObjectPair ObjectPair::deepCopy() const {
  ObjectPair result = *this;
  result.first_ = this->first_.deepCopy();
  result.second_ = this->second_.deepCopy();
  return result;
}

string ObjectPair::toString() const {
  std::stringstream str;
  str << '(' << first().toString() << ", " << second().toString() << ')';
  return str.str();
}

bool operator==(const ObjectPair& lhs, const ObjectPair& rhs) {
  return lhs.first() == rhs.first() && lhs.second() == rhs.second();
}

bool operator!=(const ObjectPair& lhs, const ObjectPair& rhs) {
  return !(lhs == rhs);
}
}
