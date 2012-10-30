// Copyright Yandex 2012
#ifndef LTR_CLIENT_UTILITY_CROSSVALIDATION_LAUNCH_INFO_H_
#define LTR_CLIENT_UTILITY_CROSSVALIDATION_LAUNCH_INFO_H_

#include <string>

#include "boost/unordered_set.hpp"

using std::string;
/**
 * Contains the information about the crossvalidation launch,
 * including the leaners vector, measures vector,
 * datasets vector and the splitter
 */
struct CrossvalidationLaunchInfo {
  CrossvalidationLaunchInfo() { }
  // C++11 -> we have no initializer, so we MUST write stupid
  // conctructor manually
  /**
   * Basic constructor.
   * @param fold_ - splitter fold
   */
  explicit CrossvalidationLaunchInfo(const string& fold_)
    : splitter(fold_) { }
  string splitter;
  boost::unordered_set<string> learners;
  boost::unordered_set<string> measures;
  boost::unordered_set<string> datas;
};
/**
 * Converts CrossvalidationLaunchInfo object into the printable string.
 * @param Info - CrossvalidationLaunchInfo object to convert
 * @returns converted string
 */
string ToString(const CrossvalidationLaunchInfo& Info);

#endif //LTR_CLIENT_UTILITY_CROSSVALIDATION_LAUNCH_INFO_H_

