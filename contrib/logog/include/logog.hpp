/**
 * \file logog.hpp Main include file for logog logging functionality.  Include this file to enable logging for your application.
 */

#ifndef __LOGOG_HPP__
#define __LOGOG_HPP__

/** \def LOGOG_UNICODE
 ** Define this macro to enable Unicode support in logog.  The logog library works either in Unicode mode or not --
 ** attempting to mix the two log types will have unexpected results.
 **/
// #define LOGOG_UNICODE 1

// "logog/include/" prefix is added by ltr developers for convenience
#include "logog/include/const.hpp"
#include "logog/include/platform.hpp"
#include "logog/include/statics.hpp"
#include "logog/include/object.hpp"
#include "logog/include/timer.hpp"
#include "logog/include/mutex.hpp"
#include "logog/include/string.hpp"
#include "logog/include/node.hpp"
#include "logog/include/topic.hpp"
#include "logog/include/formatter.hpp"
#include "logog/include/target.hpp"
// #include "socket.hpp"
#include "logog/include/checkpoint.hpp"
#include "logog/include/api.hpp"
#include "logog/include/message.hpp"
#include "logog/include/macro.hpp"

#ifdef LOGOG_UNIT_TESTING
#include "logog/include/thread.hpp"
#include "logog/include/unittest.hpp"
#endif

//LTR HACK: in order to resolve conflicts with std::min, std::max
#ifdef min
#  undef min
#endif
#ifdef max
#  undef max
#endif

#endif // __LOGOG_HPP_
