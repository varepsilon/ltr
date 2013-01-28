// Copyright Yandex 2013
#include "ltr/utility/cross_platform_time.h"


#ifdef _WIN32
#include <Windows.h>
namespace ltr {
namespace utility {

/* taken from this stackoverflow question
 * http://stackoverflow.com/questions/5404277/porting-clock-gettime-to-windows
 */
static LARGE_INTEGER getFILETIMEoffset() {
  SYSTEMTIME s;
  FILETIME f;
  LARGE_INTEGER t;

  s.wYear = 1970;
  s.wMonth = 1;
  s.wDay = 1;
  s.wHour = 0;
  s.wMinute = 0;
  s.wSecond = 0;
  s.wMilliseconds = 0;
  SystemTimeToFileTime(&s, &f);
  t.QuadPart = f.dwHighDateTime;
  t.QuadPart <<= 32;
  t.QuadPart |= f.dwLowDateTime;
  return (t);
}

double time() {
  LARGE_INTEGER t;
  FILETIME f;
  double microseconds;
  static LARGE_INTEGER offset;
  static double frequencyToSeconds;
  static int initialized = 0;
  static BOOL usePerformanceCounter = 1;

  if (!initialized) {
      LARGE_INTEGER performanceFrequency;
      initialized = 1;
      usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
      if (usePerformanceCounter) {
          QueryPerformanceCounter(&offset);
          frequencyToSeconds = static_cast<double>(
          performanceFrequency.QuadPart);
      } else {
          offset = getFILETIMEoffset();
          frequencyToSeconds = 10.;
      }
  }
  if (usePerformanceCounter) {
    QueryPerformanceCounter(&t);
  } else {
      GetSystemTimeAsFileTime(&f);
      t.QuadPart = f.dwHighDateTime;
      t.QuadPart <<= 32;
      t.QuadPart |= f.dwLowDateTime;
  }

  t.QuadPart -= offset.QuadPart;
  return (double)t.QuadPart / frequencyToSeconds;
}
}
}
#else  // !_WIN32
#include <time.h>
namespace ltr {
namespace utility {
double time() {
  struct timespec tv;
  clock_gettime(CLOCK_MONOTONIC, &tv);
  return tv.tv_sec + tv.tv_nsec * 1E-9;
}
}
}
#endif  // _WIN32

