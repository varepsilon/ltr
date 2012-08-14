#include "ltr_client/utility/crossvalidation_launch_info.h"
#include "ltr/utility/container_utility.h"

#include <sstream>

using ltr::utility::ToString;

using std::stringstream;

string ToString(const CrossvalidationLaunchInfo& info) {
  stringstream out(stringstream::out);
  out << "TCrossvalidationInfo(fold=" << info.splitter
      << ", learners=" << ToString(info.learners)
      << ", measures=" << ToString(info.measures)
      << ", datas=" << ToString(info.datas)
      << ")";
  return out.str();
}
