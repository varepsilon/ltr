// Copyright 2012 Yandex

#ifndef LTR_INTERFACES_PRINTABLE_H_
#define LTR_INTERFACES_PRINTABLE_H_

#include <ostream>
#include <string>

using std::string;

namespace ltr {

class IPrintable {
  public:
    virtual string toString() const = 0;
    virtual ~IPrintable() {}
};
std::ostream& operator<<(std::ostream& stream, const IPrintable& printable);
}

#endif  // LTR_INTERFACES_PRINTABLE_H_
