// Copyright 2012 Yandex

#include "ltr/utility/html.h"
#include "ltr/utility/boost/path.h"

using ltr::utility::IsAbsoluteFileName;
using ltr::utility::GetFileName;

namespace ltr {
namespace utility {

string FileLink(const string& filename) {
  if (IsAbsoluteFileName(filename)) {
    return "<a href=\"" + filename + "\">" + GetFileName(filename) + "</a>";
  }
  return filename;
}
};
};
