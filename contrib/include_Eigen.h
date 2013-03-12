//Copyright 2013 Yandex

#ifndef CONTRIB_INCLUDE_EIGEN_H_
#define CONTRIB_INCLUDE_EIGEN_H_

#ifdef __unix__
  #include <unistd.h>
#endif

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)
  #pragma GCC diagnostic ignored "-Wenum-compare"
#endif

#include <Eigen/Dense>

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)
  #pragma GCC diagnostic warning "-Wenum-compare"
#endif

#endif  // CONTRIB_INCLUDE_EIGEN_H_

