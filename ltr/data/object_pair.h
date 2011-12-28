// Copyright 2011 Yandex

#ifndef LTR_DATA_OBJECT_PAIR_H_
#define LTR_DATA_OBJECT_PAIR_H_

#include <vector>

#include "ltr/data/object.h"

#include <boost/shared_ptr.hpp>

namespace ltr {

class ObjectPair {
    public:
    typedef boost::shared_ptr< ObjectPair > Ptr;

    ObjectPair();
    ObjectPair(const Object& o1, const Object& o2);
    /**
     * makes ObjectPair from objects[0] and objects[1]
     */
    ObjectPair(const std::vector<Object>& objects);

    const Object& first() const;
    Object& first();
    const Object& second() const;
    Object& second();

    const Object& operator[](size_t i) const;
    Object& operator[](size_t i);
    const Object& at(const size_t j) const;
    Object& at(const size_t j);

    /*
     * Returns always 2.
     */
    size_t size() const;

    ObjectPair deepCopy() const;

    private:
    Object o1_;
    Object o2_;
};

bool operator==(const ObjectPair& o1, const ObjectPair& o2);
bool operator!=(const ObjectPair& o1, const ObjectPair& o2);
std::ostream& operator<<(std::ostream& ostr, const ObjectPair& pair);
}

#endif  // LTR_DATA_OBJECT_PAIR_H_
