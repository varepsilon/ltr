#include <sstream>

#include "ltr_client/utility/data_info.h"

using std::stringstream;

string ToString(const DataInfo& info) {;
  stringstream out(stringstream::out);
  out << "TDataInfo(name=" << info.name
      << ", approach=" << info.approach
      << ", format=" << info.format
      << ", file_name=" << info.file
      << ")";
  return out.str();
}
