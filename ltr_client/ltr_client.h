// Copyright 2012 Yandex

#ifndef LTR_CLIENT_LTR_CLIENT_H_
#define LTR_CLIENT_LTR_CLIENT_H_

#include <string>

#include "ltr/learners/learner.h"

#include "ltr_client/configurator.h"
#include "ltr_client/dependency_resolving_factory.h"
#include "ltr_client/utility/parameterized_info.h"

using std::string;
using ltr::Learner;

/**
 * Performs all the application actions
 */
class LtrClient {
 public:
  /**
   * Basic constructor for LtrClient
   */
  LtrClient();
  /**
   * Basic destructor for LtrClient
   */
  ~LtrClient();
  /**
   * Reads all the settings from the config file specified
   * @param name - string with the name of config file
   */
  void initFrom(const string& file_name);
  /**
   * Starts all the computations according to the settings read from config
   */
  void launch();
  /**
   * Starts train according to the settings read from config
   */
  void launchTrain();
  /**
   * Starts crossvalidation according to the settings read from config
   */
  void launchCrossvalidation();
  /**
   * Saves the report to file
   * @param name - string with the name of report file
   */
  void saveReport(const string& file_name) const;

 private:
  /**
   * Adds given text to report
   * @param text - text to be added
   */
  void addToReport(const string& text);

  template <class TElement>
  void launchTrainImpl(typename Learner<TElement>::Ptr learner,
                   const TrainLaunchInfo& train_info);

  template <class TElement>
  void launchCrossvalidationImpl(
    const CrossvalidationLaunchInfo& crossvalidation_info);

  ConfigParser configurator_;
  DependencyResolvingFactory factory_;
  string report_body_;
};

#endif  // LTR_CLIENT_LTR_CLIENT_H_
