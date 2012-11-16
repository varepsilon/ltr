// Comments describe logging behaviour at their place

// here: no logging at all
// For obtaining the proper default logging behaviour one should include rlog/rlog_default.h, not the rlog/rlog.h
#include <rlog/rlog_default.h>
// default logging behaviour here: errors to stderr and all (every message) to ltr.log
ltr::LOG.subscribeCout("warning");
// here: errors to stderr, warnings to stdout and all to ltr.log
ltr::LOG.subscribeFile("every_message.txt");
// here: errors to stderr, warnings to stdout, all to ltr.log and every_message.txt
ltr::LOG.reset();
// here: no logging at all
ltr::LOG.subscribeFile("info_and_debug.txt", "info");
ltr::LOG.subscribeFile("info_and_debug.txt", "debug");
// here: info and debug to info_and_debug.txt
ltr::LOG.subscribeCerr("error");
ltr::LOG.subscribeCerr("warning");
// here: info and debug to info_and_debug.txt, warnings and errors to stderr
