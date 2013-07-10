// Copyright 2012 Yandex

#include <unistd.h>

#include <iostream>  // NOLINT
#include <string>
#include <vector>

#include "rlog/rlog_default.h"

#include "ltr_client/ltr_client.h"
#include "ltr_client/configuration.h"
#include "ltr_client/factory.h"
#include "ltr_client/registration.h"

#include "contrib/getopt_pp/getopt_pp.h"

using GetOpt::GetOpt_pp;
using GetOpt::Option;
using GetOpt::OptionPresent;
using GetOpt::GetOptEx;
using GetOpt::TooManyOptionsEx;
using GetOpt::GlobalOption;

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;

using ltr::LOG;


void show_usage() {
  cout << "Usage: ltr_client -h" << endl
       << "       ltr_client <config_filename> [OPTIONS]" << endl
       << "Executes commands defined in xml config file." << endl
       << endl
       << "Options:" << endl
       << "  -h, --help                   \t\t"
          "display this help and exit" << endl
       << "  -s, --silent                 \t\t"
          "produce no output to console" << endl
       << "  -w, --warning                \t\t"
          "produce warnings to console" << endl
       << "  -d, --debug                  \t\t"
          "produce debug output to console" << endl
       << "  -l, --logfile <log_filename> \t\t"
          "produce output to file" << endl;
}


int main(int argc, char *argv[]) {
  GetOpt_pp args(argc, argv);
  args.exceptions_all();

  try {
    if (args >> OptionPresent('h', "help")) {
      show_usage();
      return 0;
    }

    LOG.subscribeCerr("error");
    if (args >> OptionPresent('d', "debug")) {
      LOG.subscribeCerr("debug");
    }
    if (args >> OptionPresent('l', "logfile")) {
      string logfile;
      args >> Option('l', "logfile", logfile);
      LOG.subscribeFile(logfile, "error");
      if (!(args >> OptionPresent('s', "silent"))) {
        if (args >> OptionPresent('w', "warning")) {
          LOG.subscribeFile(logfile, "warning");
        }
        LOG.subscribeFile(logfile, "info");
      }
      if (args >> OptionPresent('d', "debug")) {
        LOG.subscribeFile(logfile, "debug");
      }
    } else {
      if (!(args >> OptionPresent('s', "silent"))) {
        if (args >> OptionPresent('w', "warning")) {
          LOG.subscribeCout("warning");
        }
        LOG.subscribeCout("info");
      }
    }
    rInfo("LTR Client. Copyright 2013 Yandex");

    vector<string> config_files;
    args >> GlobalOption(config_files);
    if (config_files.size() == 0) {
      cerr << "Configuration file name not specified." << endl
           << "Type 'ltr_client -h' for usage." << endl;
      return 0;
    }
    if (config_files.size() > 1) {
      cerr << "You can specify only one config file." << endl
           << "Type 'ltr_client -h' for usage." << endl;
      return 0;
    }
    string filename = config_files[0];
    rInfo("Config filename: %s", filename.c_str());
    args.end_of_options();

    Factory factory;
    RegisterAllTypes(&factory);

    LtrClient client;
    client.initFrom(filename);
    client.launch();
  } catch (TooManyOptionsEx) {
    cerr << "Unrecognized options specified." << endl
         << "Type 'ltr_client -h' for usage." << endl;
  } catch (const GetOptEx& err) {
    cerr << "Invalid parameters." << endl
         << "Type 'ltr_client -h' for usage." << endl;
  } catch (const logic_error& err) {
    rError("Logic error: %s", err.what());
  } catch (const std::exception err) {
    rError("Caught exception: %s", err.what());
  }
  return 0;
}
