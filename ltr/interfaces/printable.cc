// Copyright 2012 Yandex

#include "ltr/interfaces/printable.h"
namespace ltr {

std::ostream& operator<<(std::ostream& stream, const Printable& printable) {
  return stream << printable.toString();
}
}
