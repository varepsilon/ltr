// Copyright 2012 Yandex

#include "ltr/utility/boost/path.h"

namespace ltr {
namespace utility {

# if defined(_WIN32) || defined(__CYGWIN__)
const char PREFERRED_SEPARATOR = '\\';
const char WRONG_SEPARATOR = '/';
# else
const char PREFERRED_SEPARATOR = '/';
const char WRONG_SEPARATOR = '\\';
# endif

string FixPathSeparators(const string& path) {
  string fixed_path = path;
  for (size_t i = 0; i < fixed_path.length(); ++i) {
    if (fixed_path[i] == WRONG_SEPARATOR) {
      fixed_path[i] = PREFERRED_SEPARATOR;
    }
  }
  return fixed_path;
}

string AppendTrailingPathSeparator(const string& path) {
  if (*(path.rbegin()) == PREFERRED_SEPARATOR) {
    return path;
  } else {
    return path + PREFERRED_SEPARATOR;
  }
}

string GetFileName(const string& path) {
  int separator_pos = path.find_last_of(PREFERRED_SEPARATOR);
  if (separator_pos != string::npos) {
    return path.substr(separator_pos + 1);
  }
  return "";
}

bool IsAbsoluteFileName(const string& text) {
  int last_separator_pos = text.find_last_of(PREFERRED_SEPARATOR);
  if (last_separator_pos == string::npos ||
      last_separator_pos == text.length() - 1) {
    return false;
  }
  # if defined(_WIN32) || defined(__CYGWIN__)
  return (text.substr(1, 2) == ":\\");
  # else
  return (text[0] == '/');
  # endif
}
};
};
