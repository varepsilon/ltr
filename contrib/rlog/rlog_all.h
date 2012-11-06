#ifndef _rlog_all_
#define _rlog_all_

// include all
#include <rlog/Error.h>
#include <rlog/Lock.h>
#include <rlog/Mutex.h>
#include <rlog/RLogChannel.h>
#include <rlog/rloginit.h>
#include <rlog/rloglocation.h>
#include <rlog/RLogNode.h>
#include <rlog/RLogPublisher.h>
#include <rlog/RLogTime.h>
#include <rlog/StdioNode.h>
// ~include all

#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <stdexcept>
#include <fcntl.h>

using std::map;
using std::string;
using std::logic_error;
using std::pair;

#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

namespace ltr {
  class Log {
  public:
    typedef boost::shared_ptr<rlog::StdioNode> NodePtr;
    Log() {
      cerr_log_ = NodePtr(new rlog::StdioNode(STDERR_FILENO, default_flags));
      cerr_log_->subscribeTo(rlog::GetGlobalChannel("error"));
      defaultAddFile("default_log.txt");
    }
    void addFile(const string& filename) {
      files_[filename] = LogFile::Ptr(new LogFile(filename.c_str()));
    }
    void defaultAddFile(const string& filename) {
      files_[filename] = LogFile::Ptr(new DefaultLogFile(filename.c_str()));
    }
    void removeFile(const string& filename) {
      if (files_.find(filename) == files_.end()) {
        throw logic_error("No such log file");
      }
      files_.erase(filename);
    }
    NodePtr file(const string& filename) {
      if (files_.find(filename) == files_.end()) {
        throw logic_error("No such log file");
      }
      return files_.find(filename)->second->file_log;
    }
    NodePtr cerr() {
      return cerr_log_;
    }
    void clearCerr() {
      cerr_log_ = NodePtr(new rlog::StdioNode(STDERR_FILENO, default_flags));
    }
  private:
    class LogFile {
      int file_descriptor;
    public:
      typedef boost::shared_ptr<LogFile> Ptr;
      LogFile(const char* filename) {
        #ifdef _WIN32
          _sopen_s(&file_descriptor, filename, _O_APPEND | _O_RDWR | _O_CREAT,
            _SH_DENYNO, _S_IREAD | _S_IWRITE);
        #else
          file_descriptor = open(filename, O_APPEND | O_RDWR | O_CREAT,
            S_IRUSR | S_IWUSR);
        #endif
        file_log = NodePtr(new rlog::StdioNode(file_descriptor, default_flags));
      }
      virtual ~LogFile() {
        #ifdef _WIN32
          _close(file_descriptor);
        #else
          close(file_descriptor);
        #endif
      }
      NodePtr file_log;
    };
    class DefaultLogFile : public LogFile {
    public:
      DefaultLogFile(const char* filename) : LogFile(filename) {
        file_log->subscribeTo(rlog::GetGlobalChannel(""));
      }
    };
    static const int default_flags =
      (int)rlog::StdioNode::OutputContext
      + (int)rlog::StdioNode::OutputChannel;
    NodePtr cerr_log_;
    map<string, LogFile::Ptr> files_;
  };

  static Log LOG;
};


#endif // _rlog_all_
