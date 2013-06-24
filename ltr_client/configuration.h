// Copyright 2012 Yandex

#ifndef LTR_CLIENT_CONFIGURATION_H_
#define LTR_CLIENT_CONFIGURATION_H_

#include <string>

#include "ltr_client/utility/crossvalidation_launch_info.h"
#include "ltr_client/utility/data_info.h"
#include "ltr_client/utility/train_launch_info.h"
#include "ltr_client/utility/object_info.h"

#include "ltr/utility/safe_map.hpp"
#include "ltr/utility/macros.h"

using std::string;

using ltr::utility::SafeMap;

/**
 * \brief Contains info parsed from config file.
 */
struct Configuration {
  ALLOW_SHARED_PTR_ONLY_CREATION(Configuration);
 public:
  typedef SafeMap<string, DataInfo::Ptr> DataInfos;
  typedef SafeMap<string, ObjectInfo::Ptr> ObjectInfos;
  typedef SafeMap<string, TrainLaunchInfo::Ptr> TrainInfos;
  typedef SafeMap<string, CrossvalidationLaunchInfo::Ptr> CrossvalidationInfos;

  /**
   * Path to root directory
   */
  string root_path;
  /**
   * Information about datasets
   */
  DataInfos data_infos;
  /**
   * Information about any parameterized objects
   */
  ObjectInfos object_infos;
  /**
   * Information about training
   */
  TrainInfos train_infos;
  /**
   * Information about crossvalidation
   */
  CrossvalidationInfos crossvalidation_infos;
};

#endif  // LTR_CLIENT_CONFIGURATION_H_
