// Copyright 2012 Yandex

#include <sstream>

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"

#include "ltr/utility/numerical.h"

namespace ltr {
ObjectPair::ObjectPair() {}

ObjectPair::ObjectPair(const Object& first, const Object& second)
  : first(first),
    second(second) {}

ObjectPair::ObjectPair(InnerRepresentation::Ptr presentation,
                       ElementBounds bounds)
  : first(presentation, bounds.start_line_index),
    second(presentation, bounds.start_line_index + 1) { }

ObjectPair ObjectPair::deepCopy() const {
  ObjectPair result = *this;
  result.first = this->first.deepCopy();
  result.second = this->second.deepCopy();
  return result;
}

string ObjectPair::toString() const {
  std::stringstream str;
  str << '(' << first.toString() << ", " << second.toString() << ')';
  return str.str();
}

bool operator==(const ObjectPair& lhs, const ObjectPair& rhs) {
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

bool operator!=(const ObjectPair& lhs, const ObjectPair& rhs) {
  return !(lhs == rhs);
}
};
