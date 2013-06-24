// Copyright Yandex 2012
#ifndef LTR_CLIENT_UTILITY_TRAIN_LAUNCH_INFO_H_
#define LTR_CLIENT_UTILITY_TRAIN_LAUNCH_INFO_H_

#include <string>

#include "ltr/interfaces/printable.h"
#include "ltr/utility/safe_set.hpp"

using std::string;

using ltr::Printable;
using ltr::utility::SafeSet;

/**
 * \brief Contains all the information about the train launch,
 * including name, data iinformation and learner information.
 * Also it contains the flag that defines if the library will
 * generate the C++ code or not.
 */
struct TrainLaunchInfo: public Printable {
  ALLOW_SHARED_PTR_ONLY_CREATION(TrainLaunchInfo);
 public:
  /**
   * Basic constructor
   */
  TrainLaunchInfo(): gen_cpp(false) { }
  /**
   * Basic constructor.
   *
   * \param name_ - name of the train launch
   * \param data_ - dataset to launch train
   * \param learner_ - learner to launch train
   */
  TrainLaunchInfo(const string& name_,
                  const string& data_,
                  const string& learner_):
      name(name_), data(data_), learner(learner_), gen_cpp(false) { }
  string toString() const {
    return "TrainLaunchInfo(name=" + name +
        ", data=" + data +
        ", learner=" + learner +
        ", predicts=" + predicts.toString() +
        ", gen_cpp=" + lexical_cast<string>(gen_cpp) +
        ")";
  }

  /**
   * Name of the train launch
   */
  string name;
  /**
   * Dataset for train launch
   */
  string data;
  /**
   * Learner for train launch
   */
  string learner;
  /**
   * Datasets to predict
   */
  SafeSet<string> predicts;
  /**
   * C++ code generation flag
   */
  bool gen_cpp;
};

#endif  // LTR_CLIENT_UTILITY_TRAIN_LAUNCH_INFO_H_
