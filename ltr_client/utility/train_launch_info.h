// Copyright Yandex 2012
#ifndef LTR_CLIENT_UTILITY_TRAIN_LAUNCH_INFO_H_
#define LTR_CLIENT_UTILITY_TRAIN_LAUNCH_INFO_H_

#include <string>

#include "boost/unordered_set.hpp"

using std::string;
/**
 * Contains all the information about the train launch,
 * including name, data iinformation and learner information.
 * Also it contains the flag that defines if the library will
 * generate the C++ code or not.
 */
struct TrainLaunchInfo {
  TrainLaunchInfo() {}
  // C++11 -> we have no initializer, so we MUST write stupid
  // conctructor manually
  TrainLaunchInfo(const string& nm,
             const string& dt,
             const string& lr):
    name(nm), data(dt), learner(lr) { gen_cpp = false; }
  string name;
  string data;
  string learner;
  boost::unordered_set<string> predicts;
  bool gen_cpp;
};
/**
 * Converts TrainLaunchInfo to string
 * @param Info - TrinLaunchInfo to convert
 * @returns string with all the information about TrainLaunchInfo
 */
string ToString(const TrainLaunchInfo& Info);

#endif //LTR_CLIENT_UTILITY_TRAIN_LAUNCH_INFO_H_
