// Copyright 2012 Yandex

#include "ltr/utility/boost/path.h"

namespace ltr {
namespace utility {

string FixPathSeparators(const string& path) {
  # if defined(_WIN32) || defined(__CYGWIN__)
  const char preferred_separator = '\\';
  const char wrong_separator = '/';
  # else
  const char preferred_separator = '/';
  const char wrong_separator = '\\';
  # endif

  string fixed_path = path;
  for (size_t i = 0; i < fixed_path.length(); ++i) {
    if (fixed_path[i] == wrong_separator) {
      fixed_path[i] = preferred_separator;
    }
  }
  return fixed_path;
}

};
};
