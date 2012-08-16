#include "ltr_client/utility/train_launch_info.h"
#include "ltr_client/configurator.h"

#include <sstream>

#include "ltr/utility/container_utility.h"

using std::stringstream;

using ltr::utility::ToString;

string ToString(const TrainLaunchInfo& info) {
  stringstream out(stringstream::out);
  out << "TTrainInfo(name=" << info.name
      << ", data=" << info.data
      << ", leatner=" << info.learner
      << ", predicts=" << ToString(info.predicts)
      << ", gen_cpp=" << info.gen_cpp
      << ")";
  return out.str();
}
