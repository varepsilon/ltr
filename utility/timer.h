#pragma once

#include <string>

using std::string;

namespace timer {
  /**
   * current time (UTC in millis)
   */
  long long now();

  string formatTime();
  void formatTime(string& to);

  class Timer {
  public:
    /**
     * Creates timer.
     */
    Timer();
    /**
     * Time of creation
     */
    long long startTime() const;
    /**
     * Force time of creation
     */
    void reset(long long time = now());
    /**
     * Formatted time period from moment of creation.
     * Example: "1 hour 12 min 0 sec 144 millis"
     */
    string operator()() const;
  private:
    long long start_time_;
  };
};
