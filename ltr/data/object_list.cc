// Copyright 2011 Yandex


#include <sstream>
#include <vector>

#include "ltr/data/object_list.h"
#include "ltr/utility/numerical.h"

namespace ltr {

ObjectList::ObjectList() : objects_(new vector<Object>()) {}


const Object& ObjectList::operator[](size_t i) const {
  return at(i);
}
Object& ObjectList::operator[](size_t i) {
  return at(i);
}
const Object& ObjectList::at(size_t i) const {
  return (*objects_)[i];
}
Object& ObjectList::at(size_t i) {
    return (*objects_)[i];
}

ObjectList& ObjectList::operator<<(const Object& obj) {
  (*objects_).push_back(obj.deepCopy());
  return *this;
}
void ObjectList::add(const Object& obj) {
  (*this) << obj;
}

size_t ObjectList::size() const {
  return (*objects_).size();
}

void ObjectList::clear() {
  (*objects_).clear();
}

ObjectList ObjectList::deepCopy() const {
  ObjectList result;
  for (int object_index = 0;
       object_index < (int)this->objects_->size();
       ++object_index) {
    result << objects_->at(object_index);
  }
  return result;
}

string ObjectList::toString() const {
  std::stringstream str;
  str << '{';
  for (int object_index = 0; object_index < (int)size(); ++object_index) {
    if (object_index != 0) {
      str << ", ";
    }
    str << (*this)[object_index].toString();
  }
  str << '}';
  return str.str();
}
}
