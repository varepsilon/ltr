// Copyright 2011 Yandex

#ifndef LTR_DATA_OBJECT_LIST_H_
#define LTR_DATA_OBJECT_LIST_H_

#include "ltr/data/object.h"

#include <boost/shared_ptr.hpp>

#include <vector>

namespace ltr {

class ObjectList {
    public:
    typedef boost::shared_ptr< ObjectList > Ptr;

    ObjectList();

    const Object& operator[](size_t i) const;
    Object& operator[](size_t i);
    const Object& at(const size_t j) const;
    Object& at(const size_t j);

    ObjectList& operator<<(const Object& obj);
    void Add(const Object& obj);

    size_t size() const;
    void clear();

    ObjectList deepCopy() const;

    friend bool operator==(const ObjectList& one, const ObjectList& another);

    private:
    boost::shared_ptr< std::vector<Object> > p_Elements_;
};

bool operator==(const ObjectList& one, const ObjectList& another);
bool operator!=(const ObjectList& one, const ObjectList& another);
std::ostream& operator<<(std::ostream& istr, const ObjectList& list);
}

#endif  // LTR_DATA_OBJECT_LIST_H_