
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "logger.h"

using std::string;

namespace logger {
  Logger Logger::kDefaultOne;

  Logger::Logger()
  {
    // impossible is nothing
  }

  class Logger::Sync::Mutex {
  public:
    boost::interprocess::interprocess_mutex instance;
  };

  Logger::Sync::Sync() : 
  mutex(new Logger::Sync::Mutex())
  {
  }

  Logger::Sync::~Sync() {
    delete mutex;
  }

  Logger& Logger::Get() {
    return kDefaultOne;
  }

  void Logger::Init(const string& logFile, LogLevel level) {
    Close();
    {
      boost::interprocess::scoped_lock< boost::interprocess::interprocess_mutex > lock(sync.mutex->instance);

      log_file.open(logFile.c_str(), std::ios_base::app);
      if(!log_file.is_open())
      {
        std::cout << "Closed!" << std::endl;
      }
      if(!log_file.good())
      {
        std::cout << "Bad!" << std::endl;
      }
      log_level = level;
    }
  }

  void Logger::Close() {
    boost::interprocess::scoped_lock< boost::interprocess::interprocess_mutex > lock(sync.mutex->instance);

    if(log_file.is_open()) {
      log_file.close();
    }
    log_file.clear();
    log_level = Logger::LL_SILENT;
  }

  bool Logger::Inited() {
    boost::interprocess::scoped_lock< boost::interprocess::interprocess_mutex > lock(sync.mutex->instance);

    return log_file.good();
  }

  Logger::LogLevel Logger::logLevel() {
    return log_level;
  }

  void Logger::LogMessage(Logger::LogLevel level, const string& message, bool echoInStd, const string& loggerAlias) {
    boost::interprocess::scoped_lock< boost::interprocess::interprocess_mutex > lock(sync.mutex->instance);

    if(level >= log_level && log_level < Logger::LL_SILENT)
    {
      log_file << "[" << timer::formatTime() << "]\t" << loggerAlias << ":\t";

      switch(level) {
      case Logger::LL_INFO:
        log_file << "Information";
        break;
      case Logger::LL_WARNING:
        log_file << "Warning";
        break;
      case Logger::LL_ERROR:
        log_file << "Error";
        break;
      };

      log_file << ":\t" << message << std::endl;
    }

    if(echoInStd)
    {
      if(level >= Logger::LL_DEBUG && level <= Logger::LL_WARNING) {
        std::cout << message << std::endl;
      } else if(level == Logger::LL_ERROR) {
        std::cerr << message << std::endl;
      }
    }
  }

  void Logger::LogDebug(const string& message, bool echoInStd, const string& loggerAlias) {
    Logger::LogMessage(Logger::LL_DEBUG, message, echoInStd, loggerAlias);
  }

  void Logger::LogInfo(const string& message, bool echoInStd, const string& loggerAlias) {
    Logger::LogMessage(Logger::LL_INFO, message, echoInStd, loggerAlias);
  }

  void Logger::LogWarning(const string& message, bool echoInStd, const string& loggerAlias) {
    Logger::LogMessage(Logger::LL_WARNING, message, echoInStd, loggerAlias);
  }

  void Logger::LogError(const string& message, bool echoInStd, const string& loggerAlias) {
    Logger::LogMessage(Logger::LL_ERROR, message, echoInStd, loggerAlias);
  }

  LogStream::LogStream(Logger::LogLevel level, const string& alias, bool echoInStd, Logger& logger) :
  level_(level),
  alias_(alias),
  echoing_(echoInStd),
  logger_(logger)
  {
  }

  const LogStream& LogStream::operator<<(std::ostream& (*p)(std::ostream&)) const{
    if(p == (std::ostream& (*)(std::ostream&))std::endl) {
      logger_.LogMessage(level_, buffer_.str(), echoing_, alias_);
      buffer_.str("");
    } else {
      buffer_ << p;
    }
    return *this;
  }

  void LogStream::Output(std::ostream& output) const{
     string str_ = buffer_.str();
     output << str_;
  };

  void LogStream::width(size_t width_to_be_set) const {
    buffer_.width(width_to_be_set); 
  };
   
  const string& LogStream::Alias() const {
    return alias_;
  }

  Logger::LogLevel LogStream::level() const {
    return level_;
  }

  bool LogStream::echoing() const {
    return echoing_;
  }

  PrintLogger::PrintLogger(const string& alias, bool echoInStd, Logger& logger) :
  alias_(alias),
  echoing_(echoInStd),
  error_(Logger::LL_ERROR, alias, echoInStd, logger),
  warning_(Logger::LL_WARNING, alias, echoInStd, logger),
  info_(Logger::LL_INFO, alias, echoInStd, logger),
  debug_(Logger::LL_DEBUG, alias, true, logger)
  {
  }

  const LogStream& PrintLogger::info() const{
    return info_;
  }

  const LogStream& PrintLogger::warning() const{
    return warning_;
  }

  const LogStream& PrintLogger::error() const{
    return error_;
  }

  const string& PrintLogger::Alias() const {
    return alias_;
  }

  bool PrintLogger::echoing() const {
    return echoing_;
  };
};
