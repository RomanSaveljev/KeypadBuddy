#ifndef MONITORDEFINITIONS_H
#define MONITORDEFINITIONS_H

#include <e32def.h>

_LIT(KMonitorServerName, "KeyPadBuddyMonitor");
_LIT(KMonitorServerFileName, "KeyPadBuddyMonitor.exe");

enum TMonitorFunctions
    {
    EMonitorDeactivate = 1000,
    EMonitorResetCache
    };

#endif //MONITORDEFINITIONS_H
