// Copyright 2012 Yandex

#ifndef LTR_INTERFACES_PRINTABLE_H_
#define LTR_INTERFACES_PRINTABLE_H_

#include <ostream>
#include <string>
#include <vector>
#include <sstream>

using std::string;
using std::vector;
using std::stringstream;
using std::fixed;

namespace ltr {

class Printable {
 public:
  virtual string toString() const = 0;
  virtual ~Printable() {}
};

std::ostream& operator<<(std::ostream& stream, const Printable& printable);

template<typename T>
static string vectorToString(vector<T> vectorForConversion) {
  stringstream str;
  fixed(str);
  str.precision(2);
  str << "[";
  for (size_t i = 0; i < vectorForConversion.size(); ++i) {
    if (i != 0) {
      str << ", ";
    }
    str << vectorForConversion[i];
  }
  str << "]";
  return str.str();
}
}

#endif  // LTR_INTERFACES_PRINTABLE_H_
