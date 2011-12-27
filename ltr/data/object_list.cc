// Copyright 2011 Yandex


#include <sstream>
#include <vector>

#include "ltr/data/object_list.h"
#include "ltr/utility/numerical.h"

namespace ltr {

ObjectList::ObjectList() : p_Elements_(new std::vector< Object >()) {}

const Object& ObjectList::operator[](size_t i) const {
    return at(i);
}
Object& ObjectList::operator[](size_t i) {
    return at(i);
}
const Object& ObjectList::at(const size_t i) const {
    return (*p_Elements_)[i];
}
Object& ObjectList::at(const size_t i) {
    return (*p_Elements_)[i];
}

ObjectList& ObjectList::operator<<(const Object& obj) {
    (*p_Elements_).push_back(obj.deepCopy());
    return *this;
}
void ObjectList::add(const Object& obj) {
    (*this) << obj;
}

size_t ObjectList::size() const {
    return (*p_Elements_).size();
}

void ObjectList::clear() {
    (*p_Elements_).clear();
}

ObjectList ObjectList::deepCopy() const {
    ObjectList result;
    for (size_t i = 0; i < this->p_Elements_->size(); ++i) {
        result << p_Elements_->at(i);
    }
    return result;
}
}
