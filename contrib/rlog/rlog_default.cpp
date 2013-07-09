#include <string>

#include "rlog_default.h"

using std::string;

namespace ltr {
Log::Log() {
  cout_log_ = Destination(new rlog::StdioNode(STDOUT_FILENO, DEFAULT_FLAGS));
  cerr_log_ = Destination(new rlog::StdioNode(STDERR_FILENO, DEFAULT_FLAGS));
}

void Log::subscribeCerr(const string& channel) {
  cerr_log_->subscribeTo(rlog::GetGlobalChannel(channel.c_str()));
}

void Log::subscribeCout(const string& channel) {
  cout_log_->subscribeTo(rlog::GetGlobalChannel(channel.c_str()));
}

void Log::subscribeFile(const string& filename, const string& channel) {
  if (files_.find(filename) == files_.end()) {
    files_[filename] = LogFile::Ptr(new LogFile(filename.c_str()));
  }
  files_[filename]->file_log->
      subscribeTo(rlog::GetGlobalChannel(channel.c_str()));
}

void Log::reset() {
  cerr_log_ = Destination(new rlog::StdioNode(STDERR_FILENO, DEFAULT_FLAGS));
  cout_log_ = Destination(new rlog::StdioNode(STDOUT_FILENO, DEFAULT_FLAGS));
  files_.clear();
}

Log::LogFile::LogFile(const string& filename) {
  #ifdef _WIN32
  _sopen_s(&file_descriptor, filename.c_str(), _O_APPEND | _O_RDWR | _O_CREAT,
      _SH_DENYNO, _S_IREAD | _S_IWRITE);
  #else
    file_descriptor = open(filename.c_str(), O_APPEND | O_RDWR | O_CREAT,
      S_IRUSR | S_IWUSR);
  #endif
  file_log = Destination(new rlog::StdioNode(file_descriptor, DEFAULT_FLAGS));
}

Log::LogFile::~LogFile() {
  #ifdef _WIN32
    _close(file_descriptor);
  #else
    close(file_descriptor);
  #endif
}

Log LOG;
};
