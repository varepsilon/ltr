// Copyright Yandex 2012
#ifndef LTR_CLIENT_UTILITY_CROSSVALIDATION_LAUNCH_INFO_H_
#define LTR_CLIENT_UTILITY_CROSSVALIDATION_LAUNCH_INFO_H_

#include <string>

#include "ltr/interfaces/printable.h"
#include "ltr/utility/safe_set.hpp"
#include "ltr/utility/macros.h"

using std::string;

using ltr::Printable;
using ltr::utility::SafeSet;

/**
 * \brief Contains the information about the crossvalidation launch,
 * including the leaners vector, measures vector, datasets vector and the
 * splitter. Must be used by shared_ptr only.
 */
struct CrossvalidationLaunchInfo: public Printable {
  ALLOW_SHARED_PTR_ONLY_CREATION(CrossvalidationLaunchInfo);
 public:
  string toString() const {
    return "CrossvalidationLaunchInfo(fold=" + splitter +
        ", learners=" + learners.toString() +
        ", measures=" + measures.toString() +
        ", datas=" + datas.toString() +
        ")";
  }
  /**
   * Basic constructor
   */
  CrossvalidationLaunchInfo() { }
  /**
   * Basic constructor.
   *
   * \param fold_ - splitter fold
   */
  explicit CrossvalidationLaunchInfo(const string& fold_)
    : splitter(fold_) { }

  /**
   * Splitter fold
   */
  string splitter;
  /**
   * Set of names of using learners.
   */
  SafeSet<string> learners;
  /**
   * Set of names of using measures.
   */
  SafeSet<string> measures;
  /**
   * Set of names of using datasets.
   */
  SafeSet<string> datas;
};
#endif  // LTR_CLIENT_UTILITY_CROSSVALIDATION_LAUNCH_INFO_H_

