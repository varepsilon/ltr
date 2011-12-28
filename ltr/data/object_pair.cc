// Copyright 2011 Yandex

#include <sstream>

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/utility/numerical.h"

namespace ltr {

ObjectPair::ObjectPair() {}

ObjectPair::ObjectPair(const Object& o1, const Object& o2)
    :o1_(o1),
    o2_(o2) {}

ObjectPair::ObjectPair(const std::vector<Object>& objects)
    :o1_(objects[0]),
    o2_(objects[1]) {}


const Object& ObjectPair::first() const {
    return o1_;
}
Object& ObjectPair::first() {
    return o1_;
}
const Object& ObjectPair::second() const {
    return o2_;
}
Object& ObjectPair::second() {
    return o2_;
}

const Object& ObjectPair::operator[](size_t i) const {
    return at(i);
}
Object& ObjectPair::operator[](size_t i) {
    return at(i);
}
const Object& ObjectPair::at(const size_t j) const {
    if (j < 1) return this->o1_;
    return this->o2_;
}
Object& ObjectPair::at(const size_t j) {
    if (j < 1) return this->o1_;
    return this->o2_;
}

size_t ObjectPair::size() const {
    return 2;
};

ObjectPair ObjectPair::deepCopy() const {
    ObjectPair result = *this;
    result.o1_ = this->o1_.deepCopy();
    result.o2_ = this->o2_.deepCopy();
    return result;
}
bool operator==(const ObjectPair& p1, const ObjectPair& p2) {
  return p1.first() == p2.first() && p1.second() == p2.second();
}

bool operator!=(const ObjectPair& p1, const ObjectPair& p2) {
  return !(p1 == p2);
}
}
