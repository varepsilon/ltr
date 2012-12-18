#ifndef _rlog_default_
#define _rlog_default_

#include <rlog/rlog.h>
#include <rlog/RLogChannel.h>
#include <rlog/RLogNode.h>
#include <rlog/StdioNode.h>

#include "ltr/utility/boost/shared_ptr.h"
#include <map>
#include <string>
#include <stdexcept>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef _WIN32
  #include <io.h>
#else
  #include <unistd.h>
#endif


using std::map;
using std::string;
using std::logic_error;
using std::pair;
using ltr::utility::shared_ptr;

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif

#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

namespace ltr {
/**
 * A class for hiding the most details of rlog using and for providing
 * a default logging behaviour
 */
class Log {
public:
  /**
   * rlog::StdioNode represents an output logfile with its subscriptions
   * to different channels (for example a channel could include all
   * error-leveled messages)
   */
  typedef shared_ptr<rlog::StdioNode> Destination;
  /**
   * By default ltr::Log subscribes the stderr to error messages and
   * the ltr.log file to all messages
   */
  Log();
  /**
   * Subscribes cerr to a particular message channel - "error", "warning", "info",
   * "debug", or an empty channel "" for all above
   */
  void subscribeCerr(const string& channel);
  /**
   * Subscribes cout to a particular message channel - "error", "warning", "info",
   * "debug", or an empty channel "" for all above
   */
  void subscribeCout(const string& channel);
  /**
   * Subscribes a file to a particular message channel - "error", "warning", "info",
   * "debug", or an empty channel "" for all above
   */
  void subscribeFile(const string& filename, const string& channel = "");
  /**
   * Deletes all message subscriptions, to any file, cerr and cout
   */
  void reset();
private:
  /**
   * LogFile holds a file and rlog::StdioNode, attached to that file.
   * LogFile is a private class, used only in ltr::Log
   */
  class LogFile {
    int file_descriptor;
  public:
    typedef shared_ptr<LogFile> Ptr;
    LogFile(const string& filename);
    ~LogFile();
    Destination file_log;
  };
  /**
   * Default flags for logging - outputting time, file, line and log level. For example:
   * \include example_output.txt
   */
  static const int DEFAULT_FLAGS =
    (int)rlog::StdioNode::OutputContext
    | (int)rlog::StdioNode::OutputChannel;
  Destination cerr_log_;
  Destination cout_log_;
  map<string, LogFile::Ptr> files_;
};
/**
 * Global Log object, that provides the default logging behaviour
 */
static Log LOG;
};

#endif // _rlog_default_
