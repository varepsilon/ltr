// Copyright 2012 Yandex

#include <iostream>

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

using std::cout;
using std::cin;
using std::cerr;
using std::endl;

using ltr::LOG;


void show_usage() {
  cout << "Usage: ltr_client -f <configfilename> [-v | -h]" << endl
       << "Executes commands defined in xml config file." << endl << endl
       << "Options:" << endl
       << "  -f,  filename\t\tconfig file name" << endl
       << "  -h,  help    \t\tdisplay this help and exit" << endl
       << "  -s,  silent  \t\tproduce no output to console"
       << " (overrides 'verbose')" << endl
       << "  -v,  verbose \t\tproduce additional output to console" << endl;
}


int main(int argc, char *argv[]) {
  GetOpt_pp args(argc, argv);
  args.exceptions_all();

  try {
    if (args >> OptionPresent('h', "help")) {
      show_usage();
      return 0;
    }

    LOG.subscribeCout("error");
    if (!(args >> OptionPresent('s', "silent"))) {
      if (args >> OptionPresent('v', "verbose")) {
        LOG.subscribeCout("info");
      }
      LOG.subscribeCout("warning");
    }
    LOG.subscribeFile("log.txt", "info");
    LOG.subscribeFile("log.txt", "warning");
    LOG.subscribeFile("log.txt", "error");

    string filename;
    args >> Option('f', "configfile", filename, "");
    if (filename.empty()) {
      cerr << "Configuration file name not specified."
           << " Type 'ltr_client -h' for usage." << endl;
      return 0;
    }
    args.end_of_options();

    Factory factory;
    RegisterAllTypes(&factory);

    LtrClient client;
    client.initFrom(filename);
    client.launch();
  } catch (TooManyOptionsEx) {
    cerr << "Unrecognized options specified."
         << " Type 'ltr_client -h' for usage." << endl;
  } catch (const GetOptEx& err) {
    cerr << "Invalid parameters. Type 'ltr_client -h' for usage." << endl;
  } catch (const logic_error& err) {
    cerr << "Failed: " << err.what() << endl;
  } catch (const std::exception err) {
    cerr << "Caught exception: " << err.what() << endl;
  }
  return 0;
}
