// Copyright 2011 Yandex

#include <boost/date_time/local_time/local_time.hpp>
#include <boost/lexical_cast.hpp>

#include "utility/timer.h"

using boost::gregorian::date;
using boost::posix_time::ptime;
using boost::posix_time::microsec_clock;
using boost::lexical_cast;

namespace timer {
  class Local {
  public:
    static string align(const string& str, size_t width) {
      if (width > str.size()) {
        return string(width-str.size(), '0') + str;
      } else {
        return str;
      }
    }
  };

  int64_t now() {
    return (microsec_clock::universal_time() -
            ptime(date(1970, 1, 1))).total_milliseconds();
  }

  string formatTime() {
    string result;
    formatTime(&result);
    return result;
  }

  void formatTime(string* to) {
    boost::posix_time::ptime currTime(
        boost::posix_time::second_clock::local_time());
    *to =
      lexical_cast< string >(currTime.date().year()) +
      "-" +
      Local::align(
            lexical_cast<string>(currTime.date().month().as_number()), 2) +
      "-" +
      Local::align(lexical_cast<string>(currTime.date().day().as_number()), 2) +
      " " +
      Local::align(lexical_cast<string>(currTime.time_of_day().hours()), 2) +
      ":" +
      Local::align(lexical_cast<string>(currTime.time_of_day().minutes()), 2) +
      ":" +
      Local::align(lexical_cast<string>(currTime.time_of_day().seconds()), 2);
  }

  Timer::Timer() :
  start_time_(now()) {
  }

  int64_t Timer::startTime() const {
    return start_time_;
  }

  void Timer::reset(int64_t time) {
    start_time_ = time;
  }

  string Timer::operator()() const {
    int64_t time = now() - start_time_;
    string result;
    int sec_shift = 1;
    if (time > 60 * 1000) {  // mins
      int min_shift = 1;
      if (time > 60 * 60 * 1000) {  // hours
        int hours_shift = 1;
        if (time > 24 * 60 * 60 * 1000) {  // days
          result += lexical_cast< string >(time/(24*60*60*1000)) + " days ";
          hours_shift = 2;
        }
        result +=
            Local::align(
                lexical_cast<string>(time/(60*60*1000)%24), hours_shift)
               + " hours ";
        min_shift = 2;
      }
      result +=
          Local::align(
                lexical_cast<string>(time/(60*1000)%60), min_shift)
             + " minutes ";
      sec_shift = 2;
    }
    result +=
      Local::align(lexical_cast< string >(time/1000%60), sec_shift) +
      " seconds " +
      Local::align(lexical_cast< string >(time%1000), 3) +
      " millis";
    return result;
  };
};
