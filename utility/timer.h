// Copyright 2011 Yandex

#ifndef UTILITY_TIMER_H_
#define UTILITY_TIMER_H_

#include <string>
#include "boost/cstdint.hpp"

using std::string;
using boost::int64_t;

namespace timer {
  /**
   * current time (UTC in millis)
   */
  int64_t now();

  string formatTime();
  void formatTime(string* to);

  class Timer {
  public:
    /**
     * Creates timer.
     */
    Timer();
    /**
     * Time of creation
     */
    int64_t startTime() const;
    /**
     * Force time of creation
     */
    void reset(int64_t time = now());
    /**
     * Formatted time period from moment of creation.
     * Example: "1 hour 12 min 0 sec 144 millis"
     */
    string operator()() const;
  private:
    int64_t start_time_;
  };
};

#endif  // UTILITY_TIMER_H_
