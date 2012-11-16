#include <rlog/rlog.h> // one should include this file if uses the following rlog macros

rDebug("Some debug information");
rInfo("Some info");
rWarning("Some warning with int %d", 4);
rError("Some error information with float %f", 2.4);
