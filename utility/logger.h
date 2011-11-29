// Copyright 2011 Yandex

#ifndef UTILITY_LOGGER_H_
#define UTILITY_LOGGER_H_

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

#include "utility/timer.h"

using std::string;

namespace logger {
  class Logger {
  public:
    enum LogLevel {
      LL_DEBUG = -1,
      LL_INFO = 0,
      LL_WARNING = 1,
      LL_ERROR = 2,
      LL_SILENT = 3
    };

    /**
     * Returns default logger.
     */
    static Logger& Get();

    /**
     * Opens file stream to write messages to
     * @param logFile - log file's name
     * @param level - log's message level (all messages with lower level will not write into file)
     */
    void Init(const string& logFile, LogLevel level = LL_INFO);

    /**
     * Close file stream
     */
    void Close();

    /**
     * Writes message to log file
     * @param level - message log level (if below Logger::logLevel message will be ignored)
     * @param message - message to log
     * @param echoInStd - weather echo this message to standart output (cout for DEBUG, INFO ans WARNING and cerr for ERROR)
     * @param loggerAlias - alias of logger for log message
     */
    void LogMessage(LogLevel level, const string& message,
                    bool echoInStd, const string& loggerAlias = "");

    /**
     * Writes log message with level DEBUG
     * @see LogMessage
     */
    void LogDebug(const string& message,
                  bool echoInStd,
                  const string& loggerAlias="");

    /**
     * Writes log message with level INFO
     * @see LogMessage
     */
    void LogInfo(const string& message,
                 bool echoInStd,
                 const string& loggerAlias="");

    /**
     * Writes log message with level WARNING
     * @see LogMessage
     */
    void LogWarning(const string& message,
                    bool echoInStd,
                    const string& loggerAlias="");

    /**
     * Writes log message with level ERROR
     * @see LogMessage
     */
    void LogError(const string& message,
                  bool echoInStd,
                  const string& loggerAlias="");

    /**
     * Returns true if file stream is opened and good, false otherwise
     */
    bool Inited();

    /**
     * logger level (from initialization)
     */
    Logger::LogLevel logLevel();

  private:
    Logger();
    static Logger kDefaultOne;

    std::ofstream log_file;
    volatile Logger::LogLevel log_level;

    struct Sync {
    public:
      Sync();
      ~Sync();
      class Mutex;
      Mutex* mutex;
    };

    Sync sync;
  };

  class LogStream {
  public:
    LogStream(Logger::LogLevel level = Logger::LL_INFO,
              const string& alias = "",
              bool echoInStd = true,
              Logger& logger = Logger::Get());
    template< typename T >
    const LogStream& operator<<(const T&) const;
    const LogStream& operator<<(std::ostream& (*p)(std::ostream&)) const;
    void Output(std::ostream* output) const;
    void width(size_t width_to_be_set) const;

    const string& Alias() const;
    Logger::LogLevel level() const;
    bool echoing() const;
  private:
    mutable Logger& logger_;
    LogStream(const LogStream&);
    void operator=(const LogStream&);
    Logger::LogLevel level_;
    bool echoing_;
    string alias_;
    mutable std::stringstream buffer_;
  };

  class PrintLogger {
  public:
    /**
     * Creates set of logger stream for message types: DEBUG, INFO, WARNING, ERROR
     * @param alias - logger's alias
     * @param echoInStd - wether or not echo messages in standart output
     * @param logger - logger to be used
     */
    PrintLogger(const string& alias = "",
                bool echoInStd = true,
                Logger& logger = Logger::Get());

    /**
     * returns stream to write DEBUG messages
     * Notice: debug channel always echoing in std
     */
    const LogStream& debug() const;

    /**
     * returns stream to write INFO messages
     */
    const LogStream& info() const;

    /**
     * returns stream to write WARNING messages
     */
    const LogStream& warning() const;

    /**
     * returns stream to write ERROR messages
     */
    const LogStream& error() const;

    /**
     * return's alias for log messages
     */
    const string& Alias() const;

    /**
     * is messages echoing in standart output
     */
    bool echoing() const;

  private:
    bool echoing_;
    string alias_;
    mutable LogStream error_;
    mutable LogStream warning_;
    mutable LogStream info_;
    mutable LogStream debug_;
  };

  template< typename T >
  const LogStream& LogStream::operator<<(const T& t) const {
    buffer_ << t;
    return *this;
  };
};

#endif  // UTILITY_LOGGER_H_
