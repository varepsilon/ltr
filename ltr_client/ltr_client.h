// Copyright 2012 Yandex

#ifndef LTR_CLIENT_LTR_CLIENT_H_
#define LTR_CLIENT_LTR_CLIENT_H_

#include <string>

#include "ltr/learners/learner.h"

#include "ltr_client/configuration.h"
#include "ltr_client/config_parser.h"
#include "ltr_client/dependency_resolving_factory.h"
#include "ltr_client/utility/object_info.h"

using std::string;
using ltr::Learner;

/**
 * \brief Performs all the application actions.
 */
class LtrClient {
 public:
  /**
   * Basic constructor for LtrClient.
   */
  LtrClient();
  /**
   * Basic destructor for LtrClient.
   */
  ~LtrClient();
  /**
   * Reads all the settings from the config file specified.
   *
   * \param file_name is a string with the name of config file.
   */
  void initFrom(const string& file_name);
  /**
   * Starts all the computations according to the settings read from config.
   */
  void launch();
  /**
   * Starts train according to the settings read from config.
   */
  void launchTrain();
  /**
   * Starts crossvalidation according to the settings read from config.
   */
  void launchCrossvalidation();
  /**
   * Saves the report to file.
   *
   * \param file_name is a string with the name of report file.
   */
  void saveReport(const string& file_name) const;

 private:
  /**
   * Adds given text to report.
   *
   * \param text is a text to be added
   */
  void addToReport(const string& text);

  template <class TElement>
  void launchTrainImpl(typename Learner<TElement>::Ptr learner,
                       const TrainLaunchInfo::Ptr train_info);

  template <class TElement>
  void launchCrossvalidationImpl(
    const CrossvalidationLaunchInfo::Ptr crossvalidation_info);

  ConfigParser config_parser_;
  Configuration::Ptr configuration_;
  DependencyResolvingFactory factory_;
  string report_body_;
};

#endif  // LTR_CLIENT_LTR_CLIENT_H_
