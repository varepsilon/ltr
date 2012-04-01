// Copyright 2012 Yandex

#include "ltr/interfaces/printable.h"
namespace ltr {

std::ostream& operator<<(std::ostream& stream, const IPrintable& printable) {
  return stream << printable.toString();
}
}
